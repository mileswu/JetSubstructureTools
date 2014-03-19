#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/KTSplittingScaleTool.h"
#include "JetEDM/JetConstituentFiller.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

KTSplittingScaleTool::KTSplittingScaleTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
}

int KTSplittingScaleTool::modifyJet(xAOD::Jet &jet) const {
  jet.setAttribute("Split12", kTSplittingScale(jet, 1));
  jet.setAttribute("Split23", kTSplittingScale(jet, 2));
  jet.setAttribute("Split34", kTSplittingScale(jet, 3));
  jet.setAttribute("ZCut12", zCut(jet, 1));
  jet.setAttribute("ZCut23", zCut(jet, 2));
  jet.setAttribute("ZCut34", zCut(jet, 3));

  return 0;
}

double KTSplittingScaleTool::kTSplittingScale(const xAOD::Jet &jet, unsigned int nSubJets) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);

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



double KTSplittingScaleTool::zCut(const xAOD::Jet &jet, unsigned int nSubJets) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);
  return zCut(constit_pseudojets, nSubJets);
}

double KTSplittingScaleTool::zCut(vector<fastjet::PseudoJet> &constit_pseudojets,
                                  unsigned int nSubJets) const
{
  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, 1.5,
                                                          fastjet::E_scheme, fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);

  if(constit_pseudojets.size() < nSubJets) {
    ATH_MSG_DEBUG("We were asked to calculate zCut, but there are not enough constituents");
    return 0.0;
  }
  vector<fastjet::PseudoJet> subjets = kt_clust_seq.exclusive_jets((int)nSubJets);

  // Find last split jet (cluster_hist_index should be highest for the last created jet)
  fastjet::PseudoJet *lastSplitSubjet = NULL;
  int max_cluster_hist_index = -1;
  for(size_t iSubjet=0; iSubjet < subjets.size(); iSubjet++) {
    fastjet::PseudoJet parent1, parent2;
    if(kt_clust_seq.has_parents(subjets[iSubjet], parent1, parent2) &&
            subjets[iSubjet].cluster_hist_index() > max_cluster_hist_index) {
      max_cluster_hist_index = subjets[iSubjet].cluster_hist_index();
      lastSplitSubjet = &subjets[iSubjet];
    }
  }

  if(lastSplitSubjet == NULL) {
    ATH_MSG_DEBUG("None of the subjets were split.");
    return 0.0;
  }

  double dmin = pow(kTSplittingScale(constit_pseudojets, nSubJets), 2.0);
  double zcut = dmin / (dmin + lastSplitSubjet->m2());
  return zcut;
}
