#ifndef jetsubstructurevariables_calculator_header
#define jetsubstructurevariables_calculator_header

#include "AsgExTool/AsgTool.h"
#include "AsgExToolInterface/IProcessTool.h"

namespace JetSubstructureVariables {
	class Calculator :
		virtual public asgex::IProcessTool,
		virtual public asg::AsgTool {
			
			public:
				// Constructor and destructor
				Calculator();

				void setJetCollection(const char *jetCollectionName);

				int process() const;
				double nSubJettiness(JetAnalysisEDM::ParticleContainer &constituents, unsigned int nSubJets) const;

			private:
				const char *m_jetCollectionName;
				
		};

}



#endif
