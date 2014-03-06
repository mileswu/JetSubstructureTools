#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/CenterOfMassShapesTool.h"
#include "JetEDM/JetConstituentFiller.h"

#include "JetSubStructureMomentTools/Moments/Thrust.h"

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
  map<const char*, double> res = t.result(jet);
  jet.setAttribute("ThrustMin", res["ThrustMin"]);
  jet.setAttribute("ThrustMaj", res["ThrustMaj"]);
  return 0;
}

