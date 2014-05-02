#ifndef jetsubstructuremomenttools_nsubjetinesstool_header
#define jetsubstructuremomenttools_nsubjetinesstool_header

#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"
#include "TStopwatch.h"

class NSubjettinessTool :
  public JetSubStructureMomentToolsBase {
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
      mutable TStopwatch normal, onepass, wta, old;
      
};


#endif
