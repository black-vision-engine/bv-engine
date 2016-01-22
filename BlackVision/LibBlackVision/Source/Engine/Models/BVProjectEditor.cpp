#include "BVProjectEditor.h"

#include "Engine/Models/BVProject.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneEditor.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"

#include "Engine/Models/ModelNodeEditor.h"
#include "Engine/Models/ModelSceneEditor.h"

#include "Engine/Models/Builder/ModelNodeEffectFactory.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Serialization/Json/JsonDeserializeObject.h"

#include "Assets/AssetManager.h"

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

    InitDefaultScene( scene );

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
    return RemoveScene( scene );
}

// *******************************
//
bool    BVProjectEditor::RemoveScene		( model::SceneModelPtr scene )
{
    if( scene )
    {
        m_project->RemoveScene( scene );
        
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
        RemoveScene( m_project->m_sceneModelVec[ 0 ] );
    }
}

// *******************************
//
bool    BVProjectEditor::DetachScene			( const std::string & sceneName )
{
    auto scene = m_project->GetScene( sceneName );
    if( scene )
    {
        m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( scene->GetRootNode() ) );
        MappingsCleanup( scene->GetRootNode() );

        m_detachedScenes.push_back( scene );
        m_project->RemoveScene( scene );

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
}

// *******************************
//
model::SceneModelPtr    BVProjectEditor::GetScene ( const std::string & sceneName ) const
{
    return m_project->GetScene( sceneName );
}

// *******************************
//
bool    BVProjectEditor::SetSceneVisible		( const std::string & sceneName, bool visible )
{
	auto scene = m_project->GetScene( sceneName );
	if( scene && scene->GetRootNode() )
	{
		auto root = scene->GetRootNode();

		root->SetVisible( visible );
		GetEngineNode( root )->SetVisible( visible );

        return true;
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
        auto copy = model::SceneModelPtr( scene->Clone() );
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
    {
	    return AttachScene( sceneName, posIdx );
    }

    return false;
}

// *******************************
//
void    BVProjectEditor::SetSceneRootNode		( model::SceneModelPtr scene, model::IModelNodePtr rootNode )
{
    assert( scene );
    assert( rootNode );

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

// *******************************
//
void    BVProjectEditor::DeleteSceneRootNode	( model::SceneModelPtr scene )
{
    assert( scene );

    auto root = scene->GetRootNode();

    m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( root ) );
    MappingsCleanup( root );

    scene->GetModelSceneEditor()->SetRootNode( m_rootNode, nullptr );
}

// *******************************
//
bool    BVProjectEditor::AddChildNode         ( const std::string & sceneName, const std::string & parentPath, const std::string & newNodeName )
{
    auto newNode = model::BasicNode::Create( newNodeName, nullptr );
    auto parentNode = GetNode( sceneName, parentPath );
    auto scene = m_project->GetScene( sceneName );

    return AddChildNode( scene, parentNode, newNode );
}

// *******************************
//
bool    BVProjectEditor::DeleteChildNode      ( const std::string & sceneName, const std::string & nodePath )
{ 
    auto parentNode =  GetParentNode( sceneName, nodePath );
    auto node =  GetNode( sceneName, nodePath );
    auto scene = m_project->GetScene( sceneName );

    return DeleteChildNode( scene, parentNode, node );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, const std::string & parentPath )
{
    auto parentNode = GetNode( sceneName, parentPath );
    auto scene = m_project->GetScene( sceneName );
    return AttachChildNode( scene, parentNode );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, const std::string & parentPath, UInt32 posIdx )
{
    auto parentNode =  GetNode( sceneName, parentPath );
    auto scene = m_project->GetScene( sceneName );
    return AttachChildNode( scene, parentNode, posIdx );
}

// *******************************
//
bool                    BVProjectEditor::DetachChildNode     ( const std::string & sceneName, const std::string & nodeToDetachPath )
{
    auto parentNode =  GetParentNode( sceneName, nodeToDetachPath );
    auto nodeToDetach =  GetNode( sceneName, nodeToDetachPath );
    auto scene = m_project->GetScene( sceneName );
    return DetachChildNode( scene, parentNode, nodeToDetach );
}

// *******************************
//
model::BasicNodePtr		BVProjectEditor::AddNodeCopy        ( const std::string & destSceneName, const std::string & destParentPath, const std::string & srcSceneName, const std::string & srcNodePath )
{
    auto destParent = QueryTyped( GetNode( destSceneName, destParentPath ) );
    auto srcNode = QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    auto destScene = m_project->GetScene( destSceneName );
    auto srcScene = m_project->GetScene( srcSceneName );
    return AddNodeCopy( destScene, destParent, srcScene, srcNode );
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
			DeleteChildNode( srcSceneName, srcNodePath );
            DetachChildNode( destSceneName, destNodePath );
			return AttachChildNode( destSceneName, destNodePath, destIdx );
        }
    }
    return false;
}

