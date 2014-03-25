#ifndef jetsubstructuremoments_thrust_header
#define jetsubstructuremoments_thrust_header

#include <vector>
#include "xAODJet/Jet.h"
#include "fastjet/PseudoJet.hh"

using namespace std;

class Thrust { 
  public:
    map<string, double> result(const xAOD::Jet &jet) const;
    map<string, double> result(vector<fastjet::PseudoJet> &constit_pseudojets,
        fastjet::PseudoJet jet_p4) const;
};

#endif
