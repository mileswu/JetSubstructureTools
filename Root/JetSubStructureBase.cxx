#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructure/JetSubStructureBase.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

JetSubStructureBase::JetSubStructureBase(std::string name) : 
  JetModifierBase(name)
{
}

