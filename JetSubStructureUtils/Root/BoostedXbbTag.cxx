#include "JetSubStructureUtils/BoostedXbbTag.h"

#ifndef ROOTCORE
#include "PathResolver/PathResolver.h"
#endif

// root includes
#include <TSystem.h>
#include <TFile.h>
#include <TH2F.h>

// c++ includes
#include <iostream>
#include <sstream>
#include <fstream>

// rootcore includes
#include "AthContainers/ConstDataVector.h"
#include "AthLinks/ElementLink.h"
#include "xAODBase/IParticleContainer.h"

#define APP_NAME "BoostedXbbTag"

using namespace JetSubStructureUtils;

// make all static accessors static to this file, like extern but hip
SG::AuxElement::ConstAccessor<int>      BoostedXbbTag::AlgorithmType ("AlgorithmType");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::SizeParameter ("SizeParameter");
SG::AuxElement::ConstAccessor<int>      BoostedXbbTag::InputType ("InputType");
SG::AuxElement::ConstAccessor<int>      BoostedXbbTag::TransformType ("TransformType");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::RClus ("RClus");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::PtFrac ("PtFrac");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::RCut ("RCut");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::ZCut ("ZCut");
SG::AuxElement::ConstAccessor<char>     BoostedXbbTag::BDRS ("BDRS");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::YMin ("YMin");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::MuMax ("MuMax");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::D2 ("D2");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::ECF1 ("ECF1");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::ECF2 ("ECF2");
SG::AuxElement::ConstAccessor<float>    BoostedXbbTag::ECF3 ("ECF3");


BoostedXbbTag::BoostedXbbTag( std::string working_point,
                              float bTagCut,
                              float massCut,
                              float D2Cut,
                              const xAOD::MuonContainer* muons,
                              bool debug,
                              bool verbose) :
  m_working_point(working_point),
  m_bTagCut(bTagCut),
  m_massCut(massCut),
  m_D2Cut(D2Cut),
  m_muons(muons),
  m_debug(debug),
  m_verbose(verbose),
  m_bad_configuration(false)
{

  /* check configurations passed in, use m_bad_configuration as flag:
        - flag it to true if something is badly configured
        - otherwise, it should be false if everything seems ok
  */

  if(m_verbose)
    printf("<%s>: Attempting to configure with\r\n\t"
            "Working Point     %s\r\n\t"
            "Mass Cut          %0.2f\r\n\t"
            "D2 Cut            %0.2f\r\n\t"
            "Debug Output?     %s\r\n\t"
            "Verbose Output?   %s\r\n"
            "=========================================\r\n",
            APP_NAME, m_working_point.c_str(), m_massCut, m_D2Cut,
            m_debug?"Yes":"No", m_verbose?"Yes":"No");

  std::set<std::string> validWorkingPoints;
  validWorkingPoints.insert("tight");
  validWorkingPoints.insert("medium");
  validWorkingPoints.insert("loose");
  validWorkingPoints.insert("veryloose");

  if( validWorkingPoints.find(m_working_point) == validWorkingPoints.end()){
    printf("<%s>: Unknown working point requested.\r\n\tExpected: veryloose, loose, medium, tight\r\n\tGiven:    %s\r\n", APP_NAME, m_working_point.c_str());
    m_bad_configuration |= true;
  } else {
    if(m_verbose) printf("<%s>: Valid working point requested.\r\n", APP_NAME);
  }

  if(m_bad_configuration){
    std::cout << "|=====================================================|" << std::endl;
    std::cout << "|        WARNING        WARNING        WARNING        |" << std::endl;
    std::cout << "|        WARNING        WARNING        WARNING        |" << std::endl;
    std::cout << "|        WARNING        WARNING        WARNING        |" << std::endl;
    std::cout << "|        WARNING        WARNING        WARNING        |" << std::endl;
    std::cout << "|-----------------------------------------------------|" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "|         BoostedXbbTagger is misconfigured!          |" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "|=====================================================|" << std::endl;
  } else {
    if(m_verbose) printf("<%s>: BoostedXbbTagger is configured successfuly! Congratulations on such an achievement.\r\n", APP_NAME);
  }

}

