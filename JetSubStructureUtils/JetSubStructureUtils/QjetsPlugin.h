#ifndef jetsubstructureutils_qjetsplugin_header
#define jetsubstructureutils_qjetsplugin_header

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

namespace JetSubStructureUtils {
  class QjetsPlugin: public fastjet::JetDefinition::Plugin {
    private:
      double _zcut, _dcut_fctr, _exp_min, _exp_max, _rigidity;
    public:
      QjetsPlugin(double zcut, double dcut_fctr, double exp_min, double exp_max, double rigidity);
      double R() const;
      std::string description() const;
      void run_clustering(fastjet::ClusterSequence & cs) const;
  };
}

#endif
