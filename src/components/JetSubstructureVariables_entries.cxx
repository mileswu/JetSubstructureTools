// JetSubstructureVariables_entries.cxx

#include "JetSubstructureVariables/NSubjettinessTool.h"
#include "JetSubstructureVariables/Validator.h"
#include "GaudiKernel/DeclareFactoryEntries.h"


DECLARE_TOOL_FACTORY(NSubjettinessTool)
DECLARE_TOOL_FACTORY(Validator)

DECLARE_FACTORY_ENTRIES(JetSubstructureVariables) {
  DECLARE_TOOL(NSubjettinessTool)
  DECLARE_TOOL(Validator)
}

