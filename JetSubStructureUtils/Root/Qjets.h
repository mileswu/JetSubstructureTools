#ifndef jetsubstructureutils_qjets_header
#define jetsubstructureutils_qjets_header

#include <queue>
#include <vector>
#include <list>
#include <algorithm>
#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

namespace JetSubStructureUtils {
  struct jet_distance{
    double dij;
    int j1;
    int j2;
  };

  class JetDistanceCompare{
    public:
      JetDistanceCompare() {};
      bool operator() (const jet_distance& lhs, const jet_distance&rhs) const;
  };

  class Qjets{
    private:
      double _zcut, _dcut_fctr, _exp_min, _exp_max, _rigidity, _dcut;
      std::vector<int> _merged_jets;
      std::list<jet_distance> _distances;

      double d_ij(const fastjet::PseudoJet& v1, const fastjet::PseudoJet& v2);
      void ComputeDCut(fastjet::ClusterSequence & cs);

      double Rand();
      bool Prune(jet_distance& jd,fastjet::ClusterSequence & cs);
      bool JetsUnmerged(jet_distance& jd);
      bool JetUnmerged(int num);
      void ComputeNewDistanceMeasures(fastjet::ClusterSequence & cs, int new_jet);
      void ComputeAllDistances(const std::vector<fastjet::PseudoJet>& inp);  
      double ComputeMinimumDistance();
      double ComputeNormalization(double dmin);

    public:
      Qjets(double zcut, double dcut_fctr, double exp_min, double exp_max, double rigidity);
      void Cluster(fastjet::ClusterSequence & cs);
  };
}

#endif

