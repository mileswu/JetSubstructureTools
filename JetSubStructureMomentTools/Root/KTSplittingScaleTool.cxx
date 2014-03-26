#include "JetSubStructureMomentTools/KTSplittingScaleTool.h"
#include "JetSubStructureUtils/KtSplittingScale.h"
#include "JetSubStructureUtils/ZCut.h"

using namespace std;
using fastjet::PseudoJet;

KTSplittingScaleTool::KTSplittingScaleTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
}

int KTSplittingScaleTool::modifyJet(xAOD::Jet &jet) const
{
  KtSplittingScale split12(1);
  KtSplittingScale split23(2);
  KtSplittingScale split34(3);
  jet.setAttribute("Split12", split12.result(jet));
  jet.setAttribute("Split23", split23.result(jet));
  jet.setAttribute("Split34", split34.result(jet));

  ZCut zcut12(1);
  ZCut zcut23(2);
  ZCut zcut34(1);
  jet.setAttribute("ZCut12", zcut12.result(jet));
  jet.setAttribute("ZCut23", zcut23.result(jet));
  jet.setAttribute("ZCut34", zcut34.result(jet));

  return 0;
}
