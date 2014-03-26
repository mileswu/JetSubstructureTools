#include "JetSubStructureUtils/Angularity.h"
#include "TLorentzVector.h"

using namespace std;

double Angularity::result(const fastjet::PseudoJet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet.constituents();
  TLorentzVector jet_p4(jet.px(), jet.py(), jet.pz(), jet.e());

  double Angularity2=-1.;
  const double m_a2=-2.;
  double sum_a2=0.;

  for(unsigned int iConstit=0; iConstit < constit_pseudojets.size(); iConstit++) {
    TLorentzVector tclus = TLorentzVector(constit_pseudojets[iConstit].px(),constit_pseudojets[iConstit].py(),constit_pseudojets[iConstit].pz(),constit_pseudojets[iConstit].e());
    double theta_i = jet_p4.Angle(tclus.Vect());
    double sintheta_i = sin(theta_i);
    if( sintheta_i == 0 ) continue; // avoid FPE
    double e_theta_i_a2 = constit_pseudojets[iConstit].E()*pow(sintheta_i,m_a2)*pow(1-cos(theta_i),1-m_a2);
    sum_a2 += e_theta_i_a2;
  }

  Angularity2 = sum_a2/jet.m(); 
  return Angularity2;
}
