#include "JetSubStructureUtils/BosonTag.h"
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

using namespace JetSubStructureUtils;

// make all static accessors static to this file, like extern but hip
SG::AuxElement::ConstAccessor<int>      BosonTag::AlgorithmType ("AlgorithmType");
SG::AuxElement::ConstAccessor<float>    BosonTag::SizeParameter ("SizeParameter");
SG::AuxElement::ConstAccessor<int>      BosonTag::InputType ("InputType");
SG::AuxElement::ConstAccessor<int>      BosonTag::TransformType ("TransformType");
SG::AuxElement::ConstAccessor<float>    BosonTag::RClus ("RClus");
SG::AuxElement::ConstAccessor<float>    BosonTag::PtFrac ("PtFrac");
SG::AuxElement::ConstAccessor<float>    BosonTag::RCut ("RCut");
SG::AuxElement::ConstAccessor<float>    BosonTag::ZCut ("ZCut");
SG::AuxElement::ConstAccessor<char>     BosonTag::BDRS ("BDRS");
SG::AuxElement::ConstAccessor<float>    BosonTag::YMin ("YMin");
SG::AuxElement::ConstAccessor<float>    BosonTag::MuMax ("MuMax");
SG::AuxElement::ConstAccessor<float>    BosonTag::Tau1 ("Tau1");
SG::AuxElement::ConstAccessor<float>    BosonTag::Tau2 ("Tau2");
SG::AuxElement::ConstAccessor<float>    BosonTag::C2 ("C2");
SG::AuxElement::ConstAccessor<float>    BosonTag::D2 ("D2");
SG::AuxElement::ConstAccessor<float>    BosonTag::ECF1 ("ECF1");
SG::AuxElement::ConstAccessor<float>    BosonTag::ECF2 ("ECF2");
SG::AuxElement::ConstAccessor<float>    BosonTag::ECF3 ("ECF3");

void BosonTag::CONFIG::openFile(){
    if(!twovar_signalregion_file){
      twovar_signalregion_file = TFile::Open(twovar_signalregion_filepath.c_str());
    }
  }

void BosonTag::CONFIG::openHist(){
  if(!twovar_signalregion_hist){
    if(!twovar_signalregion_file) openFile();
    twovar_signalregion_hist = (TH2*)twovar_signalregion_file->Get("SignalRegion");
  }
}

double BosonTag::CONFIG::getTwoVar(double d2beta1, double tau21){
  if(!twovar_signalregion_hist) openHist();
  int binX = twovar_signalregion_hist->GetXaxis()->FindBin(d2beta1);
  int binY = twovar_signalregion_hist->GetYaxis()->FindBin(tau21);
  return static_cast<double>(twovar_signalregion_hist->GetBinContent(binX, binY) == 1);
}

bool BosonTag::CONFIG::compareVariable(double variableValue){
  if(var_dir != "LEFT" && var_dir != "RIGHT") return false;
  return !((variableValue < var_cut)^(var_dir == "LEFT"));
}

BosonTag::CONFIG::~CONFIG() {
  if(twovar_signalregion_file) delete twovar_signalregion_file;
  if(twovar_signalregion_hist) delete twovar_signalregion_hist;
}

