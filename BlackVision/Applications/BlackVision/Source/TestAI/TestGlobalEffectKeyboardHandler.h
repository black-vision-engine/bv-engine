#pragma once

#include "Engine/Models/BasicNode.h"
#include "TestAI/TestKeyboardHandler.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectNodeMask.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectNodeMask.h"


namespace bv {

class TestGlobalEfectKeyboardHandler : public TestKeyboardHandler
{
private:

    NodeEffectType          m_curSelectedNode;

public:

    virtual void            HandleKey       ( unsigned char c, BVAppLogic * logic ) override;
        
private:

    void                    HandleIncrement ( BVAppLogic * logic );
    void                    HandleDecrement ( BVAppLogic * logic );

    model::BasicNodePtr     GetRootNode     ( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeByPath   ( BVAppLogic * logic, const std::string & path );

    model::BasicNodePtr     GetVanillaNode  ( BVAppLogic * logic );
    model::BasicNodePtr     GetAlphaMaskNode( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeMaskNode ( BVAppLogic * logic );

    model::ModelNodeEffectAlphaMaskPtr GetAlphaMaskNodeEffect( BVAppLogic * logic );
    model::ModelNodeEffectNodeMaskPtr  GetNodeMaskNodeEffect( BVAppLogic * logic );

};

} //bv
