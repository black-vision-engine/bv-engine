#include "TestGlobalEffectKeyboardHandler.h"

#include "BVAppLogic.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectDefault.h"

#include "Serialization/BV/XML/XMLSerializer.h"
#include "Assets/AssetDescsWithUIDs.h"

namespace bv {

// *********************************
//
TestGlobalEfectKeyboardHandler::TestGlobalEfectKeyboardHandler  ()
{
    m_defaultEffect = nullptr;
    m_alphaMaskEffect = nullptr;
    m_nodeMaskEffect = nullptr;
    m_curWireframeNodeIdx = 0;
    m_wireframeDisabled = true;
	m_editor = nullptr;
}

// *********************************
//
void    TestGlobalEfectKeyboardHandler::HandleKey( unsigned char c, BVAppLogic * logic )
{
	if( !m_editor ) 
	{
		m_editor = logic->GetBVProject()->GetProjectEditor();
	}

    if( !m_defaultEffect )
    {
        m_defaultEffect = std::make_shared< model::ModelNodeEffectDefault >( model::TimelineManager::GetInstance()->GetRootTimeline() );
    }

    switch( c )
    {
        case 's': 
        {
            auto sob = new XMLSerializer();
            logic->GetBVProject()->GetScenes()[ 0 ]->Serialize( *sob );
            sob->Save( "test.xml" );
            delete sob; 

            break;
        }
        case 'p':
        {
            XMLSerializer ser;
            
            auto node = GetVanillaNode( logic )->GetChild( "vanilla_tex" );

            TimelineManager::GetInstance()->Serialize( ser );

            ////auto& assets = AssetDescsWithUIDs::GetInstance(); // & is crucial!
            ////GetAssetsWithUIDs( assets, std::dynamic_pointer_cast< model::BasicNode >( node ) );
            //
            //assets.Serialize( *ser );

            auto plugin_ = node->GetPlugin( "texture" );
            auto plugin = std::dynamic_pointer_cast< model::BasePlugin< model::IPlugin > >( plugin_ );
            plugin->Serialize( ser );

            ser.Save( "plugin.xml" );
            break;
        }
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

			m_editor->SetNodeEffect( curNode, m_defaultEffect );
            auto newEffect = std::make_shared< model::ModelNodeEffectWireframe >( model::TimelineManager::GetInstance()->GetRootTimeline() );
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

			m_editor->SetNodeEffect( curNode, m_defaultEffect );
            auto newEffect = std::make_shared< model::ModelNodeEffectWireframe >( model::TimelineManager::GetInstance()->GetRootTimeline() );
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
				m_editor->SetNodeEffect( node, m_defaultEffect );
            }
            else if( effect->GetType() == NodeEffectType::NET_DEFAULT )
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
            if ( effect->GetType() == NodeEffectType::NET_NODE_MASK )
            {
                m_nodeMaskEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( effect );
				m_editor->SetNodeEffect( node, m_defaultEffect );
            }
            else if( effect->GetType() == NodeEffectType::NET_DEFAULT )
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
            if ( effect->GetType() == NodeEffectType::NET_WIREFRAME )
            {
                m_wireframeEffect = std::static_pointer_cast< model::ModelNodeEffectWireframe >( effect );
				m_editor->SetNodeEffect( node, m_defaultEffect );
                m_wireframeDisabled = true;
            }
            else if( effect->GetType() == NodeEffectType::NET_DEFAULT )
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
model::BasicNodePtr     TestGlobalEfectKeyboardHandler::GetRootNode         ( BVAppLogic * logic )
{
    auto scene = logic->GetBVProject();
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
        auto newEffect = std::make_shared< model::ModelNodeEffectAlphaMask >( model::TimelineManager::GetInstance()->GetRootTimeline() );
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
        auto newEffect = std::make_shared< model::ModelNodeEffectNodeMask >( model::TimelineManager::GetInstance()->GetRootTimeline() );
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
        auto newEffect = std::make_shared< model::ModelNodeEffectWireframe >( model::TimelineManager::GetInstance()->GetRootTimeline() );
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
