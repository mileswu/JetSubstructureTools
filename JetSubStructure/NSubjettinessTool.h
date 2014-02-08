#ifndef jetsubstructure_nsubjetinesstool_header
#define jetsubstructure_nsubjetinesstool_header

#include "JetSubStructure/JetSubStructureBase.h"

class NSubjettinessTool :
  public JetSubStructureBase {
    ASG_TOOL_CLASS(NSubjettinessTool, IJetModifier)

    public:
      // Constructor and destructor
      NSubjettinessTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;
      
      double nSubjettiness(const xAOD::Jet &jet, unsigned int nSubJets) const;
      double nSubjettiness(vector<fastjet::PseudoJet> &constit_pseudojets,
                           unsigned int nSubJets, float jetRadius) const;

    private:
      float m_Alpha;
      
};


#endif
