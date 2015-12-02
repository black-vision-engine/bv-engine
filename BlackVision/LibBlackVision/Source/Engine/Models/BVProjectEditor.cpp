#include "BVProjectEditor.h"

#include "Engine/Models/BVProject.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/ModelSceneEditor.h"
#include "Engine/Graphics/SceneGraph/SceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"

namespace bv {
    
namespace {
// *******************************
//
model::BasicNodePtr QueryTyped( model::IModelNodePtr node )
{
    return std::static_pointer_cast< model::BasicNode >( node );
}

} // anonymous

// *******************************
//
BVProjectEditor::BVProjectEditor                ( BVProject * scene )
	: m_scene( scene )
	, m_rootNode( scene->GetModelSceneRoot() )
{
    assert( scene != nullptr );
    assert( scene->m_renderer != nullptr );

	scene->m_engineSceneRoot = BVProjectTools::BuildEngineSceneNode( QueryTyped( m_rootNode ), m_nodesMapping );
    m_engineSceneEditor = new SceneEditor( scene->m_renderer, scene->m_engineSceneRoot );
}

// *******************************
//
void    BVProjectEditor::AddScene       ( model::SceneModelPtr scene )
{
	if( !m_scene->GetScene( scene->GetName() ) )
	{
		m_scene->AddScene( scene );
		auto sceneNode = BVProjectTools::BuildEngineSceneNode( scene->GetRootNode(), m_nodesMapping );
		m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), sceneNode );
	}
}

// *******************************
//
bool    BVProjectEditor::RemoveScene       ( const std::string & sceneName )
{
	auto scene = m_scene->GetScene( sceneName );
	if( scene )
	{
		m_scene->RemoveScene( sceneName );
		m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( scene->GetRootNode() ) );
		MappingsCleanup( scene->GetRootNode() );
		return true;
	}
    return false;
}

// *******************************
//
void    BVProjectEditor::RemoveAllScenes		()
{
	while( !m_scene->m_sceneModelVec.empty() )
	{
		RemoveScene( m_scene->m_sceneModelVec[ 0 ]->GetName() );
	}
}

// *******************************
//
model::SceneModelPtr    BVProjectEditor::GetScene ( const std::string & sceneName )
{
	return m_scene->GetScene( sceneName );
}

// *******************************
//
void    BVProjectEditor::SetSceneVisible       ( const std::string & sceneName, bool visible )
{
	auto scene = m_scene->GetScene( sceneName );
	if( scene )
	{
		GetEngineNode( scene->GetRootNode() )->SetVisible( visible );
	}
}

// *******************************
//
model::SceneModelPtr    BVProjectEditor::CopyScene  ( const std::string & sceneName )
{
	auto scene = m_scene->GetScene( sceneName );
	if( scene )
	{
		return scene->Clone();
	}
	return nullptr;
}

// *******************************
//
void    BVProjectEditor::SetSceneRootNode     ( const std::string & sceneName, model::IModelNodePtr rootNode )
{
	auto scene = m_scene->GetScene( sceneName );
	if( scene )
	{
		auto oldRoot = scene->GetRootNode();
		m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( oldRoot ) );
		MappingsCleanup( oldRoot );

		scene->GetModelSceneEditor()->SetRootNode( m_rootNode, QueryTyped( rootNode ) );
		m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), BVProjectTools::BuildEngineSceneNode( scene->GetRootNode(), m_nodesMapping ) );
	}
}

// *******************************
//
bool    BVProjectEditor::DeleteSceneRootNode	( const std::string & sceneName )
{
	auto scene = m_scene->GetScene( sceneName );
	if( scene )
	{
		auto root = scene->GetRootNode();
		m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( root ) );
		MappingsCleanup( root );

		scene->GetModelSceneEditor()->SetRootNode( m_rootNode, nullptr );
	}
	return false;
}

