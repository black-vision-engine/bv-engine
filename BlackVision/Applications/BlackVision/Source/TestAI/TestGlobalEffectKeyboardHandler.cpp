#include "TestGlobalEffectKeyboardHandler.h"

#include "BVAppLogic.h"


namespace bv {

// *********************************
//
void    TestGlobalEfectKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    switch( c )
    {
        case '1':
        {
            m_curSelectedNode = NodeEffectType::NET_DEFAULT;

            GetVanillaNode( logic )->SetVisible( true );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( false );

            break;
        }
        case '2':
        {
            m_curSelectedNode = NodeEffectType::NET_ALPHA_MASK;

            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( true );
            GetNodeMaskNode( logic )->SetVisible( false );

            break;
        }
        case '3':
        {
            m_curSelectedNode = NodeEffectType::NET_NODE_MASK;

            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( true );

            break;
        }
        case '[':
            HandleDecrement( logic );

            break;
        case ']':
            HandleIncrement( logic );

            break;
        default:
            ;
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleIncrement ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == NodeEffectType::NET_ALPHA_MASK )
    {
        auto effect = GetAlphaMaskNodeEffect( logic );
        auto alpha = effect->GetParamAlpha();

        alpha->SetVal( min( 1.f, alpha->Evaluate() + .1f ), 0.f );

        printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
    }
    else if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );

        auto paramBg = effect->GetParamBgIdx();
        auto paramFg = effect->GetParamFgIdx();

        auto bgIdx = effect->GetBackgroundChildIdx();
        auto fgIdx = effect->GetForegroundChildIdx();

        paramBg->SetVal( fgIdx, 0.f );
        paramFg->SetVal( bgIdx, 0.f );

        printf( "New node mask bg: %d fg: %d\n", effect->GetBackgroundChildIdx(), effect->GetForegroundChildIdx() );
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleDecrement ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == NodeEffectType::NET_ALPHA_MASK )
    {
        auto effect = GetAlphaMaskNodeEffect( logic );
        auto alpha = effect->GetParamAlpha();

        alpha->SetVal( max( 0.f, alpha->Evaluate() - .1f ), 0.f );

        printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
    }
    else if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );
        
        auto paramBg = effect->GetParamBgIdx();
        auto paramFg = effect->GetParamFgIdx();
    
        auto bgIdx = effect->GetBackgroundChildIdx();
        auto fgIdx = effect->GetForegroundChildIdx();
    
        paramBg->SetVal( fgIdx, 0.f );
        paramFg->SetVal( bgIdx, 0.f );

        printf( "New node mask bg: %d fg: %d\n", effect->GetBackgroundChildIdx(), effect->GetForegroundChildIdx() );
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

// *********************************
//
model::ModelNodeEffectAlphaMaskPtr TestGlobalEfectKeyboardHandler::GetAlphaMaskNodeEffect( BVAppLogic * logic )
{
    auto node = GetNodeMaskNode( logic );
    auto effect = node->GetNodeEffect();

    if (!effect || effect->GetType() != NodeEffectType::NET_ALPHA_MASK )
    {
        auto newEffect = std::make_shared< model::ModelNodeEffectAlphaMask >( logic->GetGlobalTimeline() );
        node->SetNodeEffect( newEffect );
    }

    return std::static_pointer_cast< model::ModelNodeEffectAlphaMask >( node->GetNodeEffect() );
}

// *********************************
//
model::ModelNodeEffectNodeMaskPtr  TestGlobalEfectKeyboardHandler::GetNodeMaskNodeEffect( BVAppLogic * logic )
{
    auto node = GetNodeMaskNode( logic );
    auto effect = node->GetNodeEffect();

    if (!effect || effect->GetType() != NodeEffectType::NET_NODE_MASK )
    {
        auto newEffect = std::make_shared< model::ModelNodeEffectNodeMask >( logic->GetGlobalTimeline() );
        node->SetNodeEffect( newEffect );
    }

    return std::static_pointer_cast< model::ModelNodeEffectNodeMask >( node->GetNodeEffect() );
}

} //bv
