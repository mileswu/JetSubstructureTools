// KtDeltaRTool.cxx

#include "JetSubStructureMomentTools/KtDeltaRTool.h"
#include "JetSubStructureUtils/KtDeltaR.h"

KtDeltaRTool::KtDeltaRTool(const std::string& myname)
: JetModifierBase(myname) {
  declareProperty("JetRadius", m_jetrad =0.4);
}

StatusCode KtDeltaRTool::initialize() {
  return StatusCode::SUCCESS;
}

int KtDeltaRTool::modifyJet(xAOD::Jet& jet) const {
  KtDeltaR ktdr(m_jetrad);
  jet.setAttribute("KtDR", ktdr.result(jet));
  return 0;
}

void KtDeltaRTool::print() const {
  ATH_MSG_INFO("Reclustering size parameter: " << m_jetrad);
}

