// KtDeltaRTool.cxx

#include "JetSubStructureMomentTools/KtDeltaRTool.h"
#include "JetEDM/JetConstituentFiller.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"

KtDeltaRTool::KtDeltaRTool(const std::string& myname)
: JetModifierBase(myname) {
  declareProperty("JetRadius", m_jetrad =0.4);
}

StatusCode KtDeltaRTool::initialize() {
  return StatusCode::SUCCESS;
}

int KtDeltaRTool::modifyJet(xAOD::Jet& jet) const {
  jet.setAttribute("KtDR", ktDeltaR(jet));
  return 0;
}

void KtDeltaRTool::print() const {
  ATH_MSG_INFO("Reclustering size parameter: " << m_jetrad);
}

double KtDeltaRTool::ktDeltaR(const xAOD::Jet& jet) const {
  std::vector<fastjet::PseudoJet> constit_pseudojets =
    jet::JetConstituentFiller::constituentPseudoJets(jet, true);
  if ( constit_pseudojets.size() < 2 ) {
    ATH_MSG_DEBUG( "Jet has fewer than 2 constituents.");
    return 0.0;
  }
  ATH_MSG_DEBUG( "Finding jets from " << constit_pseudojets.size() << " input constituents.");
  fastjet::JetDefinition jetdef = fastjet::JetDefinition(fastjet::kt_algorithm, m_jetrad);
  fastjet::ClusterSequence cs(constit_pseudojets, jetdef);
  std::vector<fastjet::PseudoJet> outjets = cs.exclusive_jets(2);
  if ( outjets.size() < 2 ) {
    ATH_MSG_DEBUG( "Fewer than two subjets found.");
    return 0.0;
  }
  // TODO: Switch to JetUtils helpers for dR when package is migrated
  static double twopi = 2.0*acos(-1.0);
  double deta = outjets[1].eta() - outjets[0].eta();
  double dphi = std::abs(outjets[1].phi() - outjets[0].phi());
  if ( dphi > twopi ) dphi -= twopi;
  double dr = sqrt(deta*deta + dphi*dphi);
  return dr;
}
