// JetSubstructureTools_entries.cxx

#include "JetSubstructureTools/NSubjettinessTool.h"
#include "JetSubstructureTools/Validator.h"
#include "GaudiKernel/DeclareFactoryEntries.h"


DECLARE_TOOL_FACTORY(NSubjettinessTool)
DECLARE_TOOL_FACTORY(Validator)

DECLARE_FACTORY_ENTRIES(JetSubstructureTools) {
  DECLARE_TOOL(NSubjettinessTool)
  DECLARE_TOOL(Validator)
}

