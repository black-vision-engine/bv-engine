#include "TestGlobalEffectKeyboardHandler.h"

#include "BVAppLogic.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectDefault.h"


namespace bv {

// *********************************
//
TestGlobalEfectKeyboardHandler::TestGlobalEfectKeyboardHandler  ()
{
    m_defaultEffect = nullptr;
    m_alphaMaskEffect = nullptr;
    m_nodeMaskEffect = nullptr;
}

// *********************************
//
void    TestGlobalEfectKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    if( !m_defaultEffect )
    {
        m_defaultEffect = std::make_shared< model::ModelNodeEffectDefault >( logic->GetGlobalTimeline() );
    }

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
        case ' ':
            HandleSpace( logic );
            
            break;
        case'\\':
            HandleToggleEffect( logic );

            break;
        default:
            ;
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleIncrement     ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == NodeEffectType::NET_ALPHA_MASK )
    {
        auto effect = GetAlphaMaskNodeEffect( logic );
        if( effect->GetType() == NodeEffectType::NET_ALPHA_MASK )
        {
            auto tEffect = std::static_pointer_cast< model::ModelNodeEffectAlphaMask >( effect );
        
            auto alpha = tEffect->GetParamAlpha();

            alpha->SetVal( min( 1.f, alpha->Evaluate() + .1f ), 0.f );

            printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );

        if( effect->GetType() == NodeEffectType::NET_NODE_MASK )
        {
            auto tEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( effect );

            auto alpha = tEffect->GetParamAlpha();

            alpha->SetVal( min( 1.f, alpha->Evaluate() + .1f ), 0.f );
    
            printf( "New node mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleDecrement     ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == NodeEffectType::NET_ALPHA_MASK )
    {
        auto effect = GetAlphaMaskNodeEffect( logic );

        if( effect->GetType() == NodeEffectType::NET_ALPHA_MASK )
        {
            auto tEffect = std::static_pointer_cast< model::ModelNodeEffectAlphaMask >( effect );

            auto alpha = tEffect->GetParamAlpha();

            alpha->SetVal( max( 0.f, alpha->Evaluate() - .1f ), 0.f );

            printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );

        if( effect->GetType() == NodeEffectType::NET_NODE_MASK )
        {
            auto tEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( effect );

            auto alpha = tEffect->GetParamAlpha();

            alpha->SetVal( max( 0.f, alpha->Evaluate() - .1f ), 0.f );

            printf( "New node mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleSpace         ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );
        
        if( effect->GetType() == NodeEffectType::NET_NODE_MASK )
        {
            auto tEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( effect );

            auto paramBg = tEffect->GetParamBgIdx();
            auto paramFg = tEffect->GetParamFgIdx();
    
            auto bgIdx = tEffect->GetBackgroundChildIdx();
            auto fgIdx = tEffect->GetForegroundChildIdx();
    
            paramBg->SetVal( fgIdx, 0.f );
            paramFg->SetVal( bgIdx, 0.f );

            // printf( "New node mask bg: %d fg: %d\n", effect->GetBackgroundChildIdx(), effect->GetForegroundChildIdx() );
        }
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleToggleEffect  ( BVAppLogic * logic )
{
    if( m_curSelectedNode == NodeEffectType::NET_ALPHA_MASK )
    {
        auto node = GetAlphaMaskNode( logic );
        auto effect = GetAlphaMaskNodeEffect( logic );

        if ( effect )
        {
            if ( effect->GetType() == NodeEffectType::NET_ALPHA_MASK )
            {
                m_alphaMaskEffect = std::static_pointer_cast< model::ModelNodeEffectAlphaMask >( effect );
                node->SetNodeEffect( m_defaultEffect );
            }
            else if( effect->GetType() == NodeEffectType::NET_DEFAULT )
            {
                assert( m_alphaMaskEffect );
                
                node->SetNodeEffect( m_alphaMaskEffect );
                m_alphaMaskEffect = nullptr;
            }
            else
            {
                assert( false );
            }
        }
    }
    else if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto node = GetNodeMaskNode( logic );
        auto effect = GetNodeMaskNodeEffect( logic );

        if ( effect )
        {
            if ( effect->GetType() == NodeEffectType::NET_NODE_MASK )
            {
                m_nodeMaskEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( effect );
                node->SetNodeEffect( m_defaultEffect );
            }
            else if( effect->GetType() == NodeEffectType::NET_DEFAULT )
            {
                assert( m_nodeMaskEffect );
                
                node->SetNodeEffect( m_nodeMaskEffect );
                m_nodeMaskEffect = nullptr;
            }
            else
            {
                assert( false );
            }
        }
    }
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetRootNode         ( BVAppLogic * logic )
{
    auto scene = logic->GetBVScene();
    auto root = scene->GetModelSceneRoot();

    return root;
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetNodeByPath       ( BVAppLogic * logic, const std::string & path )
{
    auto root = GetRootNode( logic );

    return std::static_pointer_cast< model::BasicNode >( root->GetNode( path ) );
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetVanillaNode      ( BVAppLogic * logic )
{
    auto node = GetNodeByPath( logic, "root/vanilla" );

    return node;
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetAlphaMaskNode    ( BVAppLogic * logic )
{
    auto node = GetNodeByPath( logic, "root/alpha_mask" );

    return node;
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetNodeMaskNode     ( BVAppLogic * logic )
{
    auto node = GetNodeByPath( logic, "root/node_mask" );

    return node;
}

// *********************************
//
model::IModelNodeEffectPtr TestGlobalEfectKeyboardHandler::GetAlphaMaskNodeEffect( BVAppLogic * logic )
{
    auto node = GetAlphaMaskNode( logic );
    auto effect = node->GetNodeEffect();

    if (!effect) // || effect->GetType() != NodeEffectType::NET_ALPHA_MASK )
    {
        auto newEffect = std::make_shared< model::ModelNodeEffectAlphaMask >( logic->GetGlobalTimeline() );
        node->SetNodeEffect( newEffect );
    }

    return node->GetNodeEffect();
}

// *********************************
//
model::IModelNodeEffectPtr  TestGlobalEfectKeyboardHandler::GetNodeMaskNodeEffect( BVAppLogic * logic )
{
    auto node = GetNodeMaskNode( logic );
    auto effect = node->GetNodeEffect();

    if (!effect) // || effect->GetType() != NodeEffectType::NET_NODE_MASK )
    {
        auto newEffect = std::make_shared< model::ModelNodeEffectNodeMask >( logic->GetGlobalTimeline() );
        node->SetNodeEffect( newEffect );
    }

    return node->GetNodeEffect();
}

} //bv