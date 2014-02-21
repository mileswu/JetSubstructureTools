#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/KtMassDropTool.h"
#include "JetEDM/JetConstituentFiller.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

KtMassDropTool::KtMassDropTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  ATH_MSG_DEBUG("Initializing KtMassDrop tool.");
}

int KtMassDropTool::modifyJet(xAOD::Jet &jet) const {
  double val = ktMassDrop(jet);
  ATH_MSG_VERBOSE("Adding jet ktMassDrop: " << val);
  jet.setAttribute("Mu12", val);
  return 0;
}

double KtMassDropTool::ktMassDrop(const xAOD::Jet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);

  return ktMassDrop(constit_pseudojets, jet.p4());
}

double KtMassDropTool::ktMassDrop(vector<fastjet::PseudoJet> &constit_pseudojets, TLorentzVector jet_p4) const
{
  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, 1.5,
                                                          fastjet::E_scheme, fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);
  fastjet::PseudoJet kt_jet = fastjet::sorted_by_pt(kt_clust_seq.inclusive_jets()).front();

  fastjet::PseudoJet p1, p2;
  if(kt_jet.has_parents(p1, p2)) {
    return fmax(p1.m(), p2.m()) / jet_p4.M();
  }

  // No parents
  return -1;
}

