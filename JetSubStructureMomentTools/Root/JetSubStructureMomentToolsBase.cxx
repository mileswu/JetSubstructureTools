#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

JetSubStructureMomentToolsBase::JetSubStructureMomentToolsBase(std::string name) : 
  JetModifierBase(name)
{
}

