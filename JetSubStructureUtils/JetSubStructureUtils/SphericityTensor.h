#ifndef jetsubstructuremoments_sphericitytensor_header
#define jetsubstructuremoments_sphericitytensor_header

#include "JetSubStructureUtils/SubstructureMultiCalculator.h"

class SphericityTensor : public SubstructureMultiCalculator { 
  public:
    using SubstructureMultiCalculator::result;
    virtual std::map<std::string, double> result(std::vector<fastjet::PseudoJet> &constit_pseudojets,
        fastjet::PseudoJet jet_p4) const;
};

#endif
