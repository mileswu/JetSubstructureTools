#ifndef jetsubstructurevariables_calculator_header
#define jetsubstructurevariables_calculator_header

#include "xAODCaloEvent/CaloCluster.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

#include "AsgTools/AsgTool.h"
#include "JetInterface/IJetExecuteTool.h"
#include <vector>

using namespace std;

namespace fastjet {
	class PseudoJet;
}

//namespace JetSubstructureVariables {
	class Calculator :
		virtual public IJetExecuteTool,
		virtual public asg::AsgTool {
			ASG_TOOL_CLASS(Calculator, IJetExecuteTool)
			
			public:
				// Constructor and destructor
				Calculator(std::string name);

				void setJetCollection(const char *jetCollectionName);

				int execute() const;
				int modify(const xAOD::JetContainer &jets) const;
				int modify(const xAOD::Jet &jet) const;
				
				double nSubJettiness(const xAOD::Jet &jet, unsigned int nSubJets) const;
				double nSubJettiness(vector<fastjet::PseudoJet> &constit_pseudojets, unsigned int nSubJets) const;

			private:
				const char *m_jetCollectionName;
				
		};

//}



#endif
