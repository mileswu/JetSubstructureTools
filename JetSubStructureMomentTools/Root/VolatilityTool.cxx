#include "JetSubStructureMomentTools/VolatilityTool.h"
#include "JetSubStructureUtils/Volatility.h"

using namespace std;
using fastjet::PseudoJet;

VolatilityTool::VolatilityTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  ATH_MSG_DEBUG("Initializing volatility tool.");
  declareProperty("NumberOfIterations", m_num_iterations = 100);
  declareProperty("ZCut", m_zcut = 0.01);
  declareProperty("DCut", m_dcut_fctr = 0.5);
  declareProperty("ExpMin", m_exp_min = 0.0);
  declareProperty("ExpMax", m_exp_max = 0.0);
  declareProperty("Rigidity", m_rigidity = 0.1);
}

int VolatilityTool::modifyJet(xAOD::Jet &jet) const {
  JetSubStructureUtils::Volatility volatility(m_num_iterations, m_zcut, m_dcut_fctr,
      m_exp_min, m_exp_max, m_rigidity);
  double val = volatility.result(jet);
  ATH_MSG_VERBOSE("Adding jet volatility: " << val);
  jet.setAttribute("Volatility", val);
  return 0;
}