#include <iostream>
#include <TFile.h>
#include "JetSubstructureVariables/Calculator.h"
#include "JetAnalysisEDM/EventManager.h"

using namespace std;

int main(int argc, char **argv) {
	if(argc != 2) {
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return(1);
	}

	TFile *f = new TFile(argv[1]);
	TTree *tree = (TTree *)f->Get("physics");

	if(!tree) {
		cerr << "Could not find TTree at physics/" << endl;
		return(1);
	}

	JetSubstructureVariables::Calculator *calc = new JetSubstructureVariables::Calculator();
	calc->setJetCollection("jet_CamKt12LCTopoSplitFilteredmassFraction67minSplitR0");
	JetAnalysisEDM::EventManager event(tree);
	asg::AsgTool::event(&event, true);

	event.setEntry(tree->LoadTree(0));
	calc->process();

	return(0);
}