BosonTag::BosonTag( std::string working_point,
                    std::string tagger_alg,
                    std::string recommendations_file,
                    bool compare_mass,
                    bool compare_variable,
                    bool simple_tagger,
                    bool debug) :
  m_working_point(working_point),
  m_tagger_alg(tagger_alg),
  m_recommendations_file(recommendations_file),
  m_compare_mass(compare_mass),
  m_compare_variable(compare_variable),
  m_simple_tagger(simple_tagger),
  m_debug(debug),
  m_bad_configuration(false)
{

  /* check configurations passed in, use m_bad_configuration as flag:
        - flag it to true if something is badly configured
        - otherwise, it should be false if everything seems ok
  */
  std::set<std::string> validWorkingPoints;
  validWorkingPoints.insert("tight");
  validWorkingPoints.insert("medium");
  validWorkingPoints.insert("loose");
  validWorkingPoints.insert("veryloose");

  if( validWorkingPoints.find(m_working_point) == validWorkingPoints.end()){
    printf("Unknown working point requested %s!\r\n", m_working_point.c_str());
    m_bad_configuration |= true;
  }

  std::set<std::string> validTaggers;
  validTaggers.insert("tau21");
  validTaggers.insert("c2beta1");
  validTaggers.insert("d2beta1");
  validTaggers.insert("twovar");

  if( validTaggers.find(m_tagger_alg) == validTaggers.end()){
    printf("Unknown taggger requested %s!\r\n", m_tagger_alg.c_str());
    m_bad_configuration |= true;
  }

  // at least one must be set to true
  if( !(m_compare_mass|m_compare_variable) ){
    printf("Both MassCompare and VarCompare cannot be false!\r\n");
    m_bad_configuration |= true;
  }

  if( m_recommendations_file.empty() ){
    std::cout << "No configuration file specified!" << std::endl;
    m_bad_configuration |= true;
  } else {
#ifdef ROOTCORE
    m_recommendations_file = gSystem->ExpandPathName(m_recommendations_file.c_str());
#else
    m_recommendations_file = PathResolverFindXMLFile(m_recommendations_file);
#endif
    /* https://root.cern.ch/root/roottalk/roottalk02/5332.html */
    FileStat_t fStats;
    int fSuccess = gSystem->GetPathInfo(m_recommendations_file.c_str(), fStats);
    if(fSuccess != 0){
      printf("Recommendations file could not be found: %s!\r\n", m_recommendations_file.c_str());
      m_bad_configuration |= true;
    } else {
      // if we made it here, everything appears ok with our file, attempt to read it
      std::ifstream f_in;
      f_in.open(m_recommendations_file, std::ios::in);
      if( f_in.fail() ){
        std::cout << "Something is wrong with the recommendations file. Could not open for reading." << std::endl;
        m_bad_configuration |= true;
      }

      std::string line;
      while( std::getline(f_in, line) ){
        /* token contains the current splitted text */
        std::string token;

        if(m_debug) printf("Reading in line: %s\r\n", line.c_str());

        /* notes, one should add a check on all values to make sure they are valid! */

        // split by space
        std::istringstream ss(line);
        /* lineDetails is an array of the splits */
        std::vector<std::string> lineDetails{std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>{}};

        std::string l_working_point     = lineDetails[0];
        std::string l_tagger            = lineDetails[3];
        std::string l_algorithm         = lineDetails[1];
        // might need to do std::stoi(lineDetails[2].substr(2,5).c_str())
        double l_ptbin                  = std::stoi(lineDetails[2].substr(2,5).c_str());

        BosonTag::CONFIG new_config = {};
        new_config.cut_low   = std::stod(lineDetails[4]);
        new_config.cut_high  = std::stod(lineDetails[5]);
        new_config.var_dir   = lineDetails[6];
        new_config.var_cut   = std::stod(lineDetails[7]);
        if(lineDetails.size() == 9) new_config.twovar_signalregion_filepath = lineDetails[8];

        m_configurations[l_working_point][l_tagger][l_algorithm][l_ptbin] = new_config;

        if(m_debug) printf("Parsed line as: %s | %s | %0.2f | %s | %0.2f | %0.2f | %s | %0.2f | %s\r\n",
                            l_working_point.c_str(), l_algorithm.c_str(), l_ptbin, l_tagger.c_str(), new_config.cut_low, new_config.cut_high,
                            new_config.var_dir.c_str(), new_config.var_cut, new_config.twovar_signalregion_filepath.c_str());
      }

      if( !m_configurations.count(m_working_point) ){
        printf("Could not find working point %s in recommendations file!\r\n", m_working_point.c_str());
        m_bad_configuration |= true;
      } else if( !m_configurations.at(m_working_point).count(m_tagger_alg) ){
        printf("Could not find tagger %s in recommendations file!\r\n", m_tagger_alg.c_str());
        m_bad_configuration |= true;
      }

    }
  }


  if(m_bad_configuration){
    std::cout << "|=====================================================|" << std::endl;
    std::cout << "|        WARNING        WARNING        WARNING        |" << std::endl;
    std::cout << "|-----------------------------------------------------|" << std::endl;
    std::cout << "|            BosonTagger is misconfigured!            |" << std::endl;
    std::cout << "|=====================================================|" << std::endl;
  }

}

