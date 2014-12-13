#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructureMomentTools/SubjetFinderTool.h"
#include "JetEDM/JetConstituentFiller.h"
#include "JetEDM/FastJetUtils.h"
#include "JetSubStructureUtils/SubjetFinder.h"
#include "fastjet/ClusterSequence.hh"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"

using namespace std;
using fastjet::PseudoJet;

SubjetFinderTool::SubjetFinderTool(std::string name) : 
  JetSubStructureMomentToolsBase(name), m_subjetlabel(""), m_subjetcontainername("")
{
  declareProperty("JetAlgorithm", m_jetalg = "Kt");
  declareProperty("JetRadius", m_jetrad = 0.3);
  declareProperty("PtMin", m_ptmin = 5000);
  declareProperty("SubjetLabel", m_subjetlabel);
  declareProperty("SubjetContainerName", m_subjetcontainername);
}

int SubjetFinderTool::modifyJet(xAOD::Jet &jet) const {
  // Set up jet algorithm
  xAOD::JetAlgorithmType::ID ialg = xAOD::JetAlgorithmType::algId(m_jetalg);
  fastjet::JetAlgorithm fjalg = xAOD::JetAlgorithmType::fastJetDef(ialg);

  // Run it
  JetSubStructureUtils::SubjetFinder subjetFinder(fjalg, m_jetrad, m_ptmin);
  vector<fastjet::PseudoJet> subjets = subjetFinder.result(jet);

  // Retrieve or set up subjet container
  string subjet_name = m_subjetlabel;
  string subjet_container_name = m_subjetcontainername;
  if(subjet_name == "" || subjet_container_name == "") {
    ATH_MSG_ERROR("Invalid subjet label or container name");
    return 1;
  }
  xAOD::JetContainer *subjet_container = 0;
  subjet_container = evtStore()->tryRetrieve<xAOD::JetContainer>(subjet_container_name);
  if(subjet_container == 0) {
    StatusCode sc;
    subjet_container = new xAOD::JetContainer;
    subjet_container->setStore(new xAOD::JetAuxContainer);
    sc = evtStore()->record(subjet_container, subjet_container_name);
    if(sc.isFailure()) {
      ATH_MSG_ERROR("Error recording subjet container (" << subjet_container_name << ")");
      return 1;
    }
    sc = evtStore()->record(dynamic_cast<xAOD::JetAuxContainer*>(subjet_container->getStore()), subjet_container_name + "Aux.");
    if(sc.isFailure()) {
      ATH_MSG_ERROR("Error recording subjet aux container (" << subjet_container_name << "Aux.)");
      return 1;
    }
  }

  // Create xAOD::Jet's
  vector<const xAOD::Jet *> subj_ptrs;
  for(auto it = subjets.begin(); it != subjets.end(); it++) {
    xAOD::Jet *subj = new xAOD::Jet();
    subjet_container->push_back(subj);
    subj_ptrs.push_back(subj);
    
    // Set 4-mom
    subj->setJetP4(xAOD::JetFourMom_t(it->pt(), it->eta(), it->phi(), it->m()));
    subj->setJetP4(xAOD::JetConstitScaleMomentum, subj->jetP4());

    // Set constituents
    jet::JetConstituentFiller confiller;
    confiller.extractConstituents(*subj, 0, &(*it));

    // Set association to parent
    const xAOD::JetContainer *parent_container = dynamic_cast<const xAOD::JetContainer*>(jet.container());
    ElementLink<xAOD::JetContainer> el_parent(*parent_container, jet.index());
    subj->setAttribute("Parent", el_parent);
  }
  
  // Set association to subjets
  jet.setAssociatedObjects(subjet_name, subj_ptrs);

  return 0;
}
