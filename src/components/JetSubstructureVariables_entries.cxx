// JetSubstructureVariables_entries.cxx

#include "JetSubstructureVariables/Calculator.h"
#include "JetSubstructureVariables/Validator.h"
#include "GaudiKernel/DeclareFactoryEntries.h"


DECLARE_TOOL_FACTORY(Calculator)
DECLARE_TOOL_FACTORY(Validator)

DECLARE_FACTORY_ENTRIES(JetSubstructureVariables) {
  DECLARE_TOOL(Calculator)
  DECLARE_TOOL(Validator)
}

