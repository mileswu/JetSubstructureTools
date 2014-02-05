#ifndef jetsubstructure_jetsubstructurebase_header
#define jetsubstructure_jetsubstructurebase_header

#include "xAODCaloEvent/CaloCluster.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

#include "AsgTools/AsgTool.h"
#include "JetInterface/IJetModifier.h"
#include <vector>

using namespace std;

namespace fastjet {
  class PseudoJet;
}

class JetSubStructureBase :
  virtual public IJetModifier {
    
    public:
      int modify(xAOD::JetContainer &jets) const;
      virtual int modify(xAOD::Jet &jet) const = 0;

      vector<fastjet::PseudoJet> getConstituentPseudoJets(const xAOD::Jet &jet) const;
};


#endif