std::pair<bool, BosonTag::CONFIG> BosonTag::get_configuration(const xAOD::Jet& jet,
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
      if( !PtFrac.isAvailable(jet) ){ std::cout << "PtFrac is not defined for the input jet." << std::endl; error_flag |= true; }
      if( !RClus.isAvailable(jet) ){ std::cout << "RClus is not defined for the input jet." << std::endl; error_flag |= true; }
      if(m_debug) printf("PtFrac: %0.2f\tRClus: %0.2f\r\n", PtFrac(jet), RClus(jet));
      algorithm_name += "F" + std::to_string(static_cast<int>(PtFrac(jet)*100))
                       + "R" + std::to_string(static_cast<int>(RClus(jet)*100));
    break;
    case xAOD::JetTransform::Prune:
      if( !RCut.isAvailable(jet) ){ std::cout << "RCut is not defined for the input jet." << std::endl; error_flag |= true; }
      if( !ZCut.isAvailable(jet) ){ std::cout << "ZCut is not defined for the input jet." << std::endl; error_flag |= true; }
      if(m_debug) printf("RCut: %0.2f\tZCut: %0.2f\r\n", RCut(jet), ZCut(jet));
      algorithm_name += "R" + std::to_string(static_cast<int>(RCut(jet)*100))
                       + "Z" + std::to_string(static_cast<int>(ZCut(jet)*100));
    break;
    case xAOD::JetTransform::MassDrop:
      if( !MuMax.isAvailable(jet) ){ std::cout << "MuMax is not defined for the input jet." << std::endl; error_flag |= true; }
      if( !RClus.isAvailable(jet) ){ std::cout << "RClus is not defined for the input jet." << std::endl; error_flag |= true; }
      if( !YMin.isAvailable(jet) ){  std::cout << "YMin is not defined for the input jet." << std::endl; error_flag |= true; }
      if( !BDRS.isAvailable(jet) ){  std::cout << "BDRS is not defined for the input jet." << std::endl; error_flag |= true; }
      if(m_debug) printf("MuMax: %0.2f\tRClus: %0.2f\tYMin: %0.2f\r\n", MuMax(jet), RClus(jet), YMin(jet));
      algorithm_name += "M" + std::to_string(static_cast<int>(MuMax(jet)*100))
                       + "R" + std::to_string(static_cast<int>(RClus(jet)*100))
                       + "Y" + std::to_string(static_cast<int>(YMin(jet)*100));
    break;
    default:
      std::cout << "Current value of xAOD::JetTransform::Type is not supported!" << std::endl;
      error_flag |= true;
    break;
  }

  // next, get the ptbin
  double ptbin(0.0);
  double jetPt(jet.pt()/1000.);
  // set ptbin to 200 if using the "simple" tagger option
  if(m_simple_tagger) ptbin = 200.0;
  else {
    if      (jetPt > 200.0  && jetPt < 350.0 ) ptbin =  200.0;
    else if (jetPt > 350.0  && jetPt < 500.0 ) ptbin =  350.0;
    else if (jetPt > 500.0  && jetPt < 1000.0) ptbin =  500.0;
    else if (jetPt > 1000.0 && jetPt < 1500.0) ptbin = 1000.0;
    else if (jetPt > 1500.0 && jetPt < 2000.0) ptbin = 1500.0;
    //else if (jetPt > 2000.0 && jetPt < 3000.0) ptbin = 2000.0;
    else ptbin = 200.0;
  }

  if(m_debug) printf("PtBin: %0.2f\r\n", ptbin);

  // grab the correct configuration
  if( !m_configurations.at(m_working_point).at(m_tagger_alg).count(algorithm_name) ){
    printf("Could not find algorithm %s in recommendations file!\r\n", algorithm_name.c_str());
    error_flag |= true;
  } else if( !m_configurations.at(m_working_point).at(m_tagger_alg).at(algorithm_name).count(ptbin) ){
    printf("Could not find ptbin %0.2f in recommendations file!\r\n", ptbin);
    error_flag |= true;
  }

  /* error flag tells us if a valid configuration exists or not */
  if(error_flag){
    //static BosonTag::CONFIG null_config = {};
    return std::pair<bool, BosonTag::CONFIG>(false, {});
  }

  if(m_debug) std::cout << "Found the correct configuration parameters for the jet." << std::endl;
  return std::pair<bool, BosonTag::CONFIG>(true, m_configurations.at(m_working_point).at(m_tagger_alg).at(algorithm_name).at(ptbin));
}

