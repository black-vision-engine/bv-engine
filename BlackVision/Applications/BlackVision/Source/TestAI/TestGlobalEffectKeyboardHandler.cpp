#include "TestGlobalEffectKeyboardHandler.h"

#include "BVAppLogic.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectFactory.h"
#include "Engine/Models/NodeEffects/ModelFullscreenEffectMixChannels.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


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

    m_editor = nullptr;
}

// *********************************
//
void    TestGlobalEfectKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    if( !m_editor ) 
	{
        m_editor = logic->GetBVScene()->GetSceneEditor();
    }

    if( !m_defaultEffect )
    {
        m_defaultEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_DEFAULT, "default", logic->GetGlobalTimeline() );
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
        auto fse = effect->GetFullscreenEffect();

        if( fse->GetType() == NodeEffectType::NET_ALPHA_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( fse->GetParameter( "alpha" ) );

            alpha->SetVal( min( 1.f, alpha->Evaluate() + .1f ), 0.f );

            printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto fse = GetNodeMaskNodeEffect( logic )->GetFullscreenEffect();

        if( fse->GetType() == NodeEffectType::NET_NODE_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( fse->GetParameter( "alpha" ) );

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

            m_editor->SetNodeEffect( curNode, m_defaultEffect );
            auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_WIREFRAME, "wireframe", logic->GetGlobalTimeline() );
            m_editor->SetNodeEffect( nextNode, newEffect );
        }        
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleDecrement     ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == NodeEffectType::NET_ALPHA_MASK )
    {
        auto fse = GetAlphaMaskNodeEffect( logic )->GetFullscreenEffect();

        if( fse->GetType() == NodeEffectType::NET_ALPHA_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( fse->GetParameter( "alpha" ) );

            alpha->SetVal( max( 0.f, alpha->Evaluate() - .1f ), 0.f );

            printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto fse = GetNodeMaskNodeEffect( logic )->GetFullscreenEffect();

        if( fse->GetType() == NodeEffectType::NET_NODE_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( fse->GetParameter( "alpha" ) );

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

            m_editor->SetNodeEffect( curNode, m_defaultEffect );
            auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_WIREFRAME, "wireframe", logic->GetGlobalTimeline() );
            m_editor->SetNodeEffect( nextNode, newEffect );
        }
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleSpace         ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == NodeEffectType::NET_NODE_MASK )
    {
        auto fse = GetNodeMaskNodeEffect( logic )->GetFullscreenEffect();
        
        if( fse->GetType() == NodeEffectType::NET_NODE_MASK )
        {
            auto paramBg = model::QueryTypedParam< model::ParamIntPtr >( fse->GetParameter( "maskIdx" ) );
            auto paramFg = model::QueryTypedParam< model::ParamIntPtr >( fse->GetParameter( "fgIdx" ) );

            auto bgIdx = paramBg->Evaluate();
            auto fgIdx = paramFg->Evaluate();

            paramBg->SetVal( fgIdx, 0.f );
            paramFg->SetVal( bgIdx, 0.f );

            // printf( "New node mask bg: %d fg: %d\n", effect->GetBackgroundChildIdx(), effect->GetForegroundChildIdx() );
        }
    }
    else
    {
        auto root = GetRootNode( logic );
        auto fse = root->GetNodeEffect()->GetFullscreenEffect();

        if( fse->GetType() == NodeEffectType::NET_MIX_CHANNELS )
        {
            SetNextMixChannelsPreset();
        }
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
            auto fse = effect->GetFullscreenEffect();

            if ( fse->GetType() == NodeEffectType::NET_ALPHA_MASK )
            {
                m_alphaMaskEffect = effect;
                m_editor->SetNodeEffect( node, m_defaultEffect );
            }
            else if( fse->GetType() == NodeEffectType::NET_DEFAULT )
            {
                assert( m_alphaMaskEffect );
                
                m_editor->SetNodeEffect( node, m_alphaMaskEffect );
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
            auto fse = effect->GetFullscreenEffect();

            if ( fse->GetType() == NodeEffectType::NET_NODE_MASK )
            {
                m_nodeMaskEffect = effect;
                m_editor->SetNodeEffect( node, m_defaultEffect );
            }
            else if( fse->GetType() == NodeEffectType::NET_DEFAULT )
            {
                assert( m_nodeMaskEffect );
                
                m_editor->SetNodeEffect( node, m_nodeMaskEffect );
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
            auto fse = effect->GetFullscreenEffect();

            if ( fse->GetType() == NodeEffectType::NET_WIREFRAME )
            {
                m_wireframeEffect = effect;
                m_editor->SetNodeEffect( node, m_defaultEffect );
                m_wireframeDisabled = true;
            }
            else if( fse->GetType() == NodeEffectType::NET_DEFAULT )
            {
                assert( m_wireframeEffect );
                
                m_editor->SetNodeEffect( node, m_wireframeEffect );
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

    if( !effect )
    {
        m_editor->SetNodeEffect( root, m_defaultEffect );
    
        effect = m_defaultEffect;
    }

    auto fse = effect->GetFullscreenEffect();

    if ( fse->GetType() == NodeEffectType::NET_MIX_CHANNELS )
    {
        m_editor->SetNodeEffect( root, m_defaultEffect );
    }
    else
    {
        assert( fse->GetType() == NodeEffectType::NET_DEFAULT );

        m_editor->SetNodeEffect( root, m_mixChannelsEffect );
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::SetNextMixChannelsPreset()
{
    auto normal = model::ModelFullscreenEffectMixChannels::GetChannelMixMask( 0, 1, 2, 3 );

    auto r_all  = model::ModelFullscreenEffectMixChannels::GetChannelMixMask( 0, 0, 0, 3 );
    auto g_all  = model::ModelFullscreenEffectMixChannels::GetChannelMixMask( 1, 1, 1, 3 );
    auto b_all  = model::ModelFullscreenEffectMixChannels::GetChannelMixMask( 2, 2, 2, 3 );
    auto a_all  = model::ModelFullscreenEffectMixChannels::GetChannelMixMask( 3, 3, 3, 3 );

    auto all_mask   = glm::vec4( 1.f, 1.f, 1.f, 1.f );
    auto r_mask     = glm::vec4( 1.f, 0.f, 0.f, 1.f );
    auto g_mask     = glm::vec4( 0.f, 1.f, 0.f, 1.f );
    auto b_mask     = glm::vec4( 0.f, 0.f, 1.f, 1.f );

    unsigned int mix_masks[]    = { normal, r_all, g_all, b_all, a_all, normal, normal, normal };
    glm::vec4 channel_masks[]   = { all_mask, all_mask, all_mask, all_mask, all_mask, r_mask, g_mask, b_mask };

    auto cur_mix_mask = int(mix_masks[ m_curMixChannelsPreset ]);
    auto cur_chn_mask = channel_masks[ m_curMixChannelsPreset ];

    m_curMixChannelsPreset = ( m_curMixChannelsPreset + 1 ) % 8;

    auto paramMix = model::QueryTypedParam< model::ParamIntPtr >( m_mixChannelsEffect->GetFullscreenEffect()->GetParameter( "channelMixMask" ) );
    auto paramMask = model::QueryTypedParam< model::ParamVec4Ptr >( m_mixChannelsEffect->GetFullscreenEffect()->GetParameter( "channelMaskMask" ) );

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
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_ALPHA_MASK, "alphamask", logic->GetGlobalTimeline() );
        m_editor->SetNodeEffect( node, newEffect );
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
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_NODE_MASK, "nodemask", logic->GetGlobalTimeline() );
        m_editor->SetNodeEffect( node, newEffect );
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
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_WIREFRAME, "wireframe", logic->GetGlobalTimeline() );
        m_editor->SetNodeEffect( node, newEffect );
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