// *******************************
//
bool    BVProjectEditor::AddChildNode         ( model::SceneModelPtr scene, model::IModelNodePtr parentNode, model::IModelNodePtr childNode )
{
    if( !scene || !childNode )
    {
        return false;
    }

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
        SetSceneRootNode( scene, childNode );
    }
    
    return true;
}

// *******************************
//
bool    BVProjectEditor::DeleteChildNode      ( model::SceneModelPtr scene, model::IModelNodePtr parentNode, model::IModelNodePtr node )
{    
    if( scene )
    {
        if( node == scene->GetRootNode() && !parentNode )
        {
            DeleteSceneRootNode( scene );
            return true;
        }
        else if( node && parentNode )
        {
            auto modelParentNode = QueryTyped( parentNode );
            auto modelChildNode = QueryTyped( node );
            auto sceneEditor = scene->GetModelSceneEditor();

            auto success = true;

            success &= sceneEditor->DeleteChildNode( modelParentNode, modelChildNode );
            success &= m_engineSceneEditor->DeleteChildNode( GetEngineNode( parentNode ), GetEngineNode( modelChildNode ) );
        
            MappingsCleanup( modelChildNode );

            return success;
        }
    }

    return false;
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent )
{
    return AttachChildNode( scene, parent, parent->GetNumChildren() );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent, UInt32 posIdx )
{
    if( scene && parent )
    {
        auto sceneEditor = scene->GetModelSceneEditor();
        auto attachModel    = sceneEditor->AttachChildNode( QueryTyped( parent ), posIdx );
        auto attachEngine   = m_engineSceneEditor->AttachChildNode( GetEngineNode( parent ), posIdx );

        return attachModel && attachEngine;
    }

    return false;
}

// *******************************
//
bool                    BVProjectEditor::DetachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent, model::IModelNodePtr nodeToDetach )
{
    if( scene && parent && nodeToDetach )
    {
        auto sceneEditor = scene->GetModelSceneEditor();
        MappingsCleanup( sceneEditor->GetDetachedNode() );

        auto detachModel    = sceneEditor->DetachChildNode( QueryTyped( parent ), QueryTyped( nodeToDetach ) );
        auto detachEngine   = m_engineSceneEditor->DetachChildNode( GetEngineNode( parent ), GetEngineNode( nodeToDetach ) );
        
        return detachModel && detachEngine;
    }

    return false;
}

// *******************************
//
void            BVProjectEditor::DeleteDetachedNodes          ( const std::string & sceneName )
{
    auto scene = m_project->GetScene( sceneName );

    if( scene )
    {
        auto sceneEditor = scene->GetModelSceneEditor();
        auto detachedNode = sceneEditor->GetDetachedNode();
        
        MappingsCleanup( detachedNode );

        sceneEditor->DeleteDetachedNode();
        m_engineSceneEditor->DeleteDetachedNode();
    }
}

// *******************************
//
bool            BVProjectEditor::SetNodeVisible				( const std::string & sceneName, const std::string & nodePath, bool visible )
{
    auto node = GetNode( sceneName, nodePath );
    return SetNodeVisible( node, visible );
}

// *******************************
//
bool            BVProjectEditor::RenameNode					( const std::string & sceneName, const std::string & nodePath, const std::string & newNodeName )
{
	auto node = GetNode( sceneName, nodePath );
	return RenameNode( node, newNodeName );
}

// *******************************
//
model::BasicNodePtr		BVProjectEditor::AddNodeCopy        ( model::SceneModelPtr destScene, model::BasicNodePtr destParentNode, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode )
{
    if( !srcScene || !destScene )
    {
        return false;
    }

    model::BasicNodePtr	copy = nullptr;
    
    if( srcScene == destScene )
    {
        //don't copy timelines, maybe it should be handled by CloneViaSerialization::ClonePlugin with empty prefix string?
        copy = srcNode->GetModelNodeEditor()->CopyNode();
    }
    else
    {
        //copy timelines
        auto timelines = srcNode->GetTimelines( true );
        auto prefixNum = model::TimelineHelper::CopyTimelines( destScene->GetTimeline(), timelines );

        copy = CloneViaSerialization::CloneNode( srcNode.get(), PrefixHelper::PrefixCopy( prefixNum ), srcScene->GetName(), destScene->GetName() );
    }

    if( copy )
    {
        AddChildNode( destScene, destParentNode, copy );
    }

    return copy;
}

