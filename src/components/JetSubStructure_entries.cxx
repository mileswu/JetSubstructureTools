// JetSubStructure_entries.cxx

#include "JetSubStructure/KtDeltaRTool.h"
#include "JetSubStructure/NSubjettinessTool.h"
#include "JetSubStructure/KTSplittingScaleTool.h"
#include "JetSubStructure/DipolarityTool.h"
#include "JetSubStructure/AngularityTool.h"
#include "JetSubStructure/Validator.h"
#include "GaudiKernel/DeclareFactoryEntries.h"


DECLARE_TOOL_FACTORY(KtDeltaRTool)
DECLARE_TOOL_FACTORY(NSubjettinessTool)
DECLARE_TOOL_FACTORY(KTSplittingScaleTool)
DECLARE_TOOL_FACTORY(DipolarityTool)
DECLARE_TOOL_FACTORY(AngularityTool)
DECLARE_TOOL_FACTORY(Validator)

DECLARE_FACTORY_ENTRIES(JetSubStructure) {
  DECLARE_TOOL(KtDeltaRTool)
  DECLARE_TOOL(NSubjettinessTool)
  DECLARE_TOOL(KTSplittingScaleTool)
  DECLARE_TOOL(DipolarityTool)
  DECLARE_TOOL(AngularityTool)
  DECLARE_TOOL(Validator)
}

