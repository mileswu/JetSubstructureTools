#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructure/DipolarityTool.h"
#include "JetEDM/JetConstituentFiller.h"
#include "CLHEP/Vector/TwoVector.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

DipolarityTool::DipolarityTool(std::string name) : 
  JetSubStructureBase(name)
{
  declareProperty("SubJetRadius", m_SubJetRadius = 0.3);
}

int DipolarityTool::modifyJet(xAOD::Jet &jet) const {
  jet.setAttribute("Dip12", dipolarity(jet, 1, 2));
  jet.setAttribute("Dip13", dipolarity(jet, 1, 3));
  jet.setAttribute("Dip23", dipolarity(jet, 2, 3));
  jet.setAttribute("DipExcl12", exclusiveDipolarity(jet, 1, 2));

  return 0;
}

double DipolarityTool::dipolarity(const xAOD::Jet &jet, unsigned int n1, unsigned n2) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);

  return dipolarity(constit_pseudojets, n1, n2);
}

double DipolarityTool::dipolarity(vector<fastjet::PseudoJet> &constit_pseudojets,
                                 unsigned int n1, unsigned int n2) const
{
  // Validate inputs
  if(n2 <= n1 || n1 == 0) {
    ATH_MSG_ERROR("dipolarity was called with invalid arguments");
    return 0.0;
  }

  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, m_SubJetRadius,
                                                          fastjet::E_scheme, fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);
  vector<fastjet::PseudoJet> kt_subjets = fastjet::sorted_by_pt(kt_clust_seq.inclusive_jets(5000.0));

  if(kt_subjets.size() < n2) {
    ATH_MSG_INFO("Not enough subjets");
    return 0.0;
  }

  fastjet::PseudoJet jet1, jet2;
  jet1 = kt_subjets[n1-1];
  jet2 = kt_subjets[n2-1];
  return dipolarity(constit_pseudojets, jet1, jet2);
}

double DipolarityTool::exclusiveDipolarity(const xAOD::Jet &jet, unsigned int n1, unsigned n2) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);

  return exclusiveDipolarity(constit_pseudojets, n1, n2);
}

double DipolarityTool::exclusiveDipolarity(vector<fastjet::PseudoJet> &constit_pseudojets,
                                 unsigned int n1, unsigned int n2) const
{
  // Validate inputs
  if(n2 <= n1 || n1 == 0) {
    ATH_MSG_ERROR("exclusiveDipolarity was called with invalid arguments");
    return 0.0;
  }
  if(n2 > constit_pseudojets.size()) {
    ATH_MSG_INFO("We were asked to calculate exclusiveDipolarity, but there are not enough constituents");
    return 0.0;
  }

  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, 1.5,
                                                          fastjet::E_scheme, fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);
  vector<fastjet::PseudoJet> kt_subjets = fastjet::sorted_by_pt(kt_clust_seq.exclusive_jets((int)n2));

  fastjet::PseudoJet jet1, jet2;
  jet1 = kt_subjets[n1-1];
  jet2 = kt_subjets[n2-1];
  return dipolarity(constit_pseudojets, jet1, jet2);
}

double DipolarityTool::dipolarity(vector<fastjet::PseudoJet> &constit_pseudojets,
                                  fastjet::PseudoJet jet1, fastjet::PseudoJet jet2) const
{
  float dipolarity = 0;
  float sumpt = 0;

  CLHEP::Hep2Vector v12(jet2.eta() - jet1.eta(), jet1.delta_phi_to(jet2));
  if(v12.mag2() < 0.001) return -1;

  for(unsigned int iConstit = 0; iConstit < constit_pseudojets.size(); iConstit++) {
    fastjet::PseudoJet constituent = constit_pseudojets[iConstit];

    sumpt += constituent.perp();

    CLHEP::Hep2Vector v;

    v.setX(constituent.eta() - jet1.eta());
    v.setY(jet1.delta_phi_to(constituent));
    float test = v.dot(v12.unit());

    if(test < 0)
      dipolarity += constituent.perp()*v.mag2(); 
    else if(test < v12.mag())
      dipolarity += constituent.perp()*(v.mag2() - pow(v.dot(v12.unit()), 2));                                              
    else {
      v.setX(constituent.eta() - jet2.eta());
      v.setY(jet2.delta_phi_to(constituent));
      dipolarity += constituent.perp()*v.mag2();                                                                           
    }
  }

  if(sumpt < 0.001) return -1;
  return dipolarity/(sumpt*v12.mag2()); 
}
