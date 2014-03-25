#ifndef jetsubstructuremoments_substructuremulticalculator_header
#define jetsubstructuremoments_substructuremulticalculator_header

#include <vector>
#include <map>
#include <string>
#include "xAODJet/Jet.h"
#include "fastjet/PseudoJet.hh"

class SubstructureMultiCalculator { 
  public:
    virtual std::map<std::string, double> result(const xAOD::Jet &jet) const;
    virtual std::map<std::string, double> result(std::vector<fastjet::PseudoJet> &constit_pseudojets,
        fastjet::PseudoJet jet_p4) const = 0;
};

#endif
