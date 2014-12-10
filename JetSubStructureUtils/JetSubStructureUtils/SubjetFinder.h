#ifndef jetsubstructureutils_subjetfinder_header
#define jetsubstructureutils_subjetfinder_header

#include "JetSubStructureUtils/SubstructureCalculator.h"
#include <vector>

namespace JetSubStructureUtils {
  class SubjetFinder : public SubstructureCalculator<std::vector<fastjet::PseudoJet> > { 
    public:
      SubjetFinder();

      using SubstructureCalculator::result;
      virtual std::vector<fastjet::PseudoJet> result(const fastjet::PseudoJet &jet) const;

    private:
  };
}

#endif
