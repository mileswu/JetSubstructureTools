#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructure/KTDeltaRTool.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

KTDeltaRTool::KTDeltaRTool(std::string name) : 
  AsgTool(name)
{
}

int KTDeltaRTool::modify(xAOD::Jet &jet) const {
  jet.setAttribute("KtDR", kTDeltaR(jet));

  return 0;
}

double KTDeltaRTool::kTDeltaR(const xAOD::Jet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = getConstituentPseudoJets(jet);

  return kTDeltaR(constit_pseudojets);
}

double KTDeltaRTool::kTDeltaR(vector<fastjet::PseudoJet> &constit_pseudojets) const
{
  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, 0.4,
                                                          fastjet::E_scheme, fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);

  vector<fastjet::PseudoJet> jets = kt_clust_seq.exclusive_jets(2);
  if(jets.size() != 2) {
    ATH_MSG_INFO("Hi");
    return 0.0;
  }

  double ktdr = jets[0].delta_R(jets[1]);
  double ktdr2 = sqrt(pow(jets[0].eta() - jets[1].eta(), 2) + pow(jets[0].delta_phi_to(jets[1]), 2));
  cout << "ktdr=" << ktdr << " ktdr2=" << ktdr2 << endl;
  return ktdr;
}