// *******************************
//
bool					BVProjectEditor::MoveNode			( model::SceneModelPtr destScene, model::BasicNodePtr destParentNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcParentNode, model::BasicNodePtr srcNode )
{
    if( !srcScene || !destScene )
    {
        return false;
    }

    if( srcScene == destScene )
    {
		if( DetachChildNode( srcScene, srcParentNode, srcNode ) )
        {
		    return AttachChildNode( destScene, destParentNode, destIdx );
        }
    }
    else
    {
        if( AddNodeCopy( destScene, destParentNode, srcScene, srcNode ) )
        {
            DeleteChildNode( srcScene, srcParentNode, srcNode );
            DetachChildNode( destScene, destParentNode, srcNode );
            return AttachChildNode( destScene, destParentNode, destIdx );
        }
    }

    return false;
}

// *******************************
//
bool					BVProjectEditor::AddPlugin			( const std::string & sceneName, const std::string & nodePath, const std::string & pluginUID, const std::string & pluginName, const std::string & timelinePath, UInt32 idx )
{
	auto scene = m_project->GetScene( sceneName );

    model::ITimeEvaluatorPtr timeEval = nullptr;

    if( timelinePath.empty() )
    {
        timeEval = GetSceneDefaultTimeline( scene );
    }
    else
    {
        timeEval = GetTimeEvaluator( timelinePath );
    }

    if( scene && timeEval )
    {
        auto plugin = model::PluginsManager::DefaultInstance().CreatePlugin( pluginUID, pluginName, timeEval );
        auto node = QueryTyped( GetNode( sceneName, nodePath ) );
        return AddPlugin( node, plugin, idx );
    }

    return false;
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
    auto destScene = m_project->GetScene( destSceneName );
    auto srcScene = m_project->GetScene( srcSceneName );

    return AddPluginCopy( destScene, destNode, destIdx, srcScene, srcNode, pluginNameToCopy );
}

// *******************************
//
bool					BVProjectEditor::MovePlugin			( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath, const std::string & pluginName )
{
    auto srcNode =  QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    auto destNode =  QueryTyped( GetNode( destSceneName, destNodePath ) );
    auto destScene = m_project->GetScene( destSceneName );
    auto srcScene = m_project->GetScene( srcSceneName );

    return MovePlugin( destScene, destNode, destIdx, srcScene, srcNode, pluginName );
}

