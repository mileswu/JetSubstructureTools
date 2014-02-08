#ifndef jetsubstructure_jetsubstructurebase_header
#define jetsubstructure_jetsubstructurebase_header

#include "xAODCaloEvent/CaloCluster.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

#include "JetRec/JetModifierBase.h"
#include <vector>

using namespace std;

namespace fastjet {
  class PseudoJet;
}

class JetSubStructureBase :
  public JetModifierBase {
    public:
      // Constructor and destructor
      JetSubStructureBase(std::string name);
};


#endif
