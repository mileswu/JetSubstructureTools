#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubstructureVariables/Validator.h"
#include <TFile.h>
#include <TH1.h>
#include "GaudiKernel/ITHistSvc.h"

using namespace std;

Validator::Validator(std::string name) : 
	AsgTool(name)
{
  declareProperty("InputContainer", m_InputContainer = "");
}

int Validator::execute() const
{
	// Load histogram service
	StatusCode sc;
	ITHistSvc *histSvc;
	sc = service("THistSvc", histSvc);
	if(sc.isFailure()) { 
		ATH_MSG_ERROR("Unable to access the THistSvc");
		return 1;
	}
	
	// Get leading jet
	const xAOD::JetContainer* jets = 0;
  if(evtStore()->contains<xAOD::JetContainer>(m_InputContainer) == false) {
    ATH_MSG_ERROR("Unable to retrieve jets from collection: " << m_InputContainer);
    return 1;
  }
	jets = evtStore()->retrieve<const xAOD::JetContainer>(m_InputContainer);
	if(jets->size() == 0) return 0;
	const xAOD::Jet *jet = jets->at(0); // This assumes the container is sorted

	// Loop over plots
	vector<std::string> plots;
	plots.push_back("Tau1");
	plots.push_back("Tau2");
	plots.push_back("Tau3");

	for(unsigned int i=0; i<plots.size(); i++) {
		TH1 *outputHist;
		if(histSvc->exists("/JetSubstructureMoments/" + plots[i])) {
			sc = histSvc->getHist("/JetSubstructureMoments/" + plots[i], outputHist);
			if(sc.isFailure()) { 
				ATH_MSG_ERROR("Unable to retrieve histogram");
				return 1;
			}
		}
		else {
			outputHist = new TH1F(plots[i].c_str(), "", 100, 0, 1);
			sc = histSvc->regHist("/JetSubstructureMoments/" + plots[i], outputHist);
			if(sc.isFailure()) {
				ATH_MSG_ERROR("Unable to register histogram");
				return 1;
			}
		}

		outputHist->Fill(jet->getAttribute<float>(plots[i].c_str()));
	}




	return 0;
}
