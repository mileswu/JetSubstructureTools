#ifndef jetsubstructure_dipolaritytool_header
#define jetsubstructure_dipolaritytool_header

#include "JetSubStructure/JetSubStructureBase.h"

class DipolarityTool :
  public JetSubStructureBase {
    ASG_TOOL_CLASS(DipolarityTool, IJetModifier)
    
    public:
      // Constructor and destructor
      DipolarityTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;
      
      double dipolarity(const xAOD::Jet &jet, unsigned int n1, unsigned int n2) const;
      double dipolarity(vector<fastjet::PseudoJet> &constit_pseudojets,
                           unsigned int n1, unsigned int n2) const;
      double exclusiveDipolarity(const xAOD::Jet &jet, unsigned int n1, unsigned int n2) const;
      double exclusiveDipolarity(vector<fastjet::PseudoJet> &constit_pseudojets,
                           unsigned int n1, unsigned int n2) const;

      double dipolarity(vector<fastjet::PseudoJet> &constit_pseudojets,
                        fastjet::PseudoJet jet1, fastjet::PseudoJet jet2) const;

    private:
      double m_SubJetRadius;
      
};


#endif
