// JetSubStructure_entries.cxx

#include "JetSubStructure/NSubjettinessTool.h"
#include "JetSubStructure/KTSplittingScaleTool.h"
#include "JetSubStructure/DipolarityTool.h"
#include "JetSubStructure/Validator.h"
#include "GaudiKernel/DeclareFactoryEntries.h"


DECLARE_TOOL_FACTORY(NSubjettinessTool)
DECLARE_TOOL_FACTORY(KTSplittingScaleTool)
DECLARE_TOOL_FACTORY(DipolarityTool)
DECLARE_TOOL_FACTORY(Validator)

DECLARE_FACTORY_ENTRIES(JetSubStructure) {
  DECLARE_TOOL(NSubjettinessTool)
  DECLARE_TOOL(KTSplittingScaleTool)
  DECLARE_TOOL(DipolarityTool)
  DECLARE_TOOL(Validator)
}

