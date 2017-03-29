#include "stdafxBVApp.h"

#include "TestGlobalEffectKeyboardHandler.h"

#include "BVAppLogic.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectFactory.h"

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
    m_timeEval = nullptr;
}

// *********************************
//
void    TestGlobalEfectKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
    if( !m_editor ) 
	{
        m_editor = logic->GetBVProject()->GetProjectEditor();
        m_timeEval = model::TimelineManager::GetInstance()->GetRootTimeline();
    }

    if( !m_defaultEffect )
    {
        m_defaultEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( nrl::NNodeEffectType::NNET_DEFAULT, "default", m_timeEval );
    }

    switch( c )
    {
        case '1':
        {
            m_curSelectedNode = nrl::NNodeEffectType::NNET_DEFAULT;

            GetVanillaNode( logic )->SetVisible( true );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( false );
            GetWireframeNode( logic )->SetVisible( false );

            break;
        }
        case '2':
        {
            m_curSelectedNode = nrl::NNodeEffectType::NNET_ALPHA_MASK;

            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( true );
            GetNodeMaskNode( logic )->SetVisible( false );
            GetWireframeNode( logic )->SetVisible( false );

            break;
        }
        case '3':
        {
            m_curSelectedNode = nrl::NNodeEffectType::NNET_NODE_MASK;

            GetVanillaNode( logic )->SetVisible( false );
            GetAlphaMaskNode( logic )->SetVisible( false );
            GetNodeMaskNode( logic )->SetVisible( true );
            GetWireframeNode( logic )->SetVisible( false );

            break;
        }
        case '4':
        {
            m_curSelectedNode = nrl::NNodeEffectType::NNET_WIREFRAME;

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
    if ( m_curSelectedNode == nrl::NNodeEffectType::NNET_ALPHA_MASK )
    {
        auto effect = GetAlphaMaskNodeEffect( logic );

        if( effect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( effect->GetParameter( "alpha" ) );

            alpha->SetVal( std::min( 1.f, alpha->Evaluate() + .1f ), 0.f );

            printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if ( m_curSelectedNode == nrl::NNodeEffectType::NNET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );

        if( effect->GetType() == nrl::NNodeEffectType::NNET_NODE_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( effect->GetParameter( "alpha" ) );

            alpha->SetVal( std::min( 1.f, alpha->Evaluate() + .1f ), 0.f );

            printf( "New node mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if( m_curSelectedNode == nrl::NNodeEffectType::NNET_WIREFRAME )
    {
        if( !m_wireframeDisabled )
        {
            auto curIdx = m_curWireframeNodeIdx;

            m_curWireframeNodeIdx = ( curIdx + 1 ) % 3;

            auto curNode = GetWireframeNode( logic, curIdx );
            auto nextNode = GetWireframeNode( logic, m_curWireframeNodeIdx );

            m_editor->SetNodeEffect( curNode, m_defaultEffect );
            auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( nrl::NNodeEffectType::NNET_WIREFRAME, "wireframe", m_timeEval );
            m_editor->SetNodeEffect( nextNode, newEffect );
        }        
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleDecrement     ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == nrl::NNodeEffectType::NNET_ALPHA_MASK )
    {
        auto effect = GetAlphaMaskNodeEffect( logic );

        if( effect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( effect->GetParameter( "alpha" ) );

            alpha->SetVal( std::max( 0.f, alpha->Evaluate() - .1f ), 0.f );

            printf( "New alpha mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if ( m_curSelectedNode == nrl::NNodeEffectType::NNET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );

        if( effect->GetType() == nrl::NNodeEffectType::NNET_NODE_MASK )
        {
            auto alpha = model::QueryTypedParam< model::ParamFloatPtr >( effect->GetParameter( "alpha" ) );

            alpha->SetVal( std::max( 0.f, alpha->Evaluate() - .1f ), 0.f );

            printf( "New node mask alpha value: %4f\n", alpha->Evaluate() );
        }
    }
    else if( m_curSelectedNode == nrl::NNodeEffectType::NNET_WIREFRAME )
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
            auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( nrl::NNodeEffectType::NNET_WIREFRAME, "wireframe", m_timeEval );
            m_editor->SetNodeEffect( nextNode, newEffect );
        }
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::HandleSpace         ( BVAppLogic * logic )
{
    if ( m_curSelectedNode == nrl::NNodeEffectType::NNET_NODE_MASK )
    {
        auto effect = GetNodeMaskNodeEffect( logic );
        
        if( effect->GetType() == nrl::NNodeEffectType::NNET_NODE_MASK )
        {
            auto paramBg = model::QueryTypedParam< model::ParamIntPtr >( effect->GetParameter( "maskIdx" ) );
            auto paramFg = model::QueryTypedParam< model::ParamIntPtr >( effect->GetParameter( "fgIdx" ) );

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
        auto effect = root->GetNodeEffect();

        if( effect->GetType() == nrl::NNodeEffectType::NNET_MIX_CHANNELS )
        {
            SetNextMixChannelsPreset();
        }
    }
}

// *********************************
//FIXME: implement
void                    TestGlobalEfectKeyboardHandler::HandleToggleEffect  ( BVAppLogic * logic )
{
    if( m_curSelectedNode == nrl::NNodeEffectType::NNET_ALPHA_MASK )
    {
        auto node = GetAlphaMaskNode( logic );
        auto effect = GetAlphaMaskNodeEffect( logic );

        if ( effect )
        {
            if ( effect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK )
            {
                m_alphaMaskEffect = effect;
                m_editor->SetNodeEffect( node, m_defaultEffect );
            }
            else if( effect->GetType() == nrl::NNodeEffectType::NNET_DEFAULT )
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
    else if ( m_curSelectedNode == nrl::NNodeEffectType::NNET_NODE_MASK )
    {
        auto node = GetNodeMaskNode( logic );
        auto effect = GetNodeMaskNodeEffect( logic );

        if ( effect )
        {
            if ( effect->GetType() == nrl::NNodeEffectType::NNET_NODE_MASK )
            {
                m_nodeMaskEffect = effect;
                m_editor->SetNodeEffect( node, m_defaultEffect );
            }
            else if( effect->GetType() == nrl::NNodeEffectType::NNET_DEFAULT )
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
    else if( m_curSelectedNode == nrl::NNodeEffectType::NNET_WIREFRAME )
    {
        auto node = GetWireframeNode( logic, m_curWireframeNodeIdx );
        auto effect = GetNodeWireframeEffect( logic, m_curWireframeNodeIdx );

        if ( effect )
        {
            if ( effect->GetType() == nrl::NNodeEffectType::NNET_WIREFRAME )
            {
                m_wireframeEffect = effect;
                m_editor->SetNodeEffect( node, m_defaultEffect );
                m_wireframeDisabled = true;
            }
            else if( effect->GetType() == nrl::NNodeEffectType::NNET_DEFAULT )
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

    if ( effect->GetType() == nrl::NNodeEffectType::NNET_MIX_CHANNELS )
    {
        m_editor->SetNodeEffect( root, m_defaultEffect );
    }
    else
    {
        assert( effect->GetType() == nrl::NNodeEffectType::NNET_DEFAULT );

        if( !m_mixChannelsEffect )
        {
            m_mixChannelsEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( nrl::NNodeEffectType::NNET_MIX_CHANNELS, "mix channels", m_timeEval );
        }

        m_editor->SetNodeEffect( root, m_mixChannelsEffect );
    }
}

// *********************************
//
void                    TestGlobalEfectKeyboardHandler::SetNextMixChannelsPreset()
{
    // FIXME: nrl - remove or reimplement?
    assert( false );
    //auto normal = model::ModelNodeEffectMixChannels::GetChannelMixMask( 0, 1, 2, 3 );

    //auto r_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 0, 0, 0, 3 );
    //auto g_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 1, 1, 1, 3 );
    //auto b_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 2, 2, 2, 3 );
    //auto a_all  = model::ModelNodeEffectMixChannels::GetChannelMixMask( 3, 3, 3, 3 );

    //unsigned int mix_masks[]    = { normal, r_all, g_all, b_all, a_all, normal, normal, normal };

    //auto cur_mix_mask = int(mix_masks[ m_curMixChannelsPreset ]);

    //m_curMixChannelsPreset = ( m_curMixChannelsPreset + 1 ) % 8;

    //auto paramMask = model::QueryTypedParam< model::ParamIntPtr >( m_mixChannelsEffect->GetParameter( "channelMask" ) );

    //paramMask->SetVal( cur_mix_mask, 0.f );
}

// *********************************
//
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetRootNode         ( BVAppLogic * logic )
{
    return logic->GetBVProject()->GetModelSceneRoot();
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

    if (!effect) // || effect->GetType() != nrl::NNodeEffectType::NNET_ALPHA_MASK )
    {
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( nrl::NNodeEffectType::NNET_ALPHA_MASK, "alphamask", m_timeEval );
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

    if (!effect) // || effect->GetType() != nrl::NNodeEffectType::NNET_NODE_MASK )
    {
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( nrl::NNodeEffectType::NNET_NODE_MASK, "nodemask", m_timeEval );
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

    if (!effect) // || effect->GetType() != nrl::NNodeEffectType::NNET_WIREFRAME )
    {
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( nrl::NNodeEffectType::NNET_WIREFRAME, "wireframe", m_timeEval );
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