// *******************************
//
void    BVProjectEditor::AddChildNode         ( const std::string & sceneName, model::IModelNodePtr parentNode, model::IModelNodePtr childNode )
{
	auto scene = m_scene->GetScene( sceneName );
	assert( scene != nullptr );

    if( parentNode &&  childNode )
    {
        auto engineParent = GetEngineNode( parentNode );
        auto engineChild = BVProjectTools::BuildEngineSceneNode( QueryTyped( childNode ), m_nodesMapping );

		auto sceneEditor = scene->GetModelSceneEditor();
		sceneEditor->AddChildNode( QueryTyped( parentNode ), QueryTyped( childNode ) );

        m_engineSceneEditor->AddChildNode( engineParent, engineChild );
    }
}

// *******************************
//
bool    BVProjectEditor::DeleteChildNode      ( const std::string & sceneName, model::IModelNodePtr parentNode, const std::string & childNodeName )
{    
	auto scene = m_scene->GetScene( sceneName );
	assert( scene != nullptr );

    if( parentNode )
    {
        auto modelParentNode = QueryTyped( parentNode );
        auto modelChildNode = QueryTyped( modelParentNode->GetChild( childNodeName ) );

        if( modelChildNode )
        {
			auto sceneEditor = scene->GetModelSceneEditor();
			sceneEditor->DeleteChildNode( modelParentNode, childNodeName );

            m_engineSceneEditor->DeleteChildNode( GetEngineNode( parentNode ), GetEngineNode( modelChildNode ) );
            
			MappingsCleanup( modelChildNode );

            return true;
        }
    }

    return false;
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent )
{
	auto scene = m_scene->GetScene( sceneName );
	assert( scene != nullptr );

	auto sceneEditor = scene->GetModelSceneEditor();
    if( parent )
    {
        auto attachModel    = sceneEditor->AttachChildNode( QueryTyped( parent ) );
        auto attachEngine   = m_engineSceneEditor->AttachChildNode( GetEngineNode( parent ) );

        return attachModel && attachEngine;
    }

    return false;
}

// *******************************
//
bool                    BVProjectEditor::DetachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent, const std::string & nodeToDetach )
{
	auto scene = m_scene->GetScene( sceneName );
	assert( scene != nullptr );

    if( parent )
    {
        auto childNode = parent->GetChild( nodeToDetach );

        if( childNode )
        {
			auto sceneEditor = scene->GetModelSceneEditor();
			MappingsCleanup( sceneEditor->GetDetachedNode() );

            auto detachModel    = sceneEditor->DetachChildNode( QueryTyped( parent ), nodeToDetach );
            auto detachEngine   = m_engineSceneEditor->DetachChildNode( GetEngineNode( parent ), GetEngineNode( childNode ) );
        
            return detachModel && detachEngine;
        }
    }

    return false;
}

// *******************************
//
void            BVProjectEditor::DeleteDetachedNodes          ( const std::string & sceneName )
{
	auto scene = m_scene->GetScene( sceneName );
	assert( scene != nullptr );

	auto sceneEditor = scene->GetModelSceneEditor();

	auto detachedNode = sceneEditor->GetDetachedNode();
    MappingsCleanup( detachedNode );

    sceneEditor->DeleteDetachedNode();
    m_engineSceneEditor->DeleteDetachedNode();
}

// *******************************
//
model::BasicNodePtr    BVProjectEditor::CopyNode          ( model::BasicNodePtr node )
{
	return node->GetModelNodeEditor()->CopyNode();
}

// *******************************
//
bool					BVProjectEditor::AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, unsigned int idx )
{
	auto editor = node->GetModelNodeEditor();
	if( editor->AddPlugin( plugin, idx ) )
	{
		editor->RefreshNode( GetEngineNode( node ), m_scene->m_renderer );
		return true;
	}
	return false;
}

// *******************************
//
bool					BVProjectEditor::DeletePlugin          ( model::BasicNodePtr node, const std::string & name )
{
	auto editor = node->GetModelNodeEditor();
	if ( editor->DeletePlugin( name ) )
	{
		editor->RefreshNode( GetEngineNode( node ), m_scene->m_renderer );
		return true;
	}
	return false;
}

