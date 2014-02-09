#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructure/AngularityTool.h"
#include "JetEDM/JetConstituentFiller.h"
#include "CLHEP/Vector/TwoVector.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

AngularityTool::AngularityTool(std::string name) : 
  JetSubStructureBase(name)
{
}

int AngularityTool::modifyJet(xAOD::Jet &jet) const {
  jet.setAttribute("Angularity", angularity(jet));

  return 0;
}

double AngularityTool::angularity(const xAOD::Jet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);

  TLorentzVector jet_p4;
  jet_p4.SetPxPyPzE(jet.px(), jet.py(), jet.pz(), jet.e());

  return angularity(constit_pseudojets, jet_p4);
}

double AngularityTool::angularity(vector<fastjet::PseudoJet> &constit_pseudojets, TLorentzVector jet_p4) const
{
  double Angularity2=-1.;
  const double m_a2=-2.;
  double sum_a2=0.;

  for(unsigned int iConstit=0; iConstit < constit_pseudojets.size(); iConstit++) {
    TLorentzVector tclus = TLorentzVector(constit_pseudojets[iConstit].px(),constit_pseudojets[iConstit].py(),constit_pseudojets[iConstit].pz(),constit_pseudojets[iConstit].e());
    double theta_i = jet_p4.Angle(tclus.Vect());
    double e_theta_i_a2 = constit_pseudojets[iConstit].E()*pow(sin(theta_i),m_a2)*pow(1-cos(theta_i),1-m_a2);
    sum_a2 += e_theta_i_a2;
  }

  Angularity2 = sum_a2/jet_p4.M(); 
  return Angularity2;
}

