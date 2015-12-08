#include "BVProjectEditor.h"

#include "Engine/Models/BVProject.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/ModelSceneEditor.h"
#include "Engine/Graphics/SceneGraph/SceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"

#include "tools/Utils.h"


namespace bv {

	const std::string	BVProjectEditor::COPY_PREFIX	=		"Copy#_";
	const std::string	BVProjectEditor::COPY_REGEX		=		"^(Copy([0-9]*)_)?";
	
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
BVProjectEditor::BVProjectEditor                ( BVProject * project )
	: m_project( project )
	, m_rootNode( project->GetModelSceneRoot() )
{
    assert( project != nullptr );
    assert( project->m_renderer != nullptr );

	project->m_engineSceneRoot = BVProjectTools::BuildEngineSceneNode( QueryTyped( m_rootNode ), m_nodesMapping );
    m_engineSceneEditor = new SceneEditor( project->m_renderer, project->m_engineSceneRoot );
}

// *******************************
//
void    BVProjectEditor::AddScene       ( model::SceneModelPtr scene )
{
	auto sceneName = scene->GetName();
	if( m_project->GetScene( sceneName ) )
	{
		scene->SetName( GetScenePrefix( sceneName ) + sceneName );
	}

	m_project->AddScene( scene );

	auto sceneNode = BVProjectTools::BuildEngineSceneNode( scene->GetRootNode(), m_nodesMapping );
	m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), sceneNode );
}

// *******************************
//
void    BVProjectEditor::AddEmptyScene  ( const std::string & sceneName )
{
	AddScene( model::SceneModel::CreateEmptyScene( sceneName ) );
}

// *******************************
//
bool    BVProjectEditor::RemoveScene       ( const std::string & sceneName )
{
	auto scene = m_project->GetScene( sceneName );
	if( scene )
	{
		m_project->RemoveScene( sceneName );
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
	while( !m_project->m_sceneModelVec.empty() )
	{
		RemoveScene( m_project->m_sceneModelVec[ 0 ]->GetName() );
	}
}

// *******************************
//
bool    BVProjectEditor::DetachScene			( const std::string & sceneName )
{
	auto scene = m_project->GetScene( sceneName );
	if( scene )
	{
		m_detachedScenes.push_back( scene );
		m_project->RemoveScene( sceneName );
		return true;
	}
    return false;
}

// *******************************
//
bool    BVProjectEditor::AttachScene			( const std::string & sceneName )
{
	for( UInt32 i = 0; i < m_detachedScenes.size(); ++i )
	{
		if( m_detachedScenes[ i ]->GetName() == sceneName )
		{
			m_project->AddScene( m_detachedScenes[ i ] );
			m_detachedScenes.erase( m_detachedScenes.begin() + i );
			return true;
		}
	}
    return false;
}

// *******************************
//
void    BVProjectEditor::DeleteDetachedScenes	()
{
	m_detachedScenes.clear();
	m_detachedScenes.resize( 0 );
}

// *******************************
//
model::SceneModelPtr    BVProjectEditor::GetScene ( const std::string & sceneName )
{
	return m_project->GetScene( sceneName );
}

// *******************************
//
void    BVProjectEditor::SetSceneVisible		( const std::string & sceneName, bool visible )
{
	auto scene = m_project->GetScene( sceneName );
	if( scene )
	{
		GetEngineNode( scene->GetRootNode() )->SetVisible( visible );
	}
}

// *******************************
//
void    BVProjectEditor::RenameScene			( const std::string & sceneName, std::string newSceneName )
{
	auto scene = m_project->GetScene( sceneName );
	if( scene )
	{
		scene->SetName( newSceneName );
	}
}

// *******************************
//
model::SceneModelPtr	BVProjectEditor::AddSceneCopy		( const std::string & sceneNameToCopy )
{
	auto scene = m_project->GetScene( sceneNameToCopy );
	if( scene )
	{
		auto copy = scene->Clone();
		AddScene( copy );
		return copy;
	}
	return nullptr;
}

// *******************************
//
void    BVProjectEditor::SetSceneRootNode		( const std::string & sceneName, model::IModelNodePtr rootNode )
{
	auto scene = m_project->GetScene( sceneName );
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
	auto scene = m_project->GetScene( sceneName );
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
	auto scene = m_project->GetScene( sceneName );
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
	auto scene = m_project->GetScene( sceneName );
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
	auto scene = m_project->GetScene( sceneName );
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
	auto scene = m_project->GetScene( sceneName );
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
	auto scene = m_project->GetScene( sceneName );
	assert( scene != nullptr );

	auto sceneEditor = scene->GetModelSceneEditor();

	auto detachedNode = sceneEditor->GetDetachedNode();
    MappingsCleanup( detachedNode );

    sceneEditor->DeleteDetachedNode();
    m_engineSceneEditor->DeleteDetachedNode();
}

// *******************************
//
model::BasicNodePtr		BVProjectEditor::AddNodeCopy        ( const std::string & destSceneName, model::BasicNodePtr destParentNode, const std::string & srcSceneName, model::BasicNodePtr nodeToCopy )
{
	{ srcSceneName; }
	auto copy = nodeToCopy->GetModelNodeEditor()->CopyNode();
	if( copy )
	{
		if( destParentNode )
		{
			AddChildNode( destSceneName, destParentNode, copy );
		}
		else
		{
			SetSceneRootNode( destSceneName, copy );
		}
	}
	return copy;
}

// *******************************
//
bool					BVProjectEditor::AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, unsigned int idx )
{
	auto editor = node->GetModelNodeEditor();
	if( editor->AddPlugin( plugin, idx ) )
	{
		editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
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
		editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
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
		editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
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
		editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
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
		targetEditor->RefreshNode( GetEngineNode( targetNode ), m_project->m_renderer );
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
		editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
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
		editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
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
void					BVProjectEditor::ResetDetachedPlugin( model::BasicNodePtr node )
{
	return node->GetModelNodeEditor()->ResetDetachedPlugin();
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::AddPluginCopy			( model::BasicNodePtr destNode, model::BasicNodePtr srcNode, const std::string & pluginNameToCopy, unsigned int destIdx )
{
	auto plugin = srcNode->GetModelNodeEditor()->CopyPlugin( pluginNameToCopy );
	if( plugin )
	{
		AddPlugin( destNode, plugin, destIdx );
	}
	return plugin;
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
SceneNode *             BVProjectEditor::GetEngineNode      ( model::IModelNodePtr node )
{
    return m_nodesMapping[ node.get() ];
}

// *******************************
//
std::string				BVProjectEditor::GetScenePrefix		( const std::string & name )
{
	UInt32 num = 0;
	for( auto & scene : m_project->m_sceneModelVec )
	{
		std::smatch sm;
		auto sceneName = scene->GetName();
		if( std::regex_match( sceneName, sm, std::regex( COPY_REGEX + name  + "$" ) ) )
		{
			if( sm[ 2 ].matched && !sm[ 2 ].str().empty() )
			{
				auto ires = ( UInt32 )std::stoi( sm[ 2 ].str() );
				if( num < ires )
				{
					num = ires;
				}
			}
		}
	}

	std::string result( COPY_PREFIX );
	result.replace( result.find( "#" ), 1, num == 0 ? "" : toString( num ) );
	return result;
}

} //bv