// *******************************
//
bool					BVProjectEditor::AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, UInt32 idx )
{
    if( node )
    {
        auto editor = node->GetModelNodeEditor();
        if( editor->AddPlugin( plugin, idx ) )
        {
            RefreshNode( node, GetEngineNode( node ), m_project->m_renderer );
            return true;
        }
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::DeletePlugin          ( model::BasicNodePtr node, const std::string & name )
{
    if( node )
    {
        auto editor = node->GetModelNodeEditor();
        if( editor->DeletePlugin( name ) )
        {
            RefreshNode( node, GetEngineNode( node ), m_project->m_renderer );
            return true;
        }
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::DeletePlugin          ( model::BasicNodePtr node, UInt32 posIdx )
{
    if( node )
    {
        auto editor = node->GetModelNodeEditor();
        if ( editor->DeletePlugin( posIdx ) )
        {
            RefreshNode( node, GetEngineNode( node ), m_project->m_renderer );
            return true;
        }
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::AttachPlugin          ( model::BasicNodePtr node, UInt32 posIdx )
{
    if( node )
    {
        auto editor = node->GetModelNodeEditor();
        if( editor->AttachPlugin( posIdx ) )
        {
            RefreshNode( node, GetEngineNode( node ), m_project->m_renderer );
            return true;
        }
    }
    return false;
}

// *******************************
//
bool                    BVProjectEditor::AttachPlugin			( model::BasicNodePtr destNode, model::BasicNodePtr srcNode, UInt32 posIdx )
{
    if( srcNode && destNode )
    {
        auto sourceEditor = srcNode->GetModelNodeEditor();
        auto destEditor = destNode->GetModelNodeEditor();

        if( destEditor->AddPlugin(sourceEditor->GetDetachedPlugin(), posIdx ) )
        {
            sourceEditor->ResetDetachedPlugin();
            RefreshNode( destNode, GetEngineNode( destNode ), m_project->m_renderer );
            return true;
        }
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::DetachPlugin          ( model::BasicNodePtr node, const std::string & name )
{
    if( node )
    {
        auto editor = node->GetModelNodeEditor();
        if( editor->DetachPlugin( name ) )
        {
            RefreshNode( node, GetEngineNode( node ), m_project->m_renderer );
            return true;
        }
    }
    return false;
}

// *******************************
//
bool					BVProjectEditor::DetachPlugin          ( model::BasicNodePtr node, UInt32 posIdx )
{
    if( node )
    {
        auto editor = node->GetModelNodeEditor();
        if( editor->DetachPlugin( posIdx ) )
        {
            RefreshNode( node, GetEngineNode( node ), m_project->m_renderer );
            return true;
        }
    }
    return false;
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::GetDetachedPlugin		( model::BasicNodePtr node ) const
{
    if( node )
    {
        return node->GetModelNodeEditor()->GetDetachedPlugin();
    }

    return nullptr;
}

// *******************************
//
void					BVProjectEditor::ResetDetachedPlugin	( model::BasicNodePtr node )
{
    if( node )
    {
        node->GetModelNodeEditor()->ResetDetachedPlugin();
    }
}

// *******************************
//
model::IPluginPtr		BVProjectEditor::AddPluginCopy			( model::SceneModelPtr destScene, model::BasicNodePtr destNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode, const std::string & pluginNameToCopy )
{
    if( !srcScene || !destScene || !srcNode )
    {
        return nullptr;
    }

    model::IPluginPtr copy = nullptr;
    if( srcScene == destScene )
    {
        //don't copy timelines, maybe it should be handled by CloneViaSerialization::ClonePlugin with empty prefix string?
        copy = srcNode->GetModelNodeEditor()->CopyPlugin( pluginNameToCopy );
    }
    else
    {
        auto srcPlugin = srcNode->GetPlugin( pluginNameToCopy );
        if( srcPlugin )
        {
            //copy timelines
            auto timelines = srcPlugin->GetTimelines();
            auto prefixNum = model::TimelineHelper::CopyTimelines( destScene->GetTimeline(), timelines );
        
            copy = CloneViaSerialization::ClonePlugin( srcPlugin.get(), PrefixHelper::PrefixCopy( prefixNum ), srcScene->GetName(), destScene->GetName() );
        }
    }

    if( copy )
    {
        AddPlugin( destNode, copy, destIdx );
    }

    return copy;
}

// *******************************
//
bool			BVProjectEditor::MovePlugin					( model::SceneModelPtr destScene, model::BasicNodePtr destNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode, const std::string & pluginName )
{
    if( !srcScene || !destScene )
    {
        return false;
    }

	bool success = true;
    if( srcScene == destScene )
    {
        if( DetachPlugin( srcNode, pluginName ) )
	    {
            return AttachPlugin( destNode, destIdx );
        }
    }
    else
    {
       if( AddPluginCopy( destScene, destNode, destIdx, srcScene, srcNode, pluginName ) )
       {
	        return DeletePlugin( srcNode, pluginName );
       }
    }

    return success;
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
        BVProjectTools::ReleaseUnusedResources( m_project->m_renderer );
        return success;
	}

    return false;
}

// *******************************
//
bool            BVProjectEditor::SetNodeVisible				( model::IModelNodePtr node, bool visible )
{
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
bool            BVProjectEditor::RenameNode					( model::IModelNodePtr node, const std::string & newNodeName )
{
    if( node )
    {
    	QueryTyped( node )->SetName( newNodeName );
        return true;
    }

    return false;
}

// *******************************
//
model::IModelNodeEffectPtr	BVProjectEditor::GetNodeEffect	( model::IModelNodePtr node ) const
{
    if( node )
    {
        return QueryTyped( node )->GetNodeEffect();
    }

    return nullptr;
}

// *******************************
//
bool						BVProjectEditor::SetNodeEffect	( model::IModelNodePtr node, model::IModelNodeEffectPtr nodeEffect )
{
    if( node && nodeEffect )
    {
        auto modelNode = QueryTyped( node );
        modelNode->SetNodeEffect( nodeEffect );
        BVProjectTools::UpdateSceneNodeEffect( GetEngineNode( node ), modelNode );
        return true;
    }

    return false;
}

// ***********************
//
bool                        BVProjectEditor::SetNodeEffect   ( const std::string & sceneName, const std::string & nodePath, const std::string & timelinePath, const std::string & effectName )
{
    auto scene = GetScene( sceneName );

    model::ITimeEvaluatorPtr timeEval = nullptr;
    if( timelinePath.empty() )
    {
        timeEval = GetSceneDefaultTimeline( scene );
    }
    else
    {
        timeEval = GetTimeEvaluator( timelinePath );
    }

    if( timeEval )
    {
        auto newEffect = model::ModelNodeEffectFactory::Create( effectName, timeEval );
        auto node = GetNode( sceneName, nodePath );
        return SetNodeEffect( node, newEffect );
    }

    return false;
}

// *******************************
//
bool						BVProjectEditor::AddTimeline			( const std::string & parentTimelinePath, const std::string & timelineName, TimelineType timelineType )
{
    auto timeline = model::TimelineHelper::CreateTimeEvaluator( timelineName, timelineType );
    auto parentTimeline = GetTimeEvaluator( parentTimelinePath );

    return AddTimeline( parentTimeline, timeline );
}

// *******************************
//
bool						BVProjectEditor::AddTimeline			( model::ITimeEvaluatorPtr parentTimeline, model::ITimeEvaluatorPtr timeline )
{
    if( timeline && parentTimeline && 
        !parentTimeline->GetChild( timeline->GetName() ) ) //FIXME: don't allow duplicated timeline names
    {
        parentTimeline->AddChild( timeline );
        return true;
    }

    return false;
}

// *******************************
//
bool						BVProjectEditor::DeleteTimeline			( const std::string & timelinePath )
{
    auto timeEval = GetTimeEvaluator( timelinePath );
    if( IsTimelineEditable( timeEval.get() ) && timeEval.use_count() == 2 ) //FIXME: maybe it's more safe to go through node tree..
    {
        auto sceneName = model::TimelineHelper::GetSceneName( timeEval.get() );
        return model::TimelineManager::GetInstance()->RemoveTimelineFromTimeline( timelinePath, sceneName );
    }

    return false;
}

// *******************************
//
bool						BVProjectEditor::ForceDeleteTimeline	( const std::string & timelinePath, const std::string & newTimelinePath )
{
    auto timeEval = GetTimeEvaluator( timelinePath );
    if( IsTimelineEditable( timeEval.get() ) )
    {
        auto sceneName = model::TimelineHelper::GetSceneName( timeEval.get() );
        auto scene = GetScene( sceneName );

        model::ITimeEvaluatorPtr newTimeEval = nullptr;
        if( newTimelinePath.empty() )
        {
            newTimeEval = GetSceneDefaultTimeline( scene );
        }
        else
        {
            newTimeEval = GetTimeEvaluator( newTimelinePath );
        }


        if( !scene || !scene->GetRootNode() || !newTimeEval
            || sceneName != model::TimelineHelper::GetSceneName( newTimeEval.get() ) )
        {
            return false;
        }
        
        scene->GetRootNode()->GetModelNodeEditor()->ReplaceTimeline( timeEval, newTimeEval );
        auto success = model::TimelineManager::GetInstance()->RemoveTimelineFromTimeline( timelinePath, sceneName );
    
        assert( timeEval.use_count() == 1 );
        
        return success;
    }

    return false;
}
    
// *******************************
//
bool						BVProjectEditor::RenameTimeline			( const std::string & timelinePath, const std::string & newName )
{
    auto timeline = GetTimeEvaluator( timelinePath );
    if( timeline && IsTimelineEditable( timeline.get() ) )
    {
        auto sceneName = model::TimelineHelper::GetSceneName( timeline.get() );
        if( !GetTimeEvaluator( sceneName )->GetChild( newName ) )
        {
            timeline->SetName( newName );
            return true;
        }
    }

    return false;
}

// *******************************
//
bool						BVProjectEditor::SetTimelineDuration			( const std::string & timelinePath, TimeType duration )
{
    auto timeline = GetTimeline( timelinePath );
    if( timeline && IsTimelineEditable( timeline.get() ) )
    {
        timeline->SetDuration( duration );
        return true;
    }

    return false;
}

// *******************************
//
bool						BVProjectEditor::SetTimelineWrapPreBehavior		( const std::string & timelinePath, TimelineWrapMethod preMethod )
{
    auto timeline = GetTimeline( timelinePath );
    if( timeline && IsTimelineEditable( timeline.get() ) )
    {
        timeline->SetWrapBehavior( preMethod, timeline->GetWrapBehaviorPost() );
        return true;
    }

    return false;
}

// *******************************
//
bool						BVProjectEditor::SetTimelineWrapPostBehavior	( const std::string & timelinePath, TimelineWrapMethod postMethod )
{
    auto timeline = GetTimeline( timelinePath );
    if( timeline && IsTimelineEditable( timeline.get() ) )
    {
        timeline->SetWrapBehavior( timeline->GetWrapBehaviorPre(), postMethod );
        return true;
    }

    return false;
}

// *******************************
//
void                    BVProjectEditor::RefreshNode        (  model::BasicNodePtr modelNode, SceneNode * sceneNode, Renderer * renderer )
{
    BVProjectTools::ClearSingleNode( sceneNode, renderer );
	BVProjectTools::SyncSingleNode( modelNode, sceneNode );
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
model::IModelNodePtr	BVProjectEditor::GetNode			( const std::string & sceneName, const std::string & nodePath, const std::string & separator ) const
{
    model::IModelNodePtr node = nullptr;

    auto scene = m_project->GetScene( sceneName );
    if( scene )
    {
        node = scene->GetModelSceneEditor()->GetNode( nodePath, separator );
    }

    if( !node )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Node [" + nodePath + "] in scene [" +  sceneName + "] not found";
    }

    return node;
}

// ***********************
//
model::IModelNodePtr    BVProjectEditor::GetParentNode		( const std::string & sceneName, const std::string & nodePath, const std::string & separator ) const
{
    auto parentNodePath = nodePath.substr( 0, nodePath.find_last_of( separator ) );
    return GetNode( sceneName, parentNodePath, separator );
}

// ***********************
//
model::ITimeEvaluatorPtr    BVProjectEditor::GetTimeEvaluator           ( const std::string & timelinePath ) const
{
    return model::TimelineManager::GetInstance()->GetTimeEvaluator( timelinePath );
}

// ***********************
//
model::ITimelinePtr         BVProjectEditor::GetTimeline                ( const std::string & timelinePath ) const
{
    return model::TimelineManager::GetInstance()->GetTimeline( timelinePath );
}

// *******************************
//
SceneNode *             BVProjectEditor::GetEngineNode      ( model::IModelNodePtr node ) const
{
    return m_nodesMapping.at( node.get() );
}

// *******************************
//
std::string				BVProjectEditor::PrefixSceneName		( const std::string & name ) const
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

// CUSTOM DEFAULTS

// *******************************
//
void				    BVProjectEditor::InitDefaultScene       ( model::SceneModelPtr scene )
{
    //Set default root node & timeline

    auto defaultTimeline = GetTimeline( model::TimelineHelper::CombineTimelinePath( scene->GetName(), DEFAULT_TIMELINE_NAME ) );
    //don't add default timeline if it already exists
    if( !defaultTimeline )
    {
        defaultTimeline = model::TimelineHelper::CreateDefaultTimeline( DEFAULT_TIMELINE_NAME, ( std::numeric_limits< TimeType >::max )(), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP ); //FIXME: infinite duration
        scene->GetTimeline()->AddChild( defaultTimeline );
    }

    //don't overwrite root node if it already exists
    if( !scene->GetRootNode() ) 
    {
        auto modelSceneRootNode = model::BasicNode::Create( DEFAULT_ROOT_NAME, nullptr );
        modelSceneRootNode->AddPlugin( model::DefaultTransformPluginDesc::UID(), "transform", defaultTimeline );
        scene->SetRootNode( modelSceneRootNode );
    }
}

// *******************************
//
bool				    BVProjectEditor::IsTimelineEditable     ( const model::ITimeEvaluator * timeEval ) const
{
    auto sceneName = model::TimelineHelper::GetSceneName( timeEval );
    auto scene = GetScene( sceneName );
    if( sceneName.empty() || timeEval == GetSceneDefaultTimeline( scene ).get() )
    {
        return false; //editing scene timeline & default is not allowed
    }
    return true;
}

// ***********************
//
model::ITimelinePtr         BVProjectEditor::GetSceneDefaultTimeline     ( model::SceneModelConstPtr scene ) const
{
    if( scene )
    {
        return std::static_pointer_cast< model::ITimeline >( scene->GetTimeline()->GetChild( DEFAULT_TIMELINE_NAME ) );
    }

    return nullptr;
}

// *******************************
//
const std::string       BVProjectEditor::DEFAULT_ROOT_NAME = "root";
const std::string       BVProjectEditor::DEFAULT_TIMELINE_NAME = "default";

} //bv
