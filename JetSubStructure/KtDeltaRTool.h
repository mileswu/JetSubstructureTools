#ifndef JETSUBSTRUCTURE_KTDELTARTOOL_H
#define JETSUBSTRUCTURE_KTDELTARTOOL_H

#include "JetRec/JetModifierBase.h"

#include "GaudiKernel/ToolHandle.h"
#include "JetInterface/IFastJetInterfaceTool.h"

#define ASG_DERIVED_TOOL_CLASS( CLASSNAME )\
public: \
  CLASSNAME(const std::string&, const std::string& myname, const IInterface*) \
  : CLASSNAME(myname) { \
  }

class KtDeltaRTool : public JetModifierBase
{
    ASG_DERIVED_TOOL_CLASS(KtDeltaRTool);
    
    public:
        KtDeltaRTool(const std::string& t);

        virtual StatusCode initialize();

        virtual int modifyJet(xAOD::Jet&) const;

    private:
        ToolHandle<IFastJetInterfaceTool> m_fastjetTool;
        std::string m_fromAssociation;
        double m_ptCut;
};

#undef ASG_DERIVED_TOOL_CLASS
#endif
