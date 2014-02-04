// JetSubStructure_entries.cxx

#include "JetSubStructure/NSubjettinessTool.h"
#include "JetSubStructure/Validator.h"
#include "GaudiKernel/DeclareFactoryEntries.h"


DECLARE_TOOL_FACTORY(NSubjettinessTool)
DECLARE_TOOL_FACTORY(Validator)

DECLARE_FACTORY_ENTRIES(JetSubStructure) {
  DECLARE_TOOL(NSubjettinessTool)
  DECLARE_TOOL(Validator)
}

