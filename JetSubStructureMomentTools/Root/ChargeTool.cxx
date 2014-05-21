#include "JetSubStructureMomentTools/ChargeTool.h"
#include "JetSubStructureUtils/Charge.h"

using namespace std;

ChargeTool::ChargeTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  declareProperty("k", m_k = 1.0);
}

int ChargeTool::modifyJet(xAOD::Jet &jet) const {
  JetSubStructureUtils::Charge charge(m_k);
  jet.setAttribute("Charge", charge.result(jet));
  return 0;
}
