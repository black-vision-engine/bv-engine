#pragma once

#include "Engine/Models/BasicNode.h"
#include "TestAI/TestKeyboardHandler.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectNodeMask.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectNodeMask.h"


namespace bv {

namespace model {

class ModelNodeEffectDefault;
class ModelNodeEffectAlphaMask;
class ModelNodeEffectNodeMask;

} // model


class TestGlobalEfectKeyboardHandler : public TestKeyboardHandler
{
private:

    NodeEffectType          m_curSelectedNode;

    std::shared_ptr< model::ModelNodeEffectDefault >    m_defaultEffect;
    std::shared_ptr< model::ModelNodeEffectAlphaMask >  m_alphaMaskEffect;
    std::shared_ptr< model::ModelNodeEffectNodeMask >   m_nodeMaskEffect;

public:

            TestGlobalEfectKeyboardHandler      ();

    virtual void            HandleKey           ( unsigned char c, BVAppLogic * logic ) override;
        
private:

    void                    HandleIncrement     ( BVAppLogic * logic );
    void                    HandleDecrement     ( BVAppLogic * logic );
    void                    HandleSpace         ( BVAppLogic * logic );
    void                    HandleToggleEffect  ( BVAppLogic * logic );

    model::BasicNodePtr     GetRootNode         ( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeByPath       ( BVAppLogic * logic, const std::string & path );

    model::BasicNodePtr     GetVanillaNode      ( BVAppLogic * logic );
    model::BasicNodePtr     GetAlphaMaskNode    ( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeMaskNode     ( BVAppLogic * logic );
    model::BasicNodePtr     GetWireframeNode    ( BVAppLogic * logic );

    model::IModelNodeEffectPtr  GetAlphaMaskNodeEffect   ( BVAppLogic * logic );
    model::IModelNodeEffectPtr  GetNodeMaskNodeEffect    ( BVAppLogic * logic );

};

} //bv
