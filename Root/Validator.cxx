#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructure/Validator.h"
#include <TFile.h>
#include <TH1.h>
#include "GaudiKernel/ITHistSvc.h"

using namespace std;

Validator::Validator(std::string name) : 
	AsgTool(name)
{
  declareProperty("InputContainer", m_InputContainer = "");
  declareProperty("FloatMoments", m_FloatMoments);
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

	// Loop over float moments
	for(unsigned int i=0; i<m_FloatMoments.size(); i++) {
		TH1 *outputHist;
		if(histSvc->exists("/JetSubstructureMoments/" + m_FloatMoments[i])) {
			sc = histSvc->getHist("/JetSubstructureMoments/" + m_FloatMoments[i], outputHist);
			if(sc.isFailure()) { 
				ATH_MSG_ERROR("Unable to retrieve histogram");
				return 1;
			}
		}
		else {
      unsigned int nbins = 100;
      float xlow = 0, xhigh = 1.0;
      bool isMomentNormalized = true;
      if(m_FloatMoments[i].find("Split") != string::npos) {
        isMomentNormalized = false;
      }

      if(isMomentNormalized == false) {
        nbins = 1000;
        xhigh = 2000000;
      }

			outputHist = new TH1F(m_FloatMoments[i].c_str(), "", nbins, xlow, xhigh);
			sc = histSvc->regHist("/JetSubstructureMoments/" + m_FloatMoments[i], outputHist);
			if(sc.isFailure()) {
				ATH_MSG_ERROR("Unable to register histogram");
				return 1;
			}
		}

		outputHist->Fill(jet->getAttribute<float>(m_FloatMoments[i].c_str()));
	}

	return 0;
}