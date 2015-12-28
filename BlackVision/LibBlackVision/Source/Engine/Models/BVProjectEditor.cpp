#include "BVProjectEditor.h"

#include "Engine/Models/BVProject.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/ModelSceneEditor.h"
#include "Engine/Graphics/SceneGraph/SceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/AssetManager.h"

#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

#include "Engine/Models/Builder/ModelNodeEffectFactory.h"

#include "Tools/Utils.h"
#include "Tools/PrefixHelper.h"
#include "UseLoggerLibBlackVision.h"

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
        scene->SetName( PrefixSceneName( sceneName ) );
    }

    m_project->AddScene( scene );

    auto sceneRoot = scene->GetRootNode();
    if( sceneRoot )
    {
        auto sceneNode = BVProjectTools::BuildEngineSceneNode( sceneRoot, m_nodesMapping );
        m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), sceneNode );
    }
}

// *******************************
//
void    BVProjectEditor::AddScene			( const std::string & sceneName )
{
    AddScene( model::SceneModel::CreateEmptyScene( sceneName ) );
}

// *******************************
//
bool    BVProjectEditor::RemoveScene		( const std::string & sceneName )
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
        auto sceneRoot = scene->GetRootNode();
        if( sceneRoot )
        {
            m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( sceneRoot ) );
            MappingsCleanup( sceneRoot );
        }

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
    return AttachScene( sceneName, ( UInt32 )m_project->m_sceneModelVec.size() );
}


