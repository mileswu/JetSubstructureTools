#include "JetSubStructureUtils/SphericityTensor.h"
#include "JetEDM/JetConstituentFiller.h"

#include "TMatrixD.h"
#include "TDecompSVD.h"
 

map<const char *, double> SphericityTensor::result(const xAOD::Jet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);
  fastjet::PseudoJet jet_p4(jet.px(), jet.py(), jet.pz(), jet.e());

  return result(constit_pseudojets, jet_p4);
}

map<const char *, double> SphericityTensor::result(vector<fastjet::PseudoJet> &constit_pseudojets,
        fastjet::PseudoJet jet_p4) const
{
  map<const char *, double> Variables;

  Variables["Sphericity"] = -999.*1000.;
  Variables["Aplanarity"] = -999.*1000.;

  if ( constit_pseudojets.size() < 2 ) return Variables;

  double bx = jet_p4.px()/jet_p4.e();
  double by = jet_p4.py()/jet_p4.e();
  double bz = jet_p4.pz()/jet_p4.e();

  std::vector<fastjet::PseudoJet> clusters;

  for(unsigned int i1=0; i1 < constit_pseudojets.size(); i1++) {
    TLorentzVector v;
    v.SetPxPyPzE(constit_pseudojets.at(i1).px(), constit_pseudojets.at(i1).py(),constit_pseudojets.at(i1).pz(),constit_pseudojets.at(i1).e());
    v.Boost(-bx,-by,-bz);
    fastjet::PseudoJet v2(v.Px(), v.Py(), v.Pz(), v.E());
    clusters.push_back(v2);
  }

  TMatrixD MomentumTensor(3,3);
  double P2Sum = 0;

  for(std::vector<fastjet::PseudoJet>::const_iterator Itr=clusters.begin(); Itr!=clusters.end(); Itr++) {
    MomentumTensor(0,0) += (*Itr).px()*(*Itr).px();
    MomentumTensor(0,1) += (*Itr).px()*(*Itr).py();
    MomentumTensor(0,2) += (*Itr).px()*(*Itr).pz();
    MomentumTensor(1,0) += (*Itr).py()*(*Itr).px();
    MomentumTensor(1,1) += (*Itr).py()*(*Itr).py();
    MomentumTensor(1,2) += (*Itr).py()*(*Itr).pz();
    MomentumTensor(2,0) += (*Itr).pz()*(*Itr).px();
    MomentumTensor(2,1) += (*Itr).pz()*(*Itr).py();
    MomentumTensor(2,2) += (*Itr).pz()*(*Itr).pz();

    P2Sum += (*Itr).px()*(*Itr).px()+(*Itr).py()*(*Itr).py()+(*Itr).pz()*(*Itr).pz();
  }

  double Aplanarity = -1;
  double Sphericity = -1;

  if(P2Sum > 0) {
    for(int i=0; i<3; i++) {
      for(int j=0; j<3; j++) {
        MomentumTensor(i,j) /= P2Sum;
      }
    }

    TDecompSVD * aSVD = new TDecompSVD(MomentumTensor);
    TVectorD Lambda = aSVD->GetSig();

    Aplanarity = 1.5*Lambda[2];
    Sphericity = 1.5*(Lambda[1]+Lambda[2]);

    delete aSVD;
  }

  Variables["Aplanarity"] = Aplanarity;
  Variables["Sphericity"] = Sphericity;

  return Variables;  
}
