#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructure/KTSplittingScaleTool.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

KTSplittingScaleTool::KTSplittingScaleTool(std::string name) : 
  AsgTool(name)
{
}

int KTSplittingScaleTool::modify(xAOD::Jet &jet) const {
  jet.setAttribute("SPLIT12", kTSplittingScale(jet, 1));
  jet.setAttribute("SPLIT23", kTSplittingScale(jet, 2));
  jet.setAttribute("SPLIT34", kTSplittingScale(jet, 3));

  return 0;
}

double KTSplittingScaleTool::kTSplittingScale(const xAOD::Jet &jet, unsigned int nSubJets) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = getConstituentPseudoJets(jet);

  return kTSplittingScale(constit_pseudojets, nSubJets);
}

double KTSplittingScaleTool::kTSplittingScale(vector<fastjet::PseudoJet> &constit_pseudojets,
                                 unsigned int nSubJets) const
{
  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, 1.5,
                                                          fastjet::E_scheme, fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);
  fastjet::PseudoJet kt_jet = fastjet::sorted_by_pt(kt_clust_seq.inclusive_jets()).front();
  double split = 1.5*sqrt(kt_clust_seq.exclusive_subdmerge(kt_jet, nSubJets));
  return split;
}
