#include "JetSubStructure/KtDeltaRTool.h"

#include "xAODJet/JetConstituentVector.h"

namespace
{
    static SG::AuxElement::Accessor<float> KtDRAcc("KtDR");
}

KtDeltaRTool::KtDeltaRTool(const std::string& t)
    : JetModifierBase(t)
    , m_fastjetTool("FastJetInterface")
    , m_fromAssociation("")
    , m_ptCut(0)
{
    declareProperty("FastJetTool",m_fastjetTool);
    declareProperty("FromJetAssociation", m_fromAssociation, "If non void, get input for subjets from the jet association, else from jet constituents");
    declareProperty("MinPt", m_ptCut);
}

StatusCode KtDeltaRTool::initialize()
{
    // Retrieve FastJet tool
    if (!m_fastjetTool.retrieve().isSuccess())
    {
        ATH_MSG_ERROR("Could not retrive FastJet tool");
        return StatusCode::FAILURE;
    }

    return StatusCode::SUCCESS;
}

int KtDeltaRTool::modifyJet(xAOD::Jet& jet) const
{
    // TODO: Add jet stream operator?
    //ATH_MSG_DEBUG( "  process  "<< jet );

    const xAOD::JetConstituentVector constituents = jet.getConstituents();
    if (constituents.size() < 2)
        return 0;
    
    // Prepare the moment accessor
    float& KtDR = KtDRAcc(jet);
    KtDR = 0;

    IFastJetInterfaceTool::fjetlist_t pseudojets;

    if(m_fromAssociation == "")
    {
        xAOD::JetConstituentVector::iterator iter = constituents.begin();
        xAOD::JetConstituentVector::iterator iEnd = constituents.end();
        pseudojets.reserve(constituents.size());
        for( ; iter != iEnd; ++iter)
        {
            pseudojets.push_back( fastjet::PseudoJet( iter->px(), iter->py(), iter->pz(), iter->e()) );  
        }
    }
    else
    {
        // get input from association
        const std::vector<const xAOD::IParticle*>& assoParticles = jet.getAssociatedParticles(m_fromAssociation);

        if (assoParticles.size())
        {
            std::vector<const xAOD::IParticle*>::const_iterator iter = assoParticles.begin();
            std::vector<const xAOD::IParticle*>::const_iterator iEnd = assoParticles.end();
            for ( ; iter != iEnd; ++iter)
            {
                const TLorentzVector& fourVec = (*iter)->p4();
                if ( (*iter)->pt() > m_ptCut)
                    pseudojets.push_back( fastjet::PseudoJet( fourVec.Px(), fourVec.Py(), fourVec.Pz(), fourVec.E() ) );
                    //pseudojets.push_back( fastjet::PseudoJet( (*iter)->px(), (*iter)->py(), (*iter)->pz(), (*iter)->e()) );
            }
        }

        //const JetAssociationBase * ja = jet->getAssociationBase(m_fromAssociation);

        //if(ja)
        //{
        //    NavigationToken<INavigable4Momentum,double> jaToken;
        //    ja->fillToken(jaToken , 1.0);
        //    NavigationToken<INavigable4Momentum,double>::const_iterator it = jaToken.begin();
        //    NavigationToken<INavigable4Momentum,double>::const_iterator itE = jaToken.end();
        //    for( ; it != itE; ++it)
        //    {
        //        if( (*it)->pt() > m_ptCut) pseudojets.push_back( fastjet::PseudoJet(  (*it)->px(), (*it)->py(), (*it)->pz(), (*it)->e()) );
        //    }
        //}
        
        ATH_MSG_DEBUG("input size "<< pseudojets.size() << "   ptcut="<< m_ptCut);
    }

    if( pseudojets.size() > 1 )
    {
        IFastJetInterfaceTool::fjetlist_t outjets;
        ATH_MSG_DEBUG( "  executing fastjet seq.   seq= "<< m_fastjetTool->clusterSequence() );

        if (!m_fastjetTool->execute(pseudojets,outjets).isSuccess())
            return 1;
        else if (outjets.size() < 2)
            return 0;

        // TODO: Switch to JetUtils helpers when package is migrated
        double dr = sqrt(pow(outjets[1].eta()-outjets[0].eta(),2)+pow(outjets[1].phi()-outjets[0].phi(),2));

        KtDR = dr;
    }

    return 0;
}

