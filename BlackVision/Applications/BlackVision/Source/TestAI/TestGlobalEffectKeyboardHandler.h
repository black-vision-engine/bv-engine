#pragma once


#include "Engine/Models/BasicNode.h"


namespace bv {

class BVAppLogic;


class TestGlobalEfectKeyboardHandler
{
private:

public:

    void                    HandleKey       ( unsigned char c, BVAppLogic * logic );
        
private:

    model::BasicNodePtr     GetRootNode     ( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeByPath   ( BVAppLogic * logic, const std::string & path );

    model::BasicNodePtr     GetVanillaNode  ( BVAppLogic * logic );
    model::BasicNodePtr     GetAlphaMaskNode( BVAppLogic * logic );
    model::BasicNodePtr     GetNodeMaskNode ( BVAppLogic * logic );
        
    
};

} //bv