// *******************************
//
bool					BVProjectEditor::DeletePlugin          ( model::BasicNodePtr node, unsigned int idx )
{
	auto editor = node->GetModelNodeEditor();
	if ( editor->DeletePlugin( idx ) )
	{
		editor->RefreshNode( GetEngineNode( node ), m_scene->m_renderer );
		return true;
	}
	return false;
}

// *******************************
//
bool					BVProjectEditor::AttachPlugin          ( model::BasicNodePtr node, unsigned int idx )
{
	auto editor = node->GetModelNodeEditor();
	if( editor->AttachPlugin(idx) )
	{
		editor->RefreshNode( GetEngineNode( node ), m_scene->m_renderer );
		return true;
	}
	return false;
}

// *******************************
//
bool                    BVProjectEditor::AttachPlugin			( model::BasicNodePtr sourceNode, model::BasicNodePtr targetNode, unsigned int idx )
{
	auto sourceEditor = sourceNode->GetModelNodeEditor();
	auto targetEditor = targetNode->GetModelNodeEditor();

	if( targetEditor->AddPlugin(sourceEditor->GetDetachedPlugin(), idx) )
	{
		sourceEditor->ResetDetachedPlugin();
		targetEditor->RefreshNode( GetEngineNode( targetNode ), m_scene->m_renderer );
		return true;
	}
	return false;
}

// *******************************
//
bool					BVProjectEditor::DetachPlugin          ( model::BasicNodePtr node, const std::string & name )
{
	auto editor = node->GetModelNodeEditor();
	if ( editor->DetachPlugin( name ) )
	{
		editor->RefreshNode( GetEngineNode( node ), m_scene->m_renderer );
		return true;
	}
	return false;
}

// *******************************
//
bool					BVProjectEditor::DetachPlugin          ( model::BasicNodePtr node, unsigned int idx )
{
	auto editor = node->GetModelNodeEditor();
	if ( editor->DetachPlugin( idx ) )
	{
		editor->RefreshNode( GetEngineNode( node ), m_scene->m_renderer );
		return true;
	}
	return false;
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::GetDetachedPlugin    ( model::BasicNodePtr node )
{
	return node->GetModelNodeEditor()->GetDetachedPlugin();
}

// *******************************
//
void					BVProjectEditor::ResetDetachedPlugin  ( model::BasicNodePtr node )
{
	return node->GetModelNodeEditor()->ResetDetachedPlugin();
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::CopyPlugin				( model::BasicNodePtr node, const std::string & name )
{
	return node->GetModelNodeEditor()->CopyPlugin( name );
}

// *******************************
//
model::IModelNodeEffectPtr	BVProjectEditor::GetNodeEffect	( model::IModelNodePtr node )
{
	return QueryTyped( node )->GetNodeEffect();
}

// *******************************
//
void						BVProjectEditor::SetNodeEffect	( model::IModelNodePtr node, model::IModelNodeEffectPtr nodeEffect )
{
    auto modelNode = QueryTyped( node );
	modelNode->SetNodeEffect( nodeEffect );
	BVProjectTools::UpdateSceneNodeEffect( GetEngineNode( node ), modelNode );
}

// *******************************
//
void                    BVProjectEditor::MappingsCleanup      ( model::IModelNodePtr node )
{
    if( node )
    {
        RemoveNodeMapping( node );
        UnregisterUpdaters( node );
    }
}

// *******************************
//
void                    BVProjectEditor::RemoveNodeMapping    ( model::IModelNodePtr node )
{
    assert( m_nodesMapping.find( node.get() ) != m_nodesMapping.end() );
    m_nodesMapping.erase( node.get() );
    
    auto modelNode = QueryTyped( node );

    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto child = modelNode->GetChild( i );

        RemoveNodeMapping( child );
    }
}

// *******************************
//
void                    BVProjectEditor::UnregisterUpdaters   ( model::IModelNodePtr node )
{
    UpdatersManager::Get().RemoveNodeUpdater( node.get() );

    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto child = QueryTyped( node )->GetChild( i );

        UnregisterUpdaters( child );
    }
}

// *******************************
//
SceneNode *             BVProjectEditor::GetEngineNode        ( model::IModelNodePtr node )
{
    return m_nodesMapping[ node.get() ];
}

} //bv
