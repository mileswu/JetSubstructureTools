#ifndef jetsubstructuretools_nsubjetinesstool_header
#define jetsubstructuretools_nsubjetinesstool_header

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

class NSubjettinessTool :
  virtual public IJetModifier,
  virtual public asg::AsgTool {
    ASG_TOOL_CLASS(NSubjettinessTool, IJetModifier)
    
    public:
      // Constructor and destructor
      NSubjettinessTool(std::string name);

      int modify(xAOD::JetContainer &jets) const;
      int modify(xAOD::Jet &jet) const;
      
      double nSubjettiness(const xAOD::Jet &jet, unsigned int nSubJets) const;
      double nSubjettiness(vector<fastjet::PseudoJet> &constit_pseudojets,
                           unsigned int nSubJets, float jetRadius) const;

    private:
      float m_Alpha;
      
};


#endif
