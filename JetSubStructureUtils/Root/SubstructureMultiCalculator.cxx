#include "JetSubStructureUtils/SubstructureMultiCalculator.h"
#include "JetEDM/JetConstituentFiller.h"

using namespace std;

map<string, double> SubstructureMultiCalculator::result(const xAOD::Jet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets =
    jet::JetConstituentFiller::constituentPseudoJets(jet);
  fastjet::PseudoJet jet_p4(jet.px(), jet.py(), jet.pz(), jet.e());

  return result(constit_pseudojets, jet_p4);
}
