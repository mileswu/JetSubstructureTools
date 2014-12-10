#include "JetSubStructureUtils/SubjetFinder.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/JetDefinition.hh"
#include <iostream>

using namespace std;
using namespace JetSubStructureUtils;

SubjetFinder::SubjetFinder()
{
}

vector<fastjet::PseudoJet> SubjetFinder::result(const fastjet::PseudoJet &jet) const
{
	vector<fastjet::PseudoJet> constit_pseudojets = jet.constituents();
	vector<fastjet::PseudoJet> subjets;
	fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm, 0.3,
			fastjet::E_scheme, fastjet::Best);
	fastjet::ClusterSequence clust_seq(constit_pseudojets, jet_def);
	subjets = fastjet::sorted_by_pt(clust_seq.inclusive_jets(5000.0));
	return subjets;
}
