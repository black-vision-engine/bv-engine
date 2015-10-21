#include "TestGlobalEffectKeyboardHandler.h"

#include "BVAppLogic.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectNodeMask.h"


namespace bv {

// *********************************
//
void    TestGlobalEfectKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    switch( c )
    {
        case '1':
        {
            GetVanillaNode( logic )->SetVisible( true );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( false );

            break;
        }
        case '2':
        {
            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( true );
            GetNodeMaskNode( logic )->SetVisible( false );

            break;
        }
        case '3':
        {
            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( true );

            break;
        }
        case 'q':
        case 'Q':
        {
            break;
        }
        case 'w':
        case 'W':
        {
            auto node = GetAlphaMaskNode( logic );
            auto effect = node->GetNodeEffect();

            if (!effect || effect->GetType() != NodeEffectType::NET_ALPHA_MASK )
            {
                auto newEffect = std::shared_ptr< model::ModelNodeEffectAlphaMask >( new model::ModelNodeEffectAlphaMask( logic->GetGlobalTimeline() ) );
                node->SetNodeEffect( newEffect );
            }

            auto typedEffect = std::static_pointer_cast< model::ModelNodeEffectAlphaMask >( node->GetNodeEffect() );
            // TODO: some effect related changes
            break;
        }
        case 'e':
        case 'E':
        {
            auto node = GetAlphaMaskNode( logic );
            auto effect = node->GetNodeEffect();

            if (!effect || effect->GetType() != NodeEffectType::NET_NODE_MASK )
            {
                auto newEffect = std::shared_ptr< model::ModelNodeEffectNodeMask >( new model::ModelNodeEffectNodeMask( logic->GetGlobalTimeline() ) );
                node->SetNodeEffect( newEffect );
            }

            auto typedEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( node->GetNodeEffect() );

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
