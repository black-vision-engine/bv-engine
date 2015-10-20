#include "TestGlobalEffectKeyboardHandler.h"
#include "BVAppLogic.h"


namespace bv {

// *********************************
//
void    TestGlobalEfectKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    assert( false );

    switch( c )
    {
        case '1':
        {
            auto amNode = GetAlphaMaskNode( logic )->GetNodeEffect();
            auto nmNode = GetNodeMaskNode( logic )->GetNodeEffect();
            break;
        }
        case '2':
        {
            auto amNode = GetAlphaMaskNode( logic )->GetNodeEffect();
            break;
        }
        case '3':
        {
            auto nmNode = GetNodeMaskNode( logic )->GetNodeEffect();
            break;
        }
        default:
            assert( false );
    }
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetRootNode     ( BVAppLogic * logic )
{
    auto scene = logic->GetBVScene();
    auto root = scene->GetModelSceneRoot();

    return root;
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetNodeByPath   ( BVAppLogic * logic, const std::string & path )
{
    auto root = GetRootNode( logic );

    return std::static_pointer_cast< model::BasicNode >( root->GetNode( path ) );
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetVanillaNode  ( BVAppLogic * logic )
{
    auto node = GetNodeByPath( logic, "root/vanilla" );

    return node;
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetAlphaMaskNode( BVAppLogic * logic )
{
    auto node = GetNodeByPath( logic, "root/alpha_mask" );

    return node;
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetNodeMaskNode ( BVAppLogic * logic )
{
    auto node = GetNodeByPath( logic, "root/node_mask" );

    return node;
}

} //bv
