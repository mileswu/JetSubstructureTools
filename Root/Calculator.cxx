#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubstructureVariables/Calculator.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;

//using JetSubstructureVariables::Calculator;
using fastjet::PseudoJet;

Calculator::Calculator(std::string name) : 
	AsgTool(name),
	m_jetCollectionName("AntiKt4LCTopoJets")
{
	
}

void Calculator::setJetCollection(const char *jetCollectionName)
{
	m_jetCollectionName = jetCollectionName;
}

int Calculator::execute() const {
	const xAOD::JetContainer* jets = 0;
	jets = evtStore()->retrieve<const xAOD::JetContainer>(m_jetCollectionName);
	if(jets == 0) {
		ATH_MSG_ERROR("Unable to retrieve jets from collection: " << m_jetCollectionName);
		return 1;
	}

	const xAOD::JetContainer *clusters = 0;
	clusters = evtStore()->retrieve<const xAOD::JetContainer>("JetClusters");
	if(clusters == 0) {
		ATH_MSG_ERROR("Unable to retrieve jets from collection: " << "JetClusters");
		return 1;
	}

	return modify(*jets);
}

int Calculator::modify(const xAOD::JetContainer &jets) const {
	int retval = 0;

	cout << "Number of jets: " << jets.size() << endl;

	for(size_t iJ=0; iJ < jets.size(); iJ++) {
		retval |= modify(*jets[iJ]);
	}

	return retval;
}

int Calculator::modify(const xAOD::Jet &jet) const {
	cout << "Jet pt = " << jet.pt() << endl;
	//cout << "Jet const pt = " << jets->at(iJ)->jetP4(xAOD::P4SignalState::JETCONSTITUENTSCALE).pt() << endl;
	cout << "Jet num const = " << jet.numConstituents() << endl;


	/*CaloClusterContainer constits;
	//CaloClusterContainer &constits = *jets->at(iJ)->getContainer(jets->at(iJ)->begin());
	//jets->at(iJ)->fillConstituents(constits);

	cout << "My tau1: " << nSubJettiness(constits, 1) << endl;
	cout << "Ntuple tau1: " << jets->at(iJ)->get<float>("Tau1") << endl;
	cout << "My tau2: " << nSubJettiness(constits, 2) << endl;
	cout << "Ntuple tau2: " << jets->at(iJ)->get<float>("Tau2") << endl;
	cout << "My tau25: " << nSubJettiness(constits, 25) << endl;*/
	cout << "tau1: " << nSubJettiness(jet, 1) << endl;
	return 0;
}

double Calculator::nSubJettiness(const xAOD::Jet &jet, unsigned int nSubJets) const {
	xAOD::JetConstituentVector constit = jet.getConstituents();
	vector<fastjet::PseudoJet> constit_pseudojets;
	/*for(int i=0; i<constit.size(); i++) {
		cout << "m=" << constit[i].m() << endl;
	}*/
	for(xAOD::JetConstituentVector::iterator it=constit.begin(); it!=constit.end(); it++) {
		constit_pseudojets.push_back(fastjet::PseudoJet(it->px(), it->py(), it->pz(), it->e()));
	}

	return nSubJettiness(constit_pseudojets, nSubJets);
}

double Calculator::nSubJettiness(vector<fastjet::PseudoJet> &constit_pseudojets, unsigned int nSubJets) const
{
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
	//cout << "kt subjet rap=" << kt_subjets[0].rap() << "phi=" << kt_subjets[0].phi() << endl;

	for(size_t iC=0; iC < constit_pseudojets.size(); iC++) {
		
		double minDR = DBL_MAX;
		
		for(size_t iJ=0; iJ < kt_subjets.size(); iJ++) {
			double dr = sqrt(constit_pseudojets[iC].squared_distance(kt_subjets[iJ]));
			if(dr < minDR)
				minDR = dr;
		}

		//cout << "constit pt=" << constit_pseudojets[iC].pt() << " y=" << constit_pseudojets[iC].rap() << " phi=" << constit_pseudojets[iC].phi() << endl;
		//cout << "dr = " << minDR << endl;

		tauNum += constit_pseudojets[iC].pt() * pow(minDR, alpha);
		tauDen += constit_pseudojets[iC].pt() * pow(jet_rad, alpha);
	}

	return tauNum/tauDen;
}
