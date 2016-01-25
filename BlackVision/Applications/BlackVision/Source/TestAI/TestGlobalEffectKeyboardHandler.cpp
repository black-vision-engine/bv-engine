#include "TestGlobalEffectKeyboardHandler.h"

#include "BVAppLogic.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectDefault.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectMixChannels.h"


namespace bv {

// *********************************
//
TestGlobalEfectKeyboardHandler::TestGlobalEfectKeyboardHandler  ()
{
    m_defaultEffect = nullptr;
    m_alphaMaskEffect = nullptr;
    m_nodeMaskEffect = nullptr;
    m_curWireframeNodeIdx = 0;
    m_curMixChannelsPreset = 0;
    m_wireframeDisabled = true;
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
            GetWireframeNode( logic )->SetVisible( false );

            break;
        }
        case '2':
        {
            m_curSelectedNode = NodeEffectType::NET_ALPHA_MASK;

            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( true );
            GetNodeMaskNode( logic )->SetVisible( false );
            GetWireframeNode( logic )->SetVisible( false );

            break;
        }
        case '3':
        {
            m_curSelectedNode = NodeEffectType::NET_NODE_MASK;

            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( true );
            GetWireframeNode( logic )->SetVisible( false );

            break;
        }
        case '4':
        {
            m_curSelectedNode = NodeEffectType::NET_WIREFRAME;

            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( false );
            GetWireframeNode( logic )->SetVisible( true );

            break;
        }
        case '5':
        {
            HandleMixChannels( logic );

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
    else if( m_curSelectedNode == NodeEffectType::NET_WIREFRAME )
    {
        if( !m_wireframeDisabled )
        {
            auto curIdx = m_curWireframeNodeIdx;

            m_curWireframeNodeIdx = ( curIdx + 1 ) % 3;

            auto curNode = GetWireframeNode( logic, curIdx );
            auto nextNode = GetWireframeNode( logic, m_curWireframeNodeIdx );

            curNode->SetNodeEffect( m_defaultEffect );
            auto newEffect = std::make_shared< model::ModelNodeEffectWireframe >( logic->GetGlobalTimeline() );
            nextNode->SetNodeEffect( newEffect );
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
    else if( m_curSelectedNode == NodeEffectType::NET_WIREFRAME )
    {
        if( !m_wireframeDisabled )
        {
            auto curIdx = m_curWireframeNodeIdx;

            if ( curIdx == 0 )
            {
                m_curWireframeNodeIdx = 2;
            }
            else
            {
                m_curWireframeNodeIdx -= 1;
            }

            auto curNode = GetWireframeNode( logic, curIdx );
            auto nextNode = GetWireframeNode( logic, m_curWireframeNodeIdx );

            curNode->SetNodeEffect( m_defaultEffect );
            auto newEffect = std::make_shared< model::ModelNodeEffectWireframe >( logic->GetGlobalTimeline() );
            nextNode->SetNodeEffect( newEffect );
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
    else if( GetRootNode( logic )->GetNodeEffect()->GetType() == NodeEffectType::NET_MIX_CHANNELS )
    {
        SetNextMixChannelsPreset();
    }
}

// *********************************
//FIXME: implement
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
    else if( m_curSelectedNode == NodeEffectType::NET_WIREFRAME )
    {
        auto node = GetWireframeNode( logic, m_curWireframeNodeIdx );
        auto effect = GetNodeWireframeEffect( logic, m_curWireframeNodeIdx );

        if ( effect )
        {
            if ( effect->GetType() == NodeEffectType::NET_WIREFRAME )
            {
                m_wireframeEffect = std::static_pointer_cast< model::ModelNodeEffectWireframe >( effect );
                node->SetNodeEffect( m_defaultEffect );
                m_wireframeDisabled = true;
            }
            else if( effect->GetType() == NodeEffectType::NET_DEFAULT )
            {
                assert( m_wireframeEffect );
                
                node->SetNodeEffect( m_wireframeEffect );
                m_wireframeEffect = nullptr;

                m_wireframeDisabled = false;
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
void                    TestGlobalEfectKeyboardHandler::HandleMixChannels   ( BVAppLogic * logic )
{
    auto root = GetRootNode( logic );

    auto effect = root->GetNodeEffect();

    if ( effect && effect->GetType() == NodeEffectType::NET_MIX_CHANNELS )
    {
        root->SetNodeEffect( m_defaultEffect );
    }
    else
    {
        assert( !effect || effect->GetType() == NodeEffectType::NET_DEFAULT );

        root->SetNodeEffect( m_mixChannelsEffect );
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::SetNextMixChannelsPreset()
{
    auto normal = model::ModelNodeEffectMixChannels::GetChannelMixMask( 0, 1, 2, 3 );

    auto r_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 0, 0, 0, 3 );
    auto g_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 1, 1, 1, 3 );
    auto b_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 2, 2, 2, 3 );
    auto a_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 3, 3, 3, 3 );

    auto all_mask   = glm::vec4( 1.f, 1.f, 1.f, 1.f );
    auto r_mask     = glm::vec4( 1.f, 0.f, 0.f, 1.f );
    auto g_mask     = glm::vec4( 0.f, 1.f, 0.f, 1.f );
    auto b_mask     = glm::vec4( 0.f, 0.f, 1.f, 1.f );

    unsigned int mix_masks[]    = { normal, r_all, g_all, b_all, a_all, normal, normal, normal };
    glm::vec4 channel_masks[]   = { all_mask, all_mask, all_mask, all_mask, all_mask, r_mask, g_mask, b_mask };

    auto cur_mix_mask = int(mix_masks[ m_curMixChannelsPreset ]);
    auto cur_chn_mask = channel_masks[ m_curMixChannelsPreset ];

    m_curMixChannelsPreset = ( m_curMixChannelsPreset + 1 ) % 8;

    auto paramMix  = m_mixChannelsEffect->GetParamChannelMixMask();
    auto paramMask = m_mixChannelsEffect->GetParamChannelMaskMask();

    paramMix->SetVal( cur_mix_mask, 0.f );
    paramMask->SetVal( cur_chn_mask, 0.f );
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
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetWireframeNode    ( BVAppLogic * logic )
{
    auto node = GetNodeByPath( logic, "root/overlay_alpha" );

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

// *********************************
//
model::IModelNodeEffectPtr  TestGlobalEfectKeyboardHandler::GetNodeWireframeEffect   ( BVAppLogic * logic, unsigned int idx )
{
    auto node = GetWireframeNode ( logic, idx );
    auto effect = node->GetNodeEffect();

    if (!effect) // || effect->GetType() != NodeEffectType::NET_WIREFRAME )
    {
        auto newEffect = std::make_shared< model::ModelNodeEffectWireframe >( logic->GetGlobalTimeline() );
        node->SetNodeEffect( newEffect );
    }

    return node->GetNodeEffect();
}

// *********************************
//
model::BasicNodePtr  TestGlobalEfectKeyboardHandler::GetWireframeNode               ( BVAppLogic * logic, unsigned int idx )
{
    std::string nodes[] = {"root/overlay_alpha", "root/overlay_alpha/overlay_alpha_solid", "root/overlay_alpha/overlay_alpha_tex"};

    auto node = GetNodeByPath( logic, nodes[ idx % 3 ] );

    return node;
}

} //bv
