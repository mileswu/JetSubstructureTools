#ifndef jetsubstructureutils_boostedxbbtag_header
#define jetsubstructureutils_boostedxbbtag_header

// @Author: Giordon Stark
// @Email: gstark@cern.ch

// c++ includes
#include <set>
#include <string>

// EDM includes
#include <xAODJet/JetContainer.h>
#include <xAODMuon/MuonContainer.h>

// forward-declare the ROOT includes
class TFile;
class TH2;

namespace JetSubStructureUtils {
  class BoostedXbbTag {
    public:
      // standard tool constructor
      BoostedXbbTag( std::string working_point           = "medium",
                     float bTagCut                       = 0.8,
                     float massCut                       = 125.0,
                     float D2Cut                         = 1.0,
                     const xAOD::MuonContainer* muons    = nullptr,
                     bool debug                          = false,
                     bool verbose                        = false);

      // this is recommended usage, pass in jet, get true/false
      int result(const xAOD::Jet& jet) const;

      // given the jet and configurations, return the string representation of the jet
      //        eg: AK10LCTRIMF5R20, CA10LCPRUNR50Z15, CA12LCBDRSM100R30Y15
      std::pair<bool, std::string> get_algorithm_name(const xAOD::Jet& jet,
                                     const xAOD::JetAlgorithmType::ID jet_algorithm,
                                     const float size_parameter,
                                     const xAOD::JetInput::Type jet_input,
                                     const xAOD::JetTransform::Type jet_transform) const;

    private:
      std::string m_working_point;
      float m_bTagCut,
            m_massCut,
            m_D2Cut;
      const xAOD::MuonContainer* m_muons;
      bool m_debug,
           m_verbose;

      // this is so we don't error out in general, esp. for athena jobs
      bool m_bad_configuration;

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

      // for D2
      static SG::AuxElement::ConstAccessor<float> D2;
      static SG::AuxElement::ConstAccessor<float> ECF1;
      static SG::AuxElement::ConstAccessor<float> ECF2;
      static SG::AuxElement::ConstAccessor<float> ECF3;
  };
}

#endif
