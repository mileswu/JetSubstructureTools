#ifndef jetsubstructure_ktsplittingscaletool_header
#define jetsubstructure_ktsplittingscaletool_header

#include "JetSubStructure/JetSubStructureBase.h"

class KTSplittingScaleTool :
  public JetSubStructureBase {
    ASG_TOOL_CLASS(KTSplittingScaleTool, IJetModifier)
    
    public:
      // Constructor and destructor
      KTSplittingScaleTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;
      
      double kTSplittingScale(const xAOD::Jet &jet, unsigned int nSubJets) const;
      double kTSplittingScale(vector<fastjet::PseudoJet> &constit_pseudojets,
                           unsigned int nSubJets) const;

      double zCut(const xAOD::Jet &jet, unsigned int nSubJets) const;
      double zCut(vector<fastjet::PseudoJet> &constit_pseudojets,
                  unsigned int nSubJets) const;


    private:
      
};


#endif
