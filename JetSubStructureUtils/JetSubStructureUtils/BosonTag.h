#ifndef jetsubstructureutils_bosontag_header
#define jetsubstructureutils_bosontag_header

// c++ includes
#include <set>
#include <string>

// EDM includes
#include <xAODJet/Jet.h>

// forward-declare the ROOT includes
class TFile;
class TH2;

namespace JetSubStructureUtils {
  class BosonTag {
    public:
      /* struct holding the configuration details read in from the file */
      struct CONFIG {
        double cut_low = 0.0;
        double cut_high = 0.0;
        double var_cut = 0.0;
        std::string var_dir = "";
        std::string twovar_signalregion_filepath = "";
        TFile* twovar_signalregion_file = nullptr;
        TH2* twovar_signalregion_hist = nullptr;

        void openFile();
        void openHist();
        double getTwoVar(double d2beta1, double tau21);
        bool compareVariable(double variableValue);
        ~CONFIG();
      };

      BosonTag( std::string working_point           = "medium",
                std::string tagger_alg              = "tau21",
#ifdef ROOTCORE
                std::string recommendations_file    = "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_20150306.dat",
#else
                std::string recommendations_file    = "JetSubStructureUtils/data/config_13TeV_20150306.dat",
#endif
                bool compare_mass                    = true,
                bool compare_variable                = true,
                bool simple_tagger                   = false,
                bool debug                           = false);

      // this is recommended usage, pass in jet, get true/false
      bool result(const xAOD::Jet& jet) const;
      // sometimes you don't have certain properties set so pass them in
      //    to select the appropriate tagging recommendation
      bool result(const xAOD::Jet& jet,
                  const xAOD::JetAlgorithmType::ID jet_algorithm,
                  const float size_parameter,
                  const xAOD::JetInput::Type jet_input,
                  const xAOD::JetTransform::Type jet_transform) const;

      bool check_jet(const xAOD::Jet& jet) const;

      std::pair<bool, CONFIG> get_configuration(const xAOD::Jet& jet,
                                                const xAOD::JetAlgorithmType::ID jet_algorithm,
                                                const float size_parameter,
                                                const xAOD::JetInput::Type jet_input,
                                                const xAOD::JetTransform::Type jet_transform) const;


    private:
      std::string m_working_point,
                  m_tagger_alg,
                  m_recommendations_file;
      bool m_compare_mass,
           m_compare_variable,
           m_simple_tagger,
           m_debug;

      // this is so we don't error out in general, esp. for athena jobs
      bool m_bad_configuration;



      /* map<workingPoint,
             map<tagger,
                 map<algorithm,
                     map<ptbin, CONFIG>
                    >
                >
            >
      */
      // use (fastjet::JetAlgorithm) jet->getAlgorithmType() later
      // #include <fastjet/JetDefinition.hh>
      std::map<std::string, std::map<std::string, std::map<std::string, std::map<double, CONFIG>>>> m_configurations;

      // main 4 details for classifying a jet
      static SG::AuxElement::ConstAccessor<int> AlgorithmType;
      static SG::AuxElement::ConstAccessor<float> SizeParameter;
      static SG::AuxElement::ConstAccessor<int> InputType;
      static SG::AuxElement::ConstAccessor<int> TransformType;

      // for trimming
      static SG::AuxElement::ConstAccessor<float> RClus;
      static SG::AuxElement::ConstAccessor<float> PtFrac;

      // for pruning
      static SG::AuxElement::ConstAccessor<float> RCut;
      static SG::AuxElement::ConstAccessor<float> ZCut;

      // for splitting
      // static SG::AuxElement::ConstAccessor<int> NSubjetMax ("NSubjetMax");
      static SG::AuxElement::ConstAccessor<char> BDRS;
      /* MuMax, YMin, RClus */
      // static SG::AuxElement::ConstAccessor<float> RClus ("RClus"); // defined above for trimming
      static SG::AuxElement::ConstAccessor<float> YMin;
      static SG::AuxElement::ConstAccessor<float> MuMax;

      // for tau21
      static SG::AuxElement::ConstAccessor<float> Tau1;
      static SG::AuxElement::ConstAccessor<float> Tau2;

      // for C2, D2
      static SG::AuxElement::ConstAccessor<float> C2;
      static SG::AuxElement::ConstAccessor<float> D2;
      static SG::AuxElement::ConstAccessor<float> ECF1;
      static SG::AuxElement::ConstAccessor<float> ECF2;
      static SG::AuxElement::ConstAccessor<float> ECF3;

  };
}

#endif
