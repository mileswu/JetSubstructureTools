#include "JetSubStructureMomentTools/PullTool.h"
#include "JetSubStructureUtils/Pull.h" 

using namespace std;
using fastjet::PseudoJet;

PullTool::PullTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  ATH_MSG_DEBUG("Initializing Pull tool.");
  declareProperty("UseEtaInsteadOfY", m_useEtaInsteadOfY = false);
}

int PullTool::modifyJet(xAOD::Jet &jet) const {
  JetSubStructureUtils::Pull pull(m_useEtaInsteadOfY);
  map<string, double> values = pull.result(jet);
  jet.setAttribute("PullMag", values["PullMag"]);
  jet.setAttribute("PullPhi", values["PullPhi"]);
  jet.setAttribute("Pull_C00", values["Pull_C00"]);
  jet.setAttribute("Pull_C01", values["Pull_C01"]);
  jet.setAttribute("Pull_C10", values["Pull_C10"]);
  jet.setAttribute("Pull_C11", values["Pull_C11"]);
  return 0;
}
