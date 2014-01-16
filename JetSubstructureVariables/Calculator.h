#ifndef jetsubstructurevariables_calculator_header
#define jetsubstructurevariables_calculator_header

#include "AsgExTool/AsgTool.h"
#include "AsgExToolInterface/IProcessTool.h"
#include "JetAnalysisEDM/JetDualUseEDMInclude.h"
#include <vector>

using namespace std;

namespace fastjet {
	class PsuedoJet;
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
				double nSubJettiness(CaloClusterContainer &constituents, unsigned int nSubJets) const;
				double nSubJettiness(vector<fastjet::PseudoJet> constit_pseudojets, unsigned int nSubJets) const;

			private:
				const char *m_jetCollectionName;
				
		};

}



#endif
