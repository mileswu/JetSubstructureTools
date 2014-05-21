#ifndef jetsubstructuremomenttools_chargetool_header
#define jetsubstructuremomenttools_chargetool_header

#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"

class ChargeTool :
  public JetSubStructureMomentToolsBase {
    ASG_TOOL_CLASS(ChargeTool, IJetModifier)
    
    public:
      // Constructor and destructor
      ChargeTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;

    private:
      double m_k;
};


#endif
