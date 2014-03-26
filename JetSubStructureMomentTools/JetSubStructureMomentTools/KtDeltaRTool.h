#ifndef JETSUBSTRUCTURE_KTDELTARTOOL_H
#define JETSUBSTRUCTURE_KTDELTARTOOL_H

#include "JetRec/JetModifierBase.h"
#include "GaudiKernel/ToolHandle.h"

class KtDeltaRTool : public JetModifierBase {
  ASG_TOOL_CLASS(KtDeltaRTool, IJetModifier)

public:

  // Ctor.
  KtDeltaRTool(const std::string& t);

  // Initialization.
  StatusCode initialize();

  // Add moment to a jet.
  int modifyJet(xAOD::Jet&) const;

  // Display properties.
  void print() const;

private:

  double m_jetrad;

};

#endif
