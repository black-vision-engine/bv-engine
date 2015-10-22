#pragma once

#include "Engine/Models/BasicNode.h"
#include "TestAI/TestKeyboardHandler.h"


namespace bv {

class TestGlobalEfectKeyboardHandler : public TestKeyboardHandler
{
public:

    virtual void            HandleKey       ( unsigned char c, BVAppLogic * logic ) override;
        
private:

    model::BasicNodePtr     GetRootNode     ( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeByPath   ( BVAppLogic * logic, const std::string & path );

    model::BasicNodePtr     GetVanillaNode  ( BVAppLogic * logic );
    model::BasicNodePtr     GetAlphaMaskNode( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeMaskNode ( BVAppLogic * logic );

};

} //bv
