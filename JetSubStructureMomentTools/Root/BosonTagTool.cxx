#include "JetSubStructureMomentTools/BosonTagTool.h"
#include "JetSubStructureUtils/BosonTag.h"

using namespace std;

BosonTagTool::BosonTagTool(std::string name) :
  JetSubStructureMomentToolsBase(name)
{
  declareProperty("WorkingPoint", m_working_point               = "medium");
  declareProperty("TaggerAlgorithm", m_tagger_alg               = "tau21");
#ifdef ROOTCORE
  declareProperty("RecommendationsFile", m_recommendations_file = "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_20150306.dat");
#else
  declareProperty("RecommendationsFile", m_recommendations_file =  "JetSubStructureUtils/data/config_13TeV_20150306.dat");
#endif
  declareProperty("ApplyMassCut", m_compare_mass                = true);
  declareProperty("ApplyVariableCut", m_compare_variable        = true);
  declareProperty("EnableSimpleTagger", m_simple_tagger         = false);
  declareProperty("Debug", m_debug                              = false);
}

int BosonTagTool::modifyJet(xAOD::Jet &jet) const
{
  JetSubStructureUtils::BosonTag tagger(m_working_point, m_tagger_alg, m_recommendations_file, m_compare_mass, m_compare_variable, m_simple_tagger, m_debug);
  jet.setAttribute("BosonTag", static_cast<int>(tagger.result(jet)));

  return 0;
}
