#ifndef jetsubstructure_angularitytool_header
#define jetsubstructure_angularitytool_header

#include "JetSubStructure/JetSubStructureBase.h"

class AngularityTool :
  public JetSubStructureBase {
    ASG_TOOL_CLASS(AngularityTool, IJetModifier)
    
    public:
      // Constructor and destructor
      AngularityTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;
      
      double angularity(const xAOD::Jet &jet) const;
      double angularity(vector<fastjet::PseudoJet> &constit_pseudojets,
                        TLorentzVector jet_p4) const;

    private:
};


#endif
