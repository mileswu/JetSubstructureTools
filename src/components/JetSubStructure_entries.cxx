// JetSubStructure_entries.cxx

#include "JetSubStructure/NSubjettinessTool.h"
#include "JetSubStructure/KTSplittingScaleTool.h"
#include "JetSubStructure/KTDeltaRTool.h"
#include "JetSubStructure/Validator.h"
#include "GaudiKernel/DeclareFactoryEntries.h"


DECLARE_TOOL_FACTORY(NSubjettinessTool)
DECLARE_TOOL_FACTORY(KTSplittingScaleTool)
DECLARE_TOOL_FACTORY(KTDeltaRTool)
DECLARE_TOOL_FACTORY(Validator)

DECLARE_FACTORY_ENTRIES(JetSubStructure) {
  DECLARE_TOOL(NSubjettinessTool)
  DECLARE_TOOL(KTSplittingScaleTool)
  DECLARE_TOOL(KTDeltaRTool)
  DECLARE_TOOL(Validator)
}

