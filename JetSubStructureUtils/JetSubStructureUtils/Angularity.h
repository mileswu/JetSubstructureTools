#ifndef jetsubstructureutils_angularity_header
#define jetsubstructureutils_angularity_header

#include "JetSubStructureUtils/SubstructureCalculator.h"

namespace JetSubStructureUtils {
  class Angularity : public SubstructureCalculator<double> { 
    public:
      using SubstructureCalculator::result;
      virtual double result(const fastjet::PseudoJet &jet) const;
  };
}

#endif