// *******************************
//
bool    BVProjectEditor::AttachScene			( const std::string & sceneName, UInt32 posIdx )
{
    for( UInt32 i = 0; i < m_detachedScenes.size(); ++i )
    {
        if( m_detachedScenes[ i ]->GetName() == sceneName )
        {
            m_project->AddScene( m_detachedScenes[ i ], posIdx );

            auto sceneRoot = m_detachedScenes[ i ]->GetRootNode();
            if( sceneRoot )
            {
                auto sceneNode = BVProjectTools::BuildEngineSceneNode( sceneRoot, m_nodesMapping );
                m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), sceneNode, posIdx );
            }

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
bool    BVProjectEditor::SetSceneVisible		( const std::string & sceneName, bool visible )
{
	auto scene = m_project->GetScene( sceneName );
	if( scene )
	{
		auto root = scene->GetRootNode();
		if( root )
		{
			root->SetVisible( visible );
			GetEngineNode( root )->SetVisible( visible );
            return true;
		}
	}
    return false;
}

// *******************************
//
bool    BVProjectEditor::RenameScene			( const std::string & sceneName, const std::string & newSceneName )
{
    auto scene = m_project->GetScene( sceneName );
    if( scene && !m_project->GetScene( newSceneName ) )
    {
        scene->SetName( newSceneName );
        return true;
    }
    return false;
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
bool	BVProjectEditor::MoveScene				( const std::string & sceneName, UInt32 posIdx )
{
	if( DetachScene( sceneName ) )
	    return AttachScene( sceneName, posIdx );
    return false;
}

// *******************************
//
void    BVProjectEditor::SetSceneRootNode		( const std::string & sceneName, model::IModelNodePtr rootNode )
{
    auto scene = m_project->GetScene( sceneName );
    if( scene )
    {
        auto oldRoot = scene->GetRootNode();
        if( oldRoot )
        {
            m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( oldRoot ) );
            MappingsCleanup( oldRoot );
        }

        auto modelNode = QueryTyped( rootNode );
        scene->GetModelSceneEditor()->SetRootNode( m_rootNode, modelNode );
        auto engineNode = BVProjectTools::BuildEngineSceneNode( modelNode, m_nodesMapping );
        m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), engineNode );
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
void    BVProjectEditor::AddChildNode         ( const std::string & sceneName, const std::string & parentPath, const std::string & newNodeName )
{
    auto newNode = model::BasicNode::Create( newNodeName, nullptr );
    auto parentNode =  GetNode( sceneName, parentPath );
    AddChildNode( sceneName, parentNode, newNode );
}

// *******************************
//
bool    BVProjectEditor::DeleteChildNode      ( const std::string & sceneName, const std::string & nodePath )
{ 
    auto parentNode =  GetParentNode( sceneName, nodePath );
    return DeleteChildNode( sceneName, parentNode, GetNodeName( nodePath ) );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, const std::string & parentPath )
{
    auto parentNode = GetNode( sceneName, parentPath );
    return AttachChildNode( sceneName, parentNode );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, const std::string & parentPath, UInt32 posIdx )
{
    auto parentNode =  GetNode( sceneName, parentPath );
    return AttachChildNode( sceneName, parentNode, posIdx );
}

// *******************************
//
bool                    BVProjectEditor::DetachChildNode     ( const std::string & sceneName, const std::string & nodeToDetachPath )
{
    auto nodeToDetach =  GetNode( sceneName, nodeToDetachPath );
    return DetachChildNode( sceneName, nodeToDetach, GetNodeName( nodeToDetachPath ) );
}

// *******************************
//
model::BasicNodePtr		BVProjectEditor::AddNodeCopy        ( const std::string & destSceneName, const std::string & destParentPath, const std::string & srcSceneName, const std::string & srcNodePath )
{
    auto destParent = QueryTyped( GetNode( destSceneName, destParentPath ) );
    auto srcNode = QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    return AddNodeCopy( destSceneName, destParent, srcSceneName, srcNode );
}

// *******************************
//
bool					BVProjectEditor::MoveNode			( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath )
{
    if( srcSceneName == destSceneName )
    {
		if( DetachChildNode( srcSceneName, srcNodePath ) )
		    return AttachChildNode( destSceneName, destNodePath, destIdx );
    }
    else
    {
        if( AddNodeCopy( destSceneName, destNodePath, srcSceneName, srcNodePath ) )
        {
			if( DeleteChildNode( srcSceneName, srcNodePath ) )
			    if( DetachChildNode( destSceneName, destNodePath ) )
			        return AttachChildNode( destSceneName, destNodePath, destIdx );
        }
    }
    return false;
}

// *******************************
//
void    BVProjectEditor::AddChildNode         ( const std::string & sceneName, model::IModelNodePtr parentNode, model::IModelNodePtr childNode )
{
    auto scene = m_project->GetScene( sceneName );
    assert( scene != nullptr );

    if( childNode )
    {
        if( parentNode )
        {
            auto engineParent = GetEngineNode( parentNode );
            auto engineChild = BVProjectTools::BuildEngineSceneNode( QueryTyped( childNode ), m_nodesMapping );

            auto sceneEditor = scene->GetModelSceneEditor();
            sceneEditor->AddChildNode( QueryTyped( parentNode ), QueryTyped( childNode ) );

            m_engineSceneEditor->AddChildNode( engineParent, engineChild );
        }
        else
        {
            SetSceneRootNode( sceneName, childNode );
        }
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
    else
    {
        DeleteSceneRootNode( sceneName );
    }

    return false;
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent )
{
    return AttachChildNode( sceneName, parent, parent->GetNumChildren() );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent, UInt32 posIdx )
{
    auto scene = m_project->GetScene( sceneName );
    assert( scene != nullptr );

    auto sceneEditor = scene->GetModelSceneEditor();
    if( parent )
    {
        auto attachModel    = sceneEditor->AttachChildNode( QueryTyped( parent ), posIdx );
        auto attachEngine   = m_engineSceneEditor->AttachChildNode( GetEngineNode( parent ), posIdx );

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
bool            BVProjectEditor::SetNodeVisible				( const std::string & sceneName, const std::string & nodePath, bool visible )
{
    auto node =  GetNode( sceneName, nodePath );
    if( node )
    {
	    node->SetVisible( visible );
	    GetEngineNode( node )->SetVisible( visible );
        return true;
    }
    return false;
}

// *******************************
//
bool            BVProjectEditor::RenameNode					( const std::string & sceneName, const std::string & nodePath, const std::string & newNodeName )
{
	auto node =  GetNode( sceneName, nodePath );
	return RenameNode( node, newNodeName );
}

// *******************************
//
model::BasicNodePtr		BVProjectEditor::AddNodeCopy        ( const std::string & destSceneName, model::BasicNodePtr destParentNode, const std::string & srcSceneName, model::BasicNodePtr srcNode )
{
    model::BasicNodePtr	copy = nullptr;
    
    if( srcSceneName == destSceneName )
    {
        //don't copy timelines, maybe it should be handled by CloneViaSerialization::ClonePlugin with empty prefix string?
        copy = srcNode->GetModelNodeEditor()->CopyNode();
    }
    else
    {
        auto destScene = m_project->GetScene( destSceneName );
        assert( destScene != nullptr );

        //copy timelines
        auto timelines = srcNode->GetTimelines();
        auto prefixNum = model::TimelineHelper::CopyTimelines( destScene->GetTimeline(), timelines );

        copy = CloneViaSerialization::CloneNode( srcNode.get(), PrefixHelper::PrefixCopy( prefixNum ) );
    }

    if( copy )
    {
        AddChildNode( destSceneName, destParentNode, copy );
    }
    return copy;
}

// *******************************
//
bool					BVProjectEditor::MoveNode			( const std::string & destSceneName, model::BasicNodePtr destParentNode, UInt32 destIdx, const std::string & srcSceneName, model::BasicNodePtr srcParentNode, const std::string & srcNodeName )
{
    if( srcSceneName == destSceneName )
    {
		if( DetachChildNode( srcSceneName, srcParentNode, srcNodeName ) )
		    return AttachChildNode( destSceneName, destParentNode, destIdx );
    }
    else
    {
        if( AddNodeCopy( destSceneName, destParentNode, srcSceneName, QueryTyped( srcParentNode->GetChild( srcNodeName ) ) ) )
        {
			if( DeleteChildNode( srcSceneName, srcParentNode, srcNodeName ) )
			    if( DetachChildNode( destSceneName, destParentNode, srcNodeName ) )
			        return AttachChildNode( destSceneName, destParentNode, destIdx );
        }
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::AddPlugin			( const std::string & sceneName, const std::string & nodePath, const std::string & pluginUID, const std::string & pluginName, const std::string & timelinePath, UInt32 idx )
{
    auto timeEval = GetTimeline( timelinePath );
    auto plugin = model::PluginsManager::DefaultInstance().CreatePlugin( pluginUID, pluginName, timeEval );
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    return AddPlugin( node, plugin, idx );
}

// *******************************
//
bool					BVProjectEditor::DeletePlugin          ( const std::string & sceneName, const std::string & nodePath, const std::string & name )
{
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    return DeletePlugin( node, name );
}

// *******************************
//
bool					BVProjectEditor::DeletePlugin          ( const std::string & sceneName, const std::string & nodePath, UInt32 pluginIdx )
{
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    return DeletePlugin( node, pluginIdx );
}

// *******************************
//
bool					BVProjectEditor::AttachPlugin          ( const std::string & sceneName, const std::string & nodePath, UInt32 posIdx )
{
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    return AttachPlugin( node, posIdx );
}

// *******************************
//
bool                    BVProjectEditor::AttachPlugin			( const std::string & destSceneName, const std::string & destNodePath, const std::string & srcSceneName, const std::string & srcNodePath, UInt32 posIdx )
{
    auto srcNode =  QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    auto destNode =  QueryTyped( GetNode( destSceneName, destNodePath ) );
    return AttachPlugin( destNode, srcNode, posIdx );
}

// *******************************
//
bool					BVProjectEditor::DetachPlugin          ( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName )
{
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    return DetachPlugin( node, pluginName );
}

// *******************************
//
bool					BVProjectEditor::DetachPlugin          ( const std::string & sceneName, const std::string & nodePath, UInt32 posIdx )
{
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    return DetachPlugin( node, posIdx );
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::GetDetachedPlugin		( const std::string & sceneName, const std::string & nodePath )
{
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    return GetDetachedPlugin( node );
}

// *******************************
//
void					BVProjectEditor::ResetDetachedPlugin	( const std::string & sceneName, const std::string & nodePath )
{
    auto node =  QueryTyped( GetNode( sceneName, nodePath ) );
    ResetDetachedPlugin( node );
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::AddPluginCopy			( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath, const std::string & pluginNameToCopy )
{
    auto srcNode =  QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    auto destNode =  QueryTyped( GetNode( destSceneName, destNodePath ) );
    
    return AddPluginCopy( destSceneName, destNode, destIdx, srcSceneName, srcNode, pluginNameToCopy );
}

// *******************************
//
bool					BVProjectEditor::MovePlugin			( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath, const std::string & pluginName )
{
    auto srcNode =  QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    auto destNode =  QueryTyped( GetNode( destSceneName, destNodePath ) );
    
    return MovePlugin( destSceneName, destNode, destIdx, srcSceneName, srcNode, pluginName );
}

// *******************************
//
bool					BVProjectEditor::AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, UInt32 idx )
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
bool					BVProjectEditor::DeletePlugin          ( model::BasicNodePtr node, UInt32 posIdx )
{
    auto editor = node->GetModelNodeEditor();
    if ( editor->DeletePlugin( posIdx ) )
    {
        editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
        return true;
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::AttachPlugin          ( model::BasicNodePtr node, UInt32 posIdx )
{
    auto editor = node->GetModelNodeEditor();
    if( editor->AttachPlugin( posIdx ) )
    {
        editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
        return true;
    }
    return false;
}

// *******************************
//
bool                    BVProjectEditor::AttachPlugin			( model::BasicNodePtr destNode, model::BasicNodePtr srcNode, UInt32 posIdx )
{
    auto sourceEditor = srcNode->GetModelNodeEditor();
    auto destEditor = destNode->GetModelNodeEditor();

    if( destEditor->AddPlugin(sourceEditor->GetDetachedPlugin(), posIdx ) )
    {
        sourceEditor->ResetDetachedPlugin();
        destEditor->RefreshNode( GetEngineNode( destNode ), m_project->m_renderer );
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
bool					BVProjectEditor::DetachPlugin          ( model::BasicNodePtr node, UInt32 posIdx )
{
    auto editor = node->GetModelNodeEditor();
    if ( editor->DetachPlugin( posIdx ) )
    {
        editor->RefreshNode( GetEngineNode( node ), m_project->m_renderer );
        return true;
    }
    return false;
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::GetDetachedPlugin		( model::BasicNodePtr node )
{
    return node->GetModelNodeEditor()->GetDetachedPlugin();
}

// *******************************
//
void					BVProjectEditor::ResetDetachedPlugin	( model::BasicNodePtr node )
{
    return node->GetModelNodeEditor()->ResetDetachedPlugin();
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::AddPluginCopy			( const std::string & destSceneName, model::BasicNodePtr destNode, UInt32 destIdx, const std::string & srcSceneName, model::BasicNodePtr srcNode, const std::string & pluginNameToCopy )
{
    model::IPluginPtr copy = nullptr;
    
    if( srcSceneName == destSceneName )
    {
        //don't copy timelines, maybe it should be handled by CloneViaSerialization::ClonePlugin with empty prefix string?
        copy = srcNode->GetModelNodeEditor()->CopyPlugin( pluginNameToCopy );
    }
    else
    {
        auto destScene = m_project->GetScene( destSceneName );
        auto srcPlugin = srcNode->GetPlugin( pluginNameToCopy );

        assert( destScene != nullptr );
        assert( srcPlugin != nullptr );

        //copy timelines
        auto timelines = srcPlugin->GetTimelines();
        auto prefixNum = model::TimelineHelper::CopyTimelines( destScene->GetTimeline(), timelines );
        
        copy = CloneViaSerialization::ClonePlugin( srcPlugin.get(), PrefixHelper::PrefixCopy( prefixNum ) );
    }

    if( copy )
    {
        AddPlugin( destNode, copy, destIdx );
    }
    return copy;
}

// *******************************
//
bool			BVProjectEditor::MovePlugin					( const std::string & destSceneName, model::BasicNodePtr destNode, UInt32 destIdx, const std::string & srcSceneName, model::BasicNodePtr srcNode, const std::string & pluginName )
{
    if( srcSceneName == destSceneName )
    {
		if( DetachPlugin( srcNode, pluginName ) )
		    return AttachPlugin( destNode, destIdx );
    }
    else
    {
        if( AddPluginCopy( destSceneName, destNode, destIdx, srcSceneName, srcNode, pluginName ) )
        {
			return DeletePlugin( srcNode, pluginName );
        }
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::LoadAsset			( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, const std::string & serializedAssetData )
{
    //not sure how to pass assets data
    JsonDeserializeObject deserializer;
    deserializer.Load( serializedAssetData );
    auto assetDesc = AssetManager::GetInstance().CreateDesc( deserializer );

    auto node = GetNode( sceneName, nodePath );
    
    model::IPluginPtr plugin = nullptr;
    if( node )
    {
        plugin = node->GetPlugin( pluginName );
    }
    
    return LoadAsset( plugin, assetDesc );
}

// *******************************
//
bool			BVProjectEditor::LoadAsset					( model::IPluginPtr plugin, AssetDescConstPtr assetDesc )
{
	if( plugin && assetDesc )
	{
		auto success = plugin->LoadResource( assetDesc );
        //FIXME: clear resources from engine
        return success;
	}

    return false;
}

// *******************************
//
void            BVProjectEditor::SetNodeVisible				( model::IModelNodePtr node, bool visible )
{
    node->SetVisible( visible );
    m_nodesMapping[ node.get() ]->SetVisible( visible );
}

// *******************************
//
bool            BVProjectEditor::RenameNode					( model::IModelNodePtr node, const std::string & newNodeName )
{
    if( node = nullptr )
        return false;

	QueryTyped( node )->SetName( newNodeName );
    return true;
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

// ***********************
//
bool                        BVProjectEditor::SetNodeEffect   ( const std::string& sceneName, const std::string& nodePath, const std::string& timelinePath, const std::string& effectName )
{
    auto timeEvaluator = GetTimeline( timelinePath );
    auto node = GetNode( sceneName, nodePath );

    if( !node || !timeEvaluator )
        return false;

    auto newEffect = model::ModelNodeEffectFactory::Create( effectName, timeEvaluator );

    if( !newEffect )
        return false;

    SetNodeEffect( node, newEffect );
    return true;
}

// *******************************
//
void						BVProjectEditor::AddTimeline			( const std::string & sceneName, const std::string & timelineName, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    auto timeline = model::TimelineHelper::CreateDefaultTimeline( timelineName, duration, preMethod, postMethod );
    AddTimeline( sceneName, timeline );
}

// *******************************
//
void						BVProjectEditor::AddTimeline			( const std::string & sceneName, model::ITimeEvaluatorPtr timeline )
{
    auto scene = m_project->GetScene( sceneName );
    if( scene )
    {
        //FIXME: duplicated names, how to handle?
        scene->GetTimeline()->AddChild( timeline );
    }
}

// *******************************
//
bool						BVProjectEditor::DeleteTimeline			( const std::string & timelinePath )
{
    auto scene = m_project->GetScene( model::TimelineHelper::GetParentNodePath( timelinePath ) );
    auto timeEval = model::TimelineManager::GetInstance()->GetTimeEvaluator( timelinePath );
    
    assert( scene && timeEval );
    
    if( timeEval.use_count() == 2 ) //FIXME: maybe it's more safe to go through node tree..
    {
        model::TimelineManager::GetInstance()->RemoveTimelineFromTimeline( timelinePath, model::TimelineHelper::GetParentNodePath( timelinePath ) );
        return true;
    }
    return false;
}

// *******************************
//
void						BVProjectEditor::ForceDeleteTimeline	( const std::string & timelinePath, const std::string & newTimelinePath )
{
    auto tm = model::TimelineManager::GetInstance();

    auto sceneName = model::TimelineHelper::GetParentNodePath( timelinePath );
    auto timeEval = tm->GetTimeEvaluator( timelinePath );

    auto newTimeEval = tm->GetTimeEvaluator( sceneName );
    if( !newTimelinePath.empty() )
    {
        newTimeEval = tm->GetTimeEvaluator( newTimelinePath );
    }
    assert( timeEval );
    assert( newTimeEval );
    
    auto scene = m_project->GetScene( sceneName );
    auto sceneRoot = scene->GetRootNode();
    assert( scene );
    assert( sceneRoot );

    sceneRoot->GetModelNodeEditor()->ReplaceTimeline( timeEval, newTimeEval );

    tm->RemoveTimelineFromTimeline( timelinePath, model::TimelineHelper::GetParentNodePath( timelinePath ) );
    
    assert( timeEval.use_count() == 1 );
}
    
// *******************************
//
bool						BVProjectEditor::RenameTimeline			( const std::string & timelinePath, const std::string & newName )
{
    auto timelineManager = model::TimelineManager::GetInstance();
    auto timeline = timelineManager->GetTimeline( timelinePath );
    if( timeline && !timelineManager->GetTimeline( newName ) )
    {
        timeline->SetName( newName );
        return true;
    }
    return false;
}

// *******************************
//
void						BVProjectEditor::SetTimelineDuration			( const std::string & timelinePath, TimeType duration )
{
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( timelinePath );
    assert( timeline );
    timeline->SetDuration( duration );
}

// *******************************
//
void						BVProjectEditor::SetTimelineWrapPreBehavior		( const std::string & timelinePath, TimelineWrapMethod preMethod )
{
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( timelinePath );
    assert( timeline );
    timeline->SetWrapBehavior( preMethod, timeline->GetWrapBehaviorPost() );
}

// *******************************
//
void						BVProjectEditor::SetTimelineWrapPostBehavior	( const std::string & timelinePath, TimelineWrapMethod postMethod )
{
    auto timeline = model::TimelineManager::GetInstance()->GetTimeline( timelinePath );
    assert( timeline );
    timeline->SetWrapBehavior( timeline->GetWrapBehaviorPre(), postMethod );
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
model::IModelNodePtr	BVProjectEditor::GetNode			( const std::string & sceneName, const std::string & nodePath )
{
    auto scene = m_project->GetScene( sceneName );
    if( scene )
    {
        auto node = scene->GetRootNode()->GetNode( nodePath );
        if( node )
        {
            return node;
        }
    }
    LOG_MESSAGE( SeverityLevel::error ) << "Node [" + nodePath + "] not found";
    return nullptr;
}

// ***********************
//
model::IModelNodePtr	BVProjectEditor::GetParentNode		( const std::string & sceneName, const std::string & nodePath )
{
    auto parentNodePath = nodePath.substr( 0, nodePath.find_last_of("/") );

    auto scene = m_project->GetScene( sceneName );
    if( scene )
    {
        auto node = scene->GetRootNode()->GetNode( parentNodePath );
        if( node )
        {
            return node;
        }
    }
    LOG_MESSAGE( SeverityLevel::error ) << "Node [" + parentNodePath + "] not found";
    return nullptr;
}

// ***********************
//
model::ITimeEvaluatorPtr	BVProjectEditor::GetTimeline	( const std::string & timelinePath )
{
    return model::TimelineManager::GetInstance()->GetTimeEvaluator( timelinePath );
}

// ***********************
//
std::string				BVProjectEditor::GetNodeName		( const std::string & nodePath )
{
    return nodePath.substr( nodePath.find_last_of( "/" ) + 1, nodePath.length() );
}

// *******************************
//
SceneNode *             BVProjectEditor::GetEngineNode      ( model::IModelNodePtr node )
{
    return m_nodesMapping[ node.get() ];
}

// *******************************
//
std::string				BVProjectEditor::PrefixSceneName		( const std::string & name )
{
    Int32 num = -1;
    for( auto & scene : m_project->m_sceneModelVec )
    {
        auto res = PrefixHelper::MatchNames( name, scene->GetName() );
        if( num < res )
        {
            num = res;
        }
    }

    return PrefixHelper::ReplacePrefix( name, num + 1 );
}

} //bv
