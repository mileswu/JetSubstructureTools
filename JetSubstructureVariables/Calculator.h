#ifndef jetsubstructurevariables_calculator_header
#define jetsubstructurevariables_calculator_header

#include "xAODCaloEvent/CaloCluster.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

#include "AsgExTool/AsgTool.h"
#include "AsgExToolInterface/IProcessTool.h"
#include <vector>

using namespace std;

namespace fastjet {
	class PseudoJet;
}

namespace JetSubstructureVariables {
	class Calculator :
		virtual public asgex::IProcessTool,
		virtual public asg::AsgTool {
			
			public:
				// Constructor and destructor
				Calculator();

				void setJetCollection(const char *jetCollectionName);

				int process() const;
				int modify(const xAOD::JetContainer &jets) const;
				int modify(const xAOD::Jet &jet) const;
				
				double nSubJettiness(const xAOD::Jet &jet, unsigned int nSubJets) const;
				double nSubJettiness(vector<fastjet::PseudoJet> &constit_pseudojets, unsigned int nSubJets) const;

			private:
				const char *m_jetCollectionName;
				
		};

}



#endif