bool BosonTag::check_jet(const xAOD::Jet& jet) const
{
  // check that what we are looking for is actually available
  if(!Tau1.isAvailable(jet) || !Tau2.isAvailable(jet)){
    if(m_tagger_alg == "tau21" || m_tagger_alg == "twovar"){
      std::cout << "Tau1 or Tau2 are not available!" << std::endl;
      return false;
    }
  }

  if(!ECF1.isAvailable(jet) || !ECF2.isAvailable(jet) || !ECF3.isAvailable(jet)){
    if(m_tagger_alg != "tau21") {
      std::cout << "ECF# variables are not available." << std::endl;
      return false;
    }
  }

  return true;
}

bool BosonTag::result(const xAOD::Jet& jet) const
{
  if(m_bad_configuration) return false;

  // if we call via this method, we need these 4 things defined
  if( !AlgorithmType.isAvailable(jet) ){ std::cout << "AlgorithmType is not defined for the jet."   << std::endl; return false;}
  if( !SizeParameter.isAvailable(jet) ){ std::cout << "SizeParameter is not defined for the jet."   << std::endl; return false;}
  if( !InputType.isAvailable(jet) )    { std::cout << "InputType is not defined for the jet."       << std::endl; return false;}
  if( !TransformType.isAvailable(jet) ){ std::cout << "TransformType is not defined for the jet."   << std::endl; return false;}

  return result(jet,
                static_cast<xAOD::JetAlgorithmType::ID>(AlgorithmType(jet)),
                SizeParameter(jet),
                static_cast<xAOD::JetInput::Type>(InputType(jet)),
                static_cast<xAOD::JetTransform::Type>(TransformType(jet)));
}

bool BosonTag::result(const xAOD::Jet& jet,
                      const xAOD::JetAlgorithmType::ID jet_algorithm,
                      const float size_parameter,
                      const xAOD::JetInput::Type jet_input,
                      const xAOD::JetTransform::Type jet_transform) const
{
  // bad configuration
  if(m_bad_configuration){ std::cout << "BosonTag has a bad configuration!" << std::endl; return false; }
  // jet is missing tau1, tau2, or ecf variables
  if(!check_jet(jet)){ std::cout << "The given jet is missing tau1, tau2, or ecf variables." << std::endl; return false; }

  // could not find a configuration for the particular jet
  std::pair<bool, BosonTag::CONFIG> config_parameters = get_configuration(jet, jet_algorithm, size_parameter, jet_input, jet_transform);
  if(!config_parameters.first){ std::cout << "The given jet does not have a configuration parameter." << std::endl; return false; }

  // attempt to compute the variable value we cut against
  double variable_value(-999.0);

  // Dear Giordon: don't change this back to switch() again, you're an idiot.
  if(m_tagger_alg == "tau21"){
      variable_value = Tau2(jet)/Tau1(jet);
  } else if(m_tagger_alg == "twovar"){
      double d2(0.0), tau21(0.0);
      tau21 = Tau2(jet)/Tau1(jet);
      if(D2.isAvailable(jet)){
        d2 = D2(jet);
      } else {
        d2 = ECF3(jet) * pow(ECF1(jet), 3.0) / pow(ECF2(jet), 3.0);
      }
      variable_value = config_parameters.second.getTwoVar(d2, tau21);
  } else if(m_tagger_alg == "c2beta1"){
      if(C2.isAvailable(jet)){
        variable_value = C2(jet);
      } else {
        variable_value = ECF3(jet) * ECF1(jet) / pow(ECF2(jet), 2.0);
    }
  } else if(m_tagger_alg == "d2beta1"){
      if(D2.isAvailable(jet)){
        variable_value = D2(jet);
      } else {
        variable_value = ECF3(jet) * pow(ECF1(jet), 3.0) / pow(ECF2(jet), 3.0);
      }
  }

  // figure out if we tag or not
  bool b_mass_compare = (jet.m()/1000. > config_parameters.second.cut_low && jet.m()/1000. < config_parameters.second.cut_high);
  bool b_variable_compare = config_parameters.second.compareVariable(variable_value);
  if( m_compare_mass & ~m_compare_variable) return b_mass_compare;
  if(~m_compare_mass &  m_compare_variable) return b_variable_compare;
  if( m_compare_mass &  m_compare_variable) return b_mass_compare&b_variable_compare;

  // always return false by default
  // a false negative is better than a false positive
  std::cout << "Something bad happened. Please report to somebody important." << std::endl;
  return false;
}
