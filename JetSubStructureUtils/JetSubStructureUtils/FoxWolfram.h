#ifndef jetsubstructuremoments_foxwolfram_header
#define jetsubstructuremoments_foxwolfram_header

#include <vector>
#include "xAODJet/Jet.h"
#include "fastjet/PseudoJet.hh"

using namespace std;

class FoxWolfram { 
  public:
    map<string, double> result(const xAOD::Jet &jet) const;
    map<string, double> result(vector<fastjet::PseudoJet> &constit_pseudojets,
        fastjet::PseudoJet jet_p4) const;
};

#endif