std::pair<bool, std::string> BoostedXbbTag::get_algorithm_name(const xAOD::Jet& jet,
                                         const xAOD::JetAlgorithmType::ID jet_algorithm,
                                         const float size_parameter,
                                         const xAOD::JetInput::Type jet_input,
                                         const xAOD::JetTransform::Type jet_transform) const {
  bool error_flag(false);

  /* http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODJet/xAODJet/JetContainerInfo.h */
  static std::map<int, std::string> mapAlgorithmType = {
    {xAOD::JetAlgorithmType::kt_algorithm, "KT"},
    {xAOD::JetAlgorithmType::cambridge_algorithm, "CA"},
    {xAOD::JetAlgorithmType::antikt_algorithm, "AK"}
  };

  static std::map<int, std::string> mapInputType {
    {xAOD::JetInput::LCTopo, "LC"},
    {xAOD::JetInput::EMTopo, "EM"},
    {xAOD::JetInput::Truth, "TRU"},
    {xAOD::JetInput::Track, "TRA"},
    {xAOD::JetInput::PFlow, "PFL"}
  };

  static std::map<int, std::string> mapTransformType {
    {xAOD::JetTransform::Trim, "TRIM"},
    {xAOD::JetTransform::Prune, "PRUN"},
    {xAOD::JetTransform::MassDrop, "BDRS"}
  };

  // beginning of the algorithm_name
  std::string algorithm_name = mapAlgorithmType[jet_algorithm]
                              + std::to_string(static_cast<int>(size_parameter*10))
                              + mapInputType[jet_input]
                              + mapTransformType[jet_transform];


  // ending of algorithm_name
  switch(jet_transform){
    case xAOD::JetTransform::Trim:
      if( !PtFrac.isAvailable(jet) ){
        if(m_debug) printf("<%s>: PtFrac is not defined for the input jet.\r\n", APP_NAME);
        error_flag |= true;
      }
      if( !RClus.isAvailable(jet) ){
        if(m_debug) printf("<%s>: RClus is not defined for the input jet.\r\n" , APP_NAME);
        error_flag |= true;
      }

      if(m_verbose) printf("<%s>: PtFrac: %0.2f\tRClus: %0.2f\r\n", APP_NAME, PtFrac(jet), RClus(jet));
      algorithm_name += "F" + std::to_string(static_cast<int>(PtFrac(jet)*100))
                       + "R" + std::to_string(static_cast<int>(RClus(jet)*100));
    break;
    case xAOD::JetTransform::Prune:
      if( !RCut.isAvailable(jet) ){
        if(m_debug) printf("<%s>: RCut is not defined for the input jet.\r\n", APP_NAME);
        error_flag |= true;
      }
      if( !ZCut.isAvailable(jet) ){
        if(m_debug) printf("<%s>: ZCut is not defined for the input jet.\r\n", APP_NAME);
        error_flag |= true;
      }

      if(m_verbose) printf("<%s>: RCut: %0.2f\tZCut: %0.2f\r\n", APP_NAME, RCut(jet), ZCut(jet));
      algorithm_name += "R" + std::to_string(static_cast<int>(RCut(jet)*100))
                       + "Z" + std::to_string(static_cast<int>(ZCut(jet)*100));
    break;
    case xAOD::JetTransform::MassDrop:
      if( !MuMax.isAvailable(jet) ){
        if(m_debug) printf("<%s>: MuMax is not defined for the input jet.\r\n", APP_NAME);
        error_flag |= true;
      }
      if( !RClus.isAvailable(jet) ){
        if(m_debug) printf("<%s>: RClus is not defined for the input jet.\r\n", APP_NAME);
        error_flag |= true;
      }
      if( !YMin.isAvailable(jet) ){
        if(m_debug) printf("<%s>: YMin is not defined for the input jet.\r\n" , APP_NAME);
        error_flag |= true;
      }
      if( !BDRS.isAvailable(jet) ){
        if(m_debug) printf("<%s>: BDRS is not defined for the input jet.\r\n" , APP_NAME);
        error_flag |= true;
      }

      if(m_verbose) printf("<%s>: MuMax: %0.2f\tRClus: %0.2f\tYMin: %0.2f\r\n", APP_NAME, MuMax(jet), RClus(jet), YMin(jet));
      algorithm_name += "M" + std::to_string(static_cast<int>(MuMax(jet)*100))
                       + "R" + std::to_string(static_cast<int>(RClus(jet)*100))
                       + "Y" + std::to_string(static_cast<int>(YMin(jet)*100));
    break;
    default:
      if(m_debug) printf("<%s>: Current value of xAOD::JetTransform::Type is not supported!\r\n", APP_NAME);
      error_flag |= true;
    break;
  }

  return std::pair<bool, std::string>(!error_flag, algorithm_name);
}


