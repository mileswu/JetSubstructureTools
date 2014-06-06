#include "JetSubStructureUtils/QjetsPlugin.h"
#include "Qjets.h"

using namespace JetSubStructureUtils;

QjetsPlugin::QjetsPlugin(double zcut, double dcut_fctr, double exp_min, double exp_max, double rigidity)
  : _zcut(zcut), _dcut_fctr(dcut_fctr), _exp_min(exp_min), _exp_max(exp_max), _rigidity(rigidity)
{
}

double QjetsPlugin::R() const {
  return 0.;
}

std::string QjetsPlugin::description() const {
  std::string desc("Qjets pruning plugin");
  return desc;
}

void QjetsPlugin::run_clustering(fastjet::ClusterSequence & cs) const {
  Qjets qjets(_zcut, _dcut_fctr, _exp_min, _exp_max, _rigidity);
  qjets.Cluster(cs);
}
