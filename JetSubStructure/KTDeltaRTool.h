#ifndef jetsubstructure_ktdeltartool_header
#define jetsubstructure_ktdeltartool_header

#include "JetSubStructure/JetSubStructureBase.h"

class KTDeltaRTool :
  public JetSubStructureBase,
  virtual public asg::AsgTool {
    ASG_TOOL_CLASS(KTDeltaRTool, IJetModifier)
    
    public:
      // Constructor and destructor
      KTDeltaRTool(std::string name);

      using JetSubStructureBase::modify;
      int modify(xAOD::Jet &jet) const;
      
      double kTDeltaR(const xAOD::Jet &jet) const;
      double kTDeltaR(vector<fastjet::PseudoJet> &constit_pseudojets) const;

    private:
      
};


#endif