int BoostedXbbTag::result(const xAOD::Jet& jet) const
{
  // bad configuration
  if(m_bad_configuration){
    if(m_debug) printf("<%s>: BoostedXbbTag has a bad configuration!", APP_NAME);
    return -1;
  }

  // if we call via this method, we need these 4 things defined
  if( !AlgorithmType.isAvailable(jet) ){
    if(m_debug) printf("<%s>: AlgorithmType is not defined for the jet.\r\n", APP_NAME);
    return -1;
  }
  if( !SizeParameter.isAvailable(jet) ){
    if(m_debug) printf("<%s>: SizeParameter is not defined for the jet.\r\n", APP_NAME);
    return -1;
  }
  if( !InputType.isAvailable(jet) )    {
    if(m_debug) printf("<%s>: InputType is not defined for the jet.\r\n"    , APP_NAME);
    return -1;
  }
  if( !TransformType.isAvailable(jet) ){
    if(m_debug) printf("<%s>: TransformType is not defined for the jet.\r\n", APP_NAME);
    return -1;
  }

  if(m_verbose) printf("<%s>: Jet has the 4 main properties set.\r\n\t"
      "AlgorithmType:  %d\r\n\t"
      "Size Parameter: %0.2f\r\n\t"
      "Input Type:     %d\r\n\t"
      "Transform Type: %d\r\n",
      APP_NAME, AlgorithmType(jet), SizeParameter(jet), InputType(jet), TransformType(jet));

  // get the algorithm name and check result
  std::pair<bool, std::string> res = get_algorithm_name(jet,
                                                        static_cast<xAOD::JetAlgorithmType::ID>(AlgorithmType(jet)),
                                                        SizeParameter(jet),
                                                        static_cast<xAOD::JetInput::Type>(InputType(jet)),
                                                        static_cast<xAOD::JetTransform::Type>(TransformType(jet)));

  // is it a valid result?
  if(!res.first){
    if(m_debug) printf("<%s>: Could not determine what jet you are using.\r\n", APP_NAME);
    return -1;
  } else {
    if(m_verbose) printf("<%s>: Jet introspection successful.\r\n", APP_NAME);
  }

  /* Steps:
      1. Get all AntiKt2TrackJets asssociated with the jet
      2. B-tag the two leading track-jets
      3. If both track-jets are b-tagged, match the muon (if any) to these b-tagged track-jets
        - if more than 1 muon matches a track jet (within the radius of the track jet), only use the muon closest in DR
      4. Correct the fat-jet mass by putting the matched muon back
      5. Set a cut on the corrected fat jet mass
      6. Cut on the D2 of the fat-jet (D2 from calorimeter constituents only)
  */

  // default to 1.0 if not found
  float jetRadius(SizeParameter(jet));

  // Step 1
  std::vector<const xAOD::Jet*> associated_trackJets;
  if(!jet->getAssociatedObjects<xAOD::Jet>("GhostAntiKt2TrackJet", associated_trackJets)){
    if(m_verbose) printf("<%s>: No associated track jets found.\r\n", APP_NAME);
    return -1;
  }
  if(associated_trackJets->size() < 2){
    if(m_verbose) printf("<%s>: We need at least two associated track jets.\r\n", APP_NAME);
    return -1;
  }

  // Step 2
  std::sort(associated_trackJets.begin(), associated_trackJets.end(), [](const xAOD::IParticle* lhs, const xAOD::IParticle* rhs) -> bool { return (lhs->pt() > rhs->pt()); });
  static SG::AuxElement::Decorator<int> isB("isB");
  for(int i=0; i<2; i++)
    isB(*(associated_trackJets.at(i))) = static_cast<int>(associated_trackJets.at(i)->btagging()->MV1_discriminant() > m_bTagCut);

  // Step 3
  if( ! (associated_trackJets.at(0)->getAttribute<int>("isB") && associated_trackJets.at(1)->getAttribute<int>("isB")) ){
    if(m_verbose) printf("<%s>: Both track jets are not b-tagged.\r\n", APP_NAME);
    return -1;
  }
  const xAOD::Muon* matched_muon(nullptr);
  for(int i=0; i<2; i++){
    float maxDR(0.2);
    associated_trackJets.at(i)->getAttribute("SizeParameter", maxDR);
    for(const auto muon: *m_muons){
      float DR( associated_trackJets.at(i)->p4().DeltaR(muon->p4()) );
      if(DR > maxDR) continue;
      maxDR = DR;
      matched_muon = muon;
    }
  }
  if(!matched_muon){
    if(m_verbose) printf("<%s>: There is no matched muon.\r\n", APP_NAME);
    return -1;
  }
  // super optimized version, need to know name of Muons collection
  // ElementLink< xAOD::IParticleContainer > el_muon( "Muons", matched_muon->index() );
  static SG::AuxElement::Decorator<ElementLink<xAOD::IParticleContainer> > matchedMuonLink("MatchedMuonLink");
  ElementLink<xAOD::IParticleContainer> el_muon( *m_muons, matched_muon->index() );
  matchedMuonLink(jet) = el_muon;

  // Step 4
  auto corrected_jet = jet.p4() + matched_muon->p4();

  // Step 5
  if(corrected_jet.M() < m_massCut){
    if(m_verbose) printf("<%s>: Corrected fat jet did not pass mass cut.\r\n", APP_NAME);
    return 0;
  } else {
    if(m_verbose) printf("<%s>: Corrected fat jet did pass mass cut.\r\n", APP_NAME);
  }

  // Step 6
  float d2(0.0);
  if(D2.isAvailable(jet)){
    d2 = D2(jet);
  } else {
    if((!ECF1.isAvailable(jet) || !ECF2.isAvailable(jet) || !ECF3.isAvailable(jet))){
      if(m_debug) printf("<%s>: D2 wasn't calculated. ECF# variables are not available.\r\n", APP_NAME);
      return -1;
    }
    d2 = ECF3(jet) * pow(ECF1(jet), 3.0) / pow(ECF2(jet), 3.0);
  }
  if(d2 > m_D2Cut){
    if(m_verbose) printf("<%s>: Jet did not pass the D2 cut.\r\n", APP_NAME);
    return 0;
  } else {
    if(m_verbose) printf("<%s>: Jet did pass the D2 cut.\r\n", APP_NAME);
  }

  return 1;

}
