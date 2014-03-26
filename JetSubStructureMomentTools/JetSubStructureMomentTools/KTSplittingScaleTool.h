#ifndef jetsubstructuremomenttools_ktsplittingscaletool_header
#define jetsubstructuremomenttools_ktsplittingscaletool_header

#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"

class KTSplittingScaleTool :
  public JetSubStructureMomentToolsBase {
    ASG_TOOL_CLASS(KTSplittingScaleTool, IJetModifier)
    
    public:
      // Constructor and destructor
      KTSplittingScaleTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;
};


#endif
