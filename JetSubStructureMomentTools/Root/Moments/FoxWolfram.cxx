#include "JetSubStructureMomentTools/Moments/FoxWolfram.h"
#include "JetEDM/JetConstituentFiller.h"

map<const char *, double> FoxWolfram::result(const xAOD::Jet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets =
    jet::JetConstituentFiller::constituentPseudoJets(jet);
  fastjet::PseudoJet jet_p4(jet.px(), jet.py(), jet.pz(), jet.e());

  return result(constit_pseudojets, jet_p4);
}

map<const char *, double> FoxWolfram::result(vector<fastjet::PseudoJet> &constit_pseudojets,
        fastjet::PseudoJet jet_p4) const
{
  map<const char *, double> Variables;

  double bx = jet_p4.px()/jet_p4.e();
  double by = jet_p4.py()/jet_p4.e();
  double bz = jet_p4.pz()/jet_p4.e();

  Variables["FoxWolfram0"] =  -999.*1000.;
  Variables["FoxWolfram1"] =  -999.*1000.;
  Variables["FoxWolfram2"] =  -999.*1000.;
  Variables["FoxWolfram3"] =  -999.*1000.;
  Variables["FoxWolfram4"] =  -999.*1000.;

  double FoxWolframMoments[5] = {0};

  double ESum = 0;
  std::vector<fastjet::PseudoJet> clusters;

  for(unsigned int i1=0; i1 < constit_pseudojets.size(); i1++) {
    TLorentzVector v;
    v.SetPxPyPzE(constit_pseudojets.at(i1).px(), constit_pseudojets.at(i1).py(),constit_pseudojets.at(i1).pz(),constit_pseudojets.at(i1).e());
    v.Boost(-bx,-by,-bz);
    fastjet::PseudoJet v2(v.Px(), v.Py(), v.Pz(), v.E());
    clusters.push_back(v2);
  }

  for(unsigned int i1=0; i1<clusters.size(); i1++) {
    double p1 = sqrt(clusters.at(i1).px()*clusters.at(i1).px()
        +      clusters.at(i1).py()*clusters.at(i1).py()
        +      clusters.at(i1).pz()*clusters.at(i1).pz());

    for(unsigned int i2=i1+1; i2<clusters.size(); i2++) {
      double p2 = sqrt(clusters.at(i2).px()*clusters.at(i2).px()
          +      clusters.at(i2).py()*clusters.at(i2).py()
          +      clusters.at(i2).pz()*clusters.at(i2).pz());

      TVector3 cj(clusters[i2].px(), clusters[i2].py(), clusters[i2].pz());
      TLorentzVector quadvec(clusters[i1].px(), clusters[i1].py(), clusters[i1].pz(),  clusters[i1].e());

      double CosTheta12 = TMath::Cos(quadvec.Angle(cj)); 

      double P0 = 1.;
      double P1 = CosTheta12;
      double P2 = 0.5*(3.*CosTheta12*CosTheta12 - 1.);
      double P3 = 0.5*(5.*CosTheta12*CosTheta12*CosTheta12 - 3.*CosTheta12);
      double P4 = 0.125*(35.*CosTheta12*CosTheta12*CosTheta12*CosTheta12 - 30.*CosTheta12*CosTheta12 + 3.);

      FoxWolframMoments[0] += p1*p2*P0;
      FoxWolframMoments[1] += p1*p2*P1;
      FoxWolframMoments[2] += p1*p2*P2;
      FoxWolframMoments[3] += p1*p2*P3;
      FoxWolframMoments[4] += p1*p2*P4;
    }

    ESum += clusters[i1].e();
  }

  vector<double> R;

  if(ESum > 0) {
    for(int i=0; i<5; i++) {
      FoxWolframMoments[i] /= ESum*ESum;
      R.push_back(FoxWolframMoments[i]);
    }
  }

  if(R.size() == 0) return Variables;

  Variables["FoxWolfram0"] = R.at(0);
  Variables["FoxWolfram1"] = R.at(1);
  Variables["FoxWolfram2"] = R.at(2);
  Variables["FoxWolfram3"] = R.at(3);
  Variables["FoxWolfram4"] = R.at(4);

  return Variables; 
}
