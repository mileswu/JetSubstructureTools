#ifndef jetsubstructuremoments_foxwolfram_header
#define jetsubstructuremoments_foxwolfram_header

#include <vector>
#include "xAODJet/Jet.h"
#include "fastjet/PseudoJet.hh"

using namespace std;

class FoxWolfram { 
  public:
    map<const char *, double> result(const xAOD::Jet &jet) const;
    map<const char *, double> result(vector<fastjet::PseudoJet> &constit_pseudojets,
        fastjet::PseudoJet jet_p4) const;
};

#endif
