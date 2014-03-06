#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/CenterOfMassShapesTool.h"
#include "JetEDM/JetConstituentFiller.h"

#include "JetSubStructureMomentTools/Moments/Thrust.h"
#include "JetSubStructureMomentTools/Moments/FoxWolfram.h"
#include "JetSubStructureMomentTools/Moments/SphericityTensor.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

CenterOfMassShapesTool::CenterOfMassShapesTool(std::string name) : 
  JetSubStructureMomentToolsBase(name)
{
  ATH_MSG_DEBUG("Initializing CenterOfMassShapes tool.");
}

int CenterOfMassShapesTool::modifyJet(xAOD::Jet &jet) const {
  Thrust t;
  FoxWolfram foxwolfram;
  SphericityTensor sphericity;
  map<const char*, double> res;
  
  res = t.result(jet);
  jet.setAttribute("ThrustMin", res["ThrustMin"]);
  jet.setAttribute("ThrustMaj", res["ThrustMaj"]);

  res = foxwolfram.result(jet);
  jet.setAttribute("FoxWolfram0", res["FoxWolfram0"]);
  jet.setAttribute("FoxWolfram1", res["FoxWolfram1"]);
  jet.setAttribute("FoxWolfram2", res["FoxWolfram2"]);
  jet.setAttribute("FoxWolfram3", res["FoxWolfram3"]);
  jet.setAttribute("FoxWolfram4", res["FoxWolfram4"]);

  res = sphericity.result(jet);
  jet.setAttribute("Sphericity", res["Sphericity"]);
  jet.setAttribute("Aplanarity", res["Aplanarity"]);

  return 0;
}

