#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubstructureVariables/Calculator.h"

#ifndef XAOD_STANDALONE
#include "JetAnalysisEDM/JetDualUseEDMInclude.h"
#endif

#include "fastjet/ClusterSequence.hh"

using namespace std;

using JetSubstructureVariables::Calculator;
using JetAnalysisEDM::ParticleContainer;
using JetAnalysisEDM::Particle;
using JetAnalysisEDM::LorentzVector_t;

using fastjet::PseudoJet;

Calculator::Calculator() : 
	m_jetCollectionName("")
{
	
}

void Calculator::setJetCollection(const char *jetCollectionName)
{
	m_jetCollectionName = jetCollectionName;
}

int Calculator::process() const {
	const JetCollection* jets = 0;
	const CaloClusterContainer* clusters = 0;
	StatusCode status;

	//Tau2
	status = evtStore()->retrieve(jets, m_jetCollectionName);
	if(status.isFailure() || jets == 0) {
		ATH_MSG_ERROR("Unable to retrieve jets from collection: " << m_jetCollectionName);
		return 1;
	}

	status = evtStore()->retrieve(clusters, "cl");
	if(status.isFailure() || clusters == 0) {
		ATH_MSG_ERROR("Unable to retrieve clusters");
		return 1;
	}
	cout << "Number of clusters: " << clusters->size() << endl;
	jets->declareConstituents(clusters, "constit_index");

	cout << "Number of jets: " << jets->size() << endl;
	for(size_t iJ=0; iJ < jets->size(); iJ++) {
		cout << "Jet const pt = " << jets->at(iJ)->get<float>("constscale_pt") << endl;
		ParticleContainer constits;
		jets->at(iJ)->fillConstituents(constits);

		cout << "My tau1: " << nSubJettiness(constits, 1) << endl;
		cout << "Ntuple tau1: " << jets->at(iJ)->get<float>("Tau1") << endl;
		cout << "My tau2: " << nSubJettiness(constits, 2) << endl;
		cout << "Ntuple tau2: " << jets->at(iJ)->get<float>("Tau2") << endl;
		cout << "My tau25: " << nSubJettiness(constits, 25) << endl;
	}

	return 0;
}

double Calculator::nSubJettiness(ParticleContainer &constituents, unsigned int nSubJets) const
{
	if(nSubJets == 0 || nSubJets > constituents.size()) {
		ATH_MSG_ERROR("You asked for an invalid number of subjets in nSubJettiness()");
		return 0.0;
	}

	vector<fastjet::PseudoJet> constit_pseudojets;
	for(size_t iC=0; iC < constituents.size(); iC++) {
		Particle *constit = constituents[iC];
		constit_pseudojets.push_back(fastjet::PseudoJet(constit->Px(), constit->Py(), constit->Pz(), constit->E()));
	}

	double jet_rad=1.2;
	double alpha=1;

	fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm,M_PI/2,fastjet::E_scheme,fastjet::Best);
	fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);
	vector<fastjet::PseudoJet> kt_subjets = kt_clust_seq.exclusive_jets((int)nSubJets);

	if(kt_subjets.size() != nSubJets) {
		ATH_MSG_ERROR("For some reason we did not find enough subjets");
		return 0.0;
	}

	double tauNum = 0.0, tauDen = 0.0;

	for(size_t iC=0; iC < constit_pseudojets.size(); iC++) {
		
		double minDR = DBL_MAX;
		
		for(size_t iJ=0; iJ < kt_subjets.size(); iJ++) {
			double dr = sqrt(constit_pseudojets[iC].squared_distance(kt_subjets[iJ]));
			if(dr < minDR)
				minDR = dr;
		}

		tauNum += constit_pseudojets[iC].pt() * pow(minDR, alpha);
		tauDen += constit_pseudojets[iC].pt() * pow(jet_rad, alpha);
	}

	return tauNum/tauDen;
}
