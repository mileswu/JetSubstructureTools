#ifndef jetsubstructuremomenttools_pulltool_header
#define jetsubstructuremomenttools_pulltool_header

#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"

class PullTool :
  public JetSubStructureMomentToolsBase {
    ASG_TOOL_CLASS(PullTool, IJetModifier)
    
    public:
      // Constructor and destructor
      PullTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;

    private:
      bool m_useEtaInsteadOfY;
};


#endif
