#include "JetSubStructureMomentTools/EnergyCorrelatorTool.h"
#include "JetSubStructureUtils/EnergyCorrelator.h" 

using namespace std;
using fastjet::PseudoJet;

EnergyCorrelatorTool::EnergyCorrelatorTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  ATH_MSG_DEBUG("Initializing EnergyCorrelator tool.");
  declareProperty("Beta", m_Beta = 1.0);
  declareProperty("IncludeBeta2", m_includeBeta2 = false);
}

int EnergyCorrelatorTool::modifyJet(xAOD::Jet &jet) const {
  if(checkForConstituents(jet) == false) return 1;

  JetSubStructureUtils::EnergyCorrelator ECF1(1, m_Beta, JetSubStructureUtils::EnergyCorrelator::pt_R);
  JetSubStructureUtils::EnergyCorrelator ECF2(2, m_Beta, JetSubStructureUtils::EnergyCorrelator::pt_R);
  JetSubStructureUtils::EnergyCorrelator ECF3(3, m_Beta, JetSubStructureUtils::EnergyCorrelator::pt_R);
  jet.setAttribute("ECF1", ECF1.result(jet));
  jet.setAttribute("ECF2", ECF2.result(jet));
  jet.setAttribute("ECF3", ECF3.result(jet));

  if(m_includeBeta2) {
    JetSubStructureUtils::EnergyCorrelator ECF1_beta2(1, 2.0, JetSubStructureUtils::EnergyCorrelator::pt_R);
    JetSubStructureUtils::EnergyCorrelator ECF2_beta2(2, 2.0, JetSubStructureUtils::EnergyCorrelator::pt_R);
    JetSubStructureUtils::EnergyCorrelator ECF3_beta2(3, 2.0, JetSubStructureUtils::EnergyCorrelator::pt_R);
    jet.setAttribute("ECF1_Beta2", ECF1_beta2.result(jet));
    jet.setAttribute("ECF2_Beta2", ECF2_beta2.result(jet));
    jet.setAttribute("ECF3_Beta2", ECF3_beta2.result(jet));
  }

  return 0;
}
