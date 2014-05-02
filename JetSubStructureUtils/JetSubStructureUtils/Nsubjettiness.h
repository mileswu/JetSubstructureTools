#ifndef jetsubstructureutils_nsubjettiness_header
#define jetsubstructureutils_nsubjettiness_header

#include "JetSubStructureUtils/SubstructureCalculator.h"
#include "fastjet/contrib/Nsubjettiness.hh"

namespace JetSubStructureUtils {

  class Nsubjettiness : public SubstructureCalculator<double>,
                           public fastjet::contrib::Nsubjettiness { 
    public:
      Nsubjettiness(int N, fastjet::contrib::Njettiness::AxesMode axes_mode,
          fastjet::contrib::Njettiness::MeasureMode measure_mode,
          double para1 = NAN, double para2 = NAN, double para3 = NAN, double para4 = NAN) :
        fastjet::contrib::Nsubjettiness(N, axes_mode, measure_mode, para1, para2, para3, para4) {};

      Nsubjettiness(int N, fastjet::contrib::Njettiness::AxesMode axes_mode, double beta, double R0,
          double Rcutoff=std::numeric_limits<double>::max()) :
        fastjet::contrib::Nsubjettiness(N, axes_mode, beta, R0, Rcutoff) {};

      using SubstructureCalculator::result;
      virtual double result(const fastjet::PseudoJet &jet) const {
        return fastjet::contrib::Nsubjettiness::result(jet);
      };

      virtual std::string description() const{
        return fastjet::contrib::Nsubjettiness::description();
      };
  };
}

#endif
