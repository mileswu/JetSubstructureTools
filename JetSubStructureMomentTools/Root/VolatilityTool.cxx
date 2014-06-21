#include "JetSubStructureMomentTools/VolatilityTool.h"
#include "JetSubStructureUtils/Volatility.h"
#include <sstream>

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
  declareProperty("TruncationFactor", m_truncation_fctr = 0.0);
}

int VolatilityTool::modifyJet(xAOD::Jet &jet) const {
  vector<float> truncations;
  truncations.push_back(0.5);
  truncations.push_back(0.2);
  truncations.push_back(0.1);
  truncations.push_back(0.05);
  truncations.push_back(0.02);
  truncations.push_back(0.01);
  truncations.push_back(0.0);

  for(unsigned int i=0; i<truncations.size(); i++) {
    stopwatches[truncations[i]].Start(kFALSE);
    JetSubStructureUtils::Volatility volatility(m_num_iterations, m_zcut, m_dcut_fctr,
      m_exp_min, m_exp_max, m_rigidity, truncations[i]);
    stringstream ss;
    ss << "Volatility" << truncations[i];
    jet.setAttribute(ss.str(), volatility.result(jet));
    stopwatches[truncations[i]].Stop();
    cout << ss.str() << " = " << stopwatches[truncations[i]].CpuTime() << endl;
  }

  return 0;
}
