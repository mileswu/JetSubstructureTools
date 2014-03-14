#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/PlanarFlowTool.h"
#include "JetEDM/JetConstituentFiller.h"

#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

PlanarFlowTool::PlanarFlowTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  ATH_MSG_DEBUG("Initializing PlanarFlow tool.");
}

int PlanarFlowTool::modifyJet(xAOD::Jet &jet) const {
  double val = planarFlow(jet);
  ATH_MSG_VERBOSE("Adding jet PlanarFlow: " << val);
  jet.setAttribute("PlanarFlow", val);
  return 0;
}

double PlanarFlowTool::planarFlow(const xAOD::Jet &jet) const
{
  vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(jet);

  return planarFlow(constit_pseudojets, jet.p4());
}

double PlanarFlowTool::planarFlow(vector<fastjet::PseudoJet> &constit_pseudojets,
    TLorentzVector jet_p4) const
{
  double PF=-1.;

  TMatrixDSym MomentumTensor(2);
  //Planar flow
  double phi0=jet_p4.Phi();
  double eta0=jet_p4.Eta();

  double nvec[3];
  nvec[0]=(cos(phi0)/cosh(eta0));
  nvec[1]=(sin(phi0)/cosh(eta0));
  nvec[2]=tanh(eta0);

  //this is the rotation matrix
  double RotationMatrix[3][3];

  for(int i=0; i<3; i++) {
    for(int j=0; j<3; j++) {
      RotationMatrix[i][j] = 0.;
    }
  }

  double mag3 = sqrt(nvec[0]*nvec[0] + nvec[1]*nvec[1]+ nvec[2]*nvec[2]);
  double mag2 = sqrt(nvec[0]*nvec[0] + nvec[1]*nvec[1]);

  if(mag3 <= 0) {
    ATH_MSG_VERBOSE("Rotation axis is null");
    return PF;
  }

  double ctheta0 = nvec[2]/mag3;
  double stheta0 = mag2/mag3;
  double cphi0 = (mag2>0.) ? nvec[0]/mag2:0.;
  double sphi0 = (mag2>0.) ? nvec[1]/mag2:0.;

  RotationMatrix[0][0] =- ctheta0*cphi0;
  RotationMatrix[0][1] =- ctheta0*sphi0;
  RotationMatrix[0][2] = stheta0;
  RotationMatrix[1][0] = sphi0;
  RotationMatrix[1][1] =- 1.*cphi0;
  RotationMatrix[1][2] = 0.;
  RotationMatrix[2][0] = stheta0*cphi0;
  RotationMatrix[2][1] = stheta0*sphi0;
  RotationMatrix[2][2] = ctheta0;


  for(vector<PseudoJet>::iterator cit=constit_pseudojets.begin(); cit != constit_pseudojets.end();
      cit++) {
    const PseudoJet & cp = *cit;
    TLorentzVector p = TLorentzVector(cp.px(),cp.py(),cp.pz(),cp.e());
    double n=1./(cp.e()*jet_p4.M());
    double px_rot = RotationMatrix[0][0] * (p.Px())+RotationMatrix[0][1]
      * (p.Py())+RotationMatrix[0][2]*(p.Pz());
    double py_rot = RotationMatrix[1][0] * (p.Px())+RotationMatrix[1][1]
      * (p.Py())+RotationMatrix[1][2]*(p.Pz());
    double pz_rot = RotationMatrix[2][0] * (p.Px())+RotationMatrix[2][1]
      * (p.Py())+RotationMatrix[2][2]*(p.Pz());

    TLorentzVector* prot =new TLorentzVector();
    prot->SetPx(px_rot);
    prot->SetPy(py_rot);
    prot->SetPz(pz_rot);
    prot->SetE(p.E());

    MomentumTensor(0,0) += n * prot->Px() * prot->Px();
    MomentumTensor(0,1) += n * prot->Py() * prot->Px();
    MomentumTensor(1,0) += n * prot->Px() * prot->Py();
    MomentumTensor(1,1) += n * prot->Py() * prot->Py();
  }

  TMatrixDSymEigen eigen(MomentumTensor);
  TVectorD Lambda = eigen.GetEigenValues();
  PF = (4*Lambda[0]*Lambda[1]) / ((Lambda[0]+Lambda[1]) * (Lambda[0]+Lambda[1]));
  return PF;

}

