#include "JetSubStructureUtils/BoostedXbbTag.h"
#include <iostream>

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

BoostedXbbTag::BoostedXbbTag( std::string working_point,
                              bool debug,
                              bool verbose) :
  m_working_point(working_point),
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
            "Debug Output?     %s\r\n\t"
            "Verbose Output?   %s\r\n"
            "=========================================\r\n",
            APP_NAME, m_working_point.c_str(),
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


int BoostedXbbTag::result(const xAOD::Jet& jet, const xAOD::MuonContainer* muons) const
{
  // bad configuration
  if(m_bad_configuration){
    if(m_debug) printf("<%s>: BoostedXbbTag has a bad configuration!", APP_NAME);
    return 0;
  }

  // if we call via this method, we need these 4 things defined
  if( !AlgorithmType.isAvailable(jet) ){
    if(m_debug) printf("<%s>: AlgorithmType is not defined for the jet.\r\n", APP_NAME);
    return 0;
  }
  if( !SizeParameter.isAvailable(jet) ){
    if(m_debug) printf("<%s>: SizeParameter is not defined for the jet.\r\n", APP_NAME);
    return 0;
  }
  if( !InputType.isAvailable(jet) )    {
    if(m_debug) printf("<%s>: InputType is not defined for the jet.\r\n"    , APP_NAME);
    return 0;
  }
  if( !TransformType.isAvailable(jet) ){
    if(m_debug) printf("<%s>: TransformType is not defined for the jet.\r\n", APP_NAME);
    return 0;
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
    return false;
  } else {
    if(m_verbose) printf("<%s>: Jet introspection successful.\r\n", APP_NAME);
  }

  return result(jet, res.second, muons);
}

int BoostedXbbTag::result(const xAOD::Jet& jet, std::string algorithm_name, const xAOD::MuonContainer* muons) const {
  // bad configuration
  if(m_bad_configuration){
    if(m_debug) printf("<%s>: BoostedXbbTag has a bad configuration!\r\n", APP_NAME);
    return 0;
  }

  return 1;

}
