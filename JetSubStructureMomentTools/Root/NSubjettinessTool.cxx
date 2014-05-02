#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/NSubjettinessTool.h"
#include "JetEDM/JetConstituentFiller.h"
#include "JetSubStructureUtils/Nsubjettiness.h"
#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

NSubjettinessTool::NSubjettinessTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  declareProperty("Alpha", m_Alpha = 1.0);
}

int NSubjettinessTool::modifyJet(xAOD::Jet &jet) const {
  old.Start(kFALSE);
  jet.setAttribute("Tau1_old", nSubjettiness(jet, 1));
  jet.setAttribute("Tau2_old", nSubjettiness(jet, 2));
  jet.setAttribute("Tau3_old", nSubjettiness(jet, 3));
  old.Stop();

  normal.Start(kFALSE);
  JetSubStructureUtils::Nsubjettiness tau1(1, fastjet::contrib::Njettiness::kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  JetSubStructureUtils::Nsubjettiness tau2(2, fastjet::contrib::Njettiness::kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  JetSubStructureUtils::Nsubjettiness tau3(3, fastjet::contrib::Njettiness::kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  jet.setAttribute("Tau1", tau1.result(jet));
  jet.setAttribute("Tau2", tau2.result(jet));
  jet.setAttribute("Tau3", tau3.result(jet));
  normal.Stop();

  wta.Start(kFALSE);
  JetSubStructureUtils::Nsubjettiness tau1_wta(1, fastjet::contrib::Njettiness::wta_kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  JetSubStructureUtils::Nsubjettiness tau2_wta(2, fastjet::contrib::Njettiness::wta_kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  JetSubStructureUtils::Nsubjettiness tau3_wta(3, fastjet::contrib::Njettiness::wta_kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  jet.setAttribute("Tau1_wta", tau1_wta.result(jet));
  jet.setAttribute("Tau2_wta", tau2_wta.result(jet));
  jet.setAttribute("Tau3_wta", tau3_wta.result(jet));
  wta.Stop();

  onepass.Start(kFALSE);
  JetSubStructureUtils::Nsubjettiness tau1_onepass(1, fastjet::contrib::Njettiness::onepass_kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  JetSubStructureUtils::Nsubjettiness tau2_onepass(2, fastjet::contrib::Njettiness::onepass_kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  JetSubStructureUtils::Nsubjettiness tau3_onepass(3, fastjet::contrib::Njettiness::onepass_kt_axes,
      fastjet::contrib::Njettiness::normalized_measure, m_Alpha, jet.getSizeParameter());
  jet.setAttribute("Tau1_onepass", tau1_onepass.result(jet));
  jet.setAttribute("Tau2_onepass", tau2_onepass.result(jet));
  jet.setAttribute("Tau3_onepass", tau3_onepass.result(jet));
  onepass.Stop();

  cout << "Old = " << old.CpuTime() << endl;
  cout << "Normal = " << normal.CpuTime() << endl;
  cout << "WTA = " << wta.CpuTime() << endl;
  cout << "Onepass = " << onepass.CpuTime() << endl;

  return 0;
}

double NSubjettinessTool::nSubjettiness(const xAOD::Jet &jet, unsigned int nSubJets) const {
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);
  float jetRadius = jet.getSizeParameter();

  return nSubjettiness(constit_pseudojets, nSubJets, jetRadius);
}

double NSubjettinessTool::nSubjettiness(vector<fastjet::PseudoJet> &constit_pseudojets,
                                 unsigned int nSubJets, float jetRadius) const
{
  if(constit_pseudojets.size() < nSubJets) {
    ATH_MSG_DEBUG("We were asked to calculate nSubjettiness, but there are not enough constituents");
    return 0.0;
  }
                 
  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm,
                                                          M_PI/2,fastjet::E_scheme,
                                                          fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);
  vector<fastjet::PseudoJet> kt_subjets = kt_clust_seq.exclusive_jets((int)nSubJets);

  double tauNum = 0.0, tauDen = 0.0;

  for(size_t iC=0; iC < constit_pseudojets.size(); iC++) {
    
    double minDR = DBL_MAX;
    
    for(size_t iJ=0; iJ < kt_subjets.size(); iJ++) {
      double dr = sqrt(constit_pseudojets[iC].squared_distance(kt_subjets[iJ]));
      if(dr < minDR)
        minDR = dr;
    }

    tauNum += constit_pseudojets[iC].pt() * pow(minDR, m_Alpha);
    tauDen += constit_pseudojets[iC].pt() * pow(jetRadius, m_Alpha);
  }

  return tauNum/tauDen;
}
