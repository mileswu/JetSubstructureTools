#ifndef jetsubstructurevariables_calculator_header
#define jetsubstructurevariables_calculator_header

#include "xAODCaloEvent/CaloCluster.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

#include "AsgTools/AsgTool.h"
#include "JetInterface/IJetModifier.h"
#include <vector>

using namespace std;

namespace fastjet {
  class PseudoJet;
}

class Calculator :
  virtual public IJetModifier,
  virtual public asg::AsgTool {
    ASG_TOOL_CLASS(Calculator, IJetModifier)
    
    public:
      // Constructor and destructor
      Calculator(std::string name);

      int modify(xAOD::JetContainer &jets) const;
      int modify(xAOD::Jet &jet) const;
      
      double nSubJettiness(const xAOD::Jet &jet, unsigned int nSubJets) const;
      double nSubJettiness(vector<fastjet::PseudoJet> &constit_pseudojets, unsigned int nSubJets) const;

    private:
      
};


#endif
