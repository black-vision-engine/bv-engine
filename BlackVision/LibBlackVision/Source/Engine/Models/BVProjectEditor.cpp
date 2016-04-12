#include "stdafx.h"

#include "BVProjectEditor.h"
#include "ModelState.h"

#include "Engine/Models/BVProject.h"
#include "Engine/Models/Updaters/SceneUpdater.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneEditor.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"

#include "Engine/Models/ModelNodeEditor.h"
#include "Engine/Models/ModelSceneEditor.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectFactory.h"
#include "Engine/Models/Lights/HelperModelLights.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Serialization/Json/JsonDeserializeObject.h"

#include "Assets/AssetManager.h"

#include "Tools/Utils.h"
#include "Tools/PrefixHelper.h"

#include "Serialization/SerializationHelper.h"
#include "UseLoggerLibBlackVision.h"

#include "Engine/Models/BoundingVolume.h"



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
void    BVProjectEditor::AddScene			( const std::string & sceneName )
{
    AddScene( model::SceneModel::CreateEmptyScene( sceneName ) );
}

// *******************************
//
void    BVProjectEditor::AddScene       ( model::SceneModelPtr modelScene )
{
    auto idx = ( UInt32 )m_project->m_sceneModelVec.size();
    
    AddModelScene( modelScene, idx );
    InitDefaultScene( modelScene );

    AddEngineScene( modelScene, idx );
}

// *******************************
//
bool    BVProjectEditor::RemoveScene		( const std::string & sceneName )
{
    auto scene = m_project->GetModelScene( sceneName );
    return RemoveScene( scene );
}

// *******************************
//
bool    BVProjectEditor::RemoveScene		( model::SceneModelPtr modelScene )
{
    if( modelScene )
    {
        RemoveEngineScene( modelScene );
        RemoveModelScene( modelScene );

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
    auto modelScene = m_project->GetModelScene( sceneName );
    if( modelScene )
    {
        RemoveEngineScene( modelScene );
        m_detachedScenes.push_back( modelScene );
        RemoveModelScene( modelScene );

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
            AddModelScene( m_detachedScenes[ i ], posIdx );

            AddEngineScene( m_detachedScenes[ i ], posIdx );

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
model::SceneModelPtr    BVProjectEditor::GetModelScene ( const std::string & sceneName ) const
{
    return m_project->GetModelScene( sceneName );
}

// *******************************
//
bool    BVProjectEditor::SetSceneVisible		( const std::string & sceneName, bool visible )
{
    auto scene = m_project->GetModelScene( sceneName );
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
    auto scene = m_project->GetModelScene( sceneName );
    if( scene && !m_project->GetModelScene( newSceneName ) )
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
    auto scene = m_project->GetModelScene( sceneNameToCopy );
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
void    BVProjectEditor::AddModelScene          ( model::SceneModelPtr sceneModel, UInt32 idx )
{
    auto sceneName = sceneModel->GetName();
    if( m_project->GetModelScene( sceneName ) )
    {
        sceneModel->SetName( PrefixSceneName( sceneName ) );
    }

    if( idx < m_project->m_sceneModelVec.size() )
    {
        m_project->m_sceneModelVec.insert( m_project->m_sceneModelVec.begin() + idx, sceneModel );
    }
    else
    {
        m_project->m_sceneModelVec.push_back( sceneModel );
    }

    auto modelSceneRoot = sceneModel->GetRootNode();
    if( modelSceneRoot )
    {
        m_rootNode->AddChildToModelOnly( modelSceneRoot, idx );

        model::ModelState::GetInstance().RegisterNode( modelSceneRoot.get(), nullptr );
    }

    m_project->m_globalTimeline->AddChild( sceneModel->GetTimeline() );
}

// *******************************
//
void    BVProjectEditor::AddEngineScene         ( model::SceneModelPtr modelScene, UInt32 idx )
{
    auto modelRoot = modelScene->GetRootNode();
    if( modelRoot )
    {
        auto scene = BVProjectTools::BuildEngineScene( modelScene, modelRoot, m_nodesMapping );
        
        if( idx < m_project->m_sceneVec.size() )
        {
            m_project->m_sceneVec.insert( m_project->m_sceneVec.begin() + idx, scene );
        }
        else
        {
            m_project->m_sceneVec.push_back( scene );
        }

        m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), scene->GetRoot() );

        //add scene mapping
        m_scenesMapping[ modelScene.get() ] = scene;

        //add scene updater
        auto updater  = SceneUpdater::Create( scene, modelScene.get() );
        UpdatersManager::Get().RegisterUpdater( modelScene.get(), updater );
    }
}

// *******************************
//
bool    BVProjectEditor::RemoveModelScene       ( model::SceneModelPtr sceneModel )
{
    auto & modelScenes = m_project->m_sceneModelVec;
    auto it = std::find( modelScenes.begin(), modelScenes.end(), sceneModel );

    if( it != modelScenes.end() )
    {
        sceneModel->GetModelSceneEditor()->DeleteRootNode( m_rootNode );
        m_project->m_globalTimeline->RemoveChild( sceneModel->GetTimeline() );

        auto modelSceneRoot = sceneModel->GetRootNode();
        if( modelSceneRoot )
        {
            model::ModelState::GetInstance().UnregisterNode( modelSceneRoot.get() );
        }

        m_project->m_sceneModelVec.erase( it );
        
        return true;
    }

    return false;
}

// *******************************
//
bool    BVProjectEditor::RemoveEngineScene      ( model::SceneModelPtr modelScene )
{
    auto scene = m_scenesMapping[ modelScene.get() ];
    auto & scenes = m_project->m_sceneVec;
    auto it = std::find( scenes.begin(), scenes.end(), scene );

    if( it != scenes.end() )
    { 
        auto modelRoot = modelScene->GetRootNode();
        if( modelRoot )
        {
            m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( modelRoot ) );
        }

        m_project->m_sceneVec.erase( it );

        MappingsCleanup( modelScene );

        return true;
    }

    return false;
}

// *******************************
//
void    BVProjectEditor::SetSceneRootNode		    ( model::SceneModelPtr modelScene, model::IModelNodePtr rootNode )
{
    assert( modelScene );
    assert( rootNode );

    auto oldRoot = modelScene->GetRootNode();
    if( oldRoot )
    {
        m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( oldRoot ) );
        MappingsCleanup( oldRoot );
    }

    auto modelNode = QueryTyped( rootNode );
    modelScene->GetModelSceneEditor()->SetRootNode( m_rootNode, modelNode );

    auto engineNode = BVProjectTools::BuildEngineSceneNode( modelNode, m_nodesMapping );
    m_engineSceneEditor->AddChildNode( m_engineSceneEditor->GetRootNode(), engineNode );

    auto scene = m_scenesMapping[ modelScene.get() ];
    scene->SetRoot( engineNode );
}

// *******************************
//
void    BVProjectEditor::DeleteSceneRootNode	    ( model::SceneModelPtr modelScene )
{
    assert( modelScene );

    auto root = modelScene->GetRootNode();

    m_engineSceneEditor->DeleteChildNode( m_engineSceneEditor->GetRootNode(), GetEngineNode( root ) );
    MappingsCleanup( root );

    modelScene->GetModelSceneEditor()->SetRootNode( m_rootNode, nullptr );

    auto scene = m_scenesMapping[ modelScene.get() ];
    scene->SetRoot( nullptr );
}

// *******************************
//
bool    BVProjectEditor::AddChildNode         ( const std::string & sceneName, const std::string & parentPath, const std::string & newNodeName )
{
    auto newNode = model::BasicNode::Create( newNodeName, nullptr );
    auto parentNode = GetNode( sceneName, parentPath );
    auto scene = m_project->GetModelScene( sceneName );

    return AddChildNode( scene, parentNode, newNode );
}

// *******************************
//
bool    BVProjectEditor::DeleteChildNode      ( const std::string & sceneName, const std::string & nodePath )
{ 
    auto parentNode =  GetParentNode( sceneName, nodePath );
    auto node =  GetNode( sceneName, nodePath );
    auto scene = m_project->GetModelScene( sceneName );

    return DeleteChildNode( scene, parentNode, node );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, const std::string & parentPath )
{
    auto parentNode = GetNode( sceneName, parentPath );
    auto scene = m_project->GetModelScene( sceneName );
    return AttachChildNode( scene, parentNode );
}

// *******************************
//
bool                    BVProjectEditor::AttachChildNode     ( const std::string & sceneName, const std::string & parentPath, UInt32 posIdx )
{
    auto parentNode =  GetNode( sceneName, parentPath );
    auto scene = m_project->GetModelScene( sceneName );
    return AttachChildNode( scene, parentNode, posIdx );
}

// *******************************
//
bool                    BVProjectEditor::DetachChildNode     ( const std::string & sceneName, const std::string & nodeToDetachPath )
{
    auto parentNode =  GetParentNode( sceneName, nodeToDetachPath );
    auto nodeToDetach =  GetNode( sceneName, nodeToDetachPath );
    auto scene = m_project->GetModelScene( sceneName );
    return DetachChildNode( scene, parentNode, nodeToDetach );
}

// *******************************
//
model::BasicNodePtr		BVProjectEditor::AddNodeCopy        ( const std::string & destSceneName, const std::string & destParentPath, const std::string & srcSceneName, const std::string & srcNodePath )
{
    auto destParent = QueryTyped( GetNode( destSceneName, destParentPath ) );
    auto srcNode = QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    auto destScene = m_project->GetModelScene( destSceneName );
    auto srcScene = m_project->GetModelScene( srcSceneName );
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
    if( scene && node )
    {
        if( node == scene->GetRootNode() )
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
    auto scene = m_project->GetModelScene( sceneName );

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
    if( !srcScene || !destScene || !srcNode )
    {
        return false;
    }

    auto copy = srcNode->GetModelNodeEditor()->CopyNode();
    
    if( srcScene != destScene )
    {
        //copy timelines
        auto timelines = srcNode->GetTimelines( true );
        auto prefixNum = model::TimelineHelper::CopyTimelines( destScene->GetTimeline(), timelines );
        
        CloneViaSerialization::UpdateTimelines( copy.get(), PrefixHelper::PrefixCopy( prefixNum ), destScene->GetName(), true );
    }

    AddChildNode( destScene, destParentNode, copy );

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
    auto scene = m_project->GetModelScene( sceneName );

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
    auto destScene = m_project->GetModelScene( destSceneName );
    auto srcScene = m_project->GetModelScene( srcSceneName );

    return AddPluginCopy( destScene, destNode, destIdx, srcScene, srcNode, pluginNameToCopy );
}

// *******************************
//
bool					BVProjectEditor::MovePlugin			( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath, const std::string & pluginName )
{
    auto srcNode =  QueryTyped( GetNode( srcSceneName, srcNodePath ) );
    auto destNode =  QueryTyped( GetNode( destSceneName, destNodePath ) );
    auto destScene = m_project->GetModelScene( destSceneName );
    auto srcScene = m_project->GetModelScene( srcSceneName );

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
    
    auto srcPlugin = srcNode->GetPlugin( pluginNameToCopy );

    if( srcPlugin )
    {
        auto copy = srcNode->GetModelNodeEditor()->CopyPlugin( pluginNameToCopy );

        if( srcScene != destScene )
        {
            //copy timelines
            auto timelines = srcPlugin->GetTimelines();
            auto prefixNum = model::TimelineHelper::CopyTimelines( destScene->GetTimeline(), timelines );
        
            CloneViaSerialization::UpdateTimelines( copy.get(), PrefixHelper::PrefixCopy( prefixNum ), destScene->GetName() );
        }

        AddPlugin( destNode, copy, destIdx );

        return copy;
    }

    return nullptr;
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
bool					BVProjectEditor::LoadAsset			( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, IDeserializer & serializedAssetData )
{
    auto assetDesc = AssetManager::GetInstance().CreateDesc( serializedAssetData );

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
bool            BVProjectEditor::AddLight                   ( const std::string & sceneName, const std::string & lightType, const std::string & timelinePath )
{
    auto modelScene = m_project->GetModelScene( sceneName );
    auto type = SerializationHelper::String2T< LightType >( lightType , LightType::LT_DIRECTIONAL );
    auto timeline = GetTimeEvaluator( timelinePath );

    return AddLight( modelScene, type, timeline );
}

// *******************************
//
bool            BVProjectEditor::RemoveLight                ( const std::string & sceneName, UInt32 idx )
{
    auto modelScene = m_project->GetModelScene( sceneName );
    return RemoveLight( modelScene, idx );
}

// *******************************
//
bool            BVProjectEditor::AddLight                    ( model::SceneModelPtr modelScene, LightType type, model::ITimeEvaluatorPtr timeline )
{
    if( modelScene && timeline )
    {
        auto light = model::HelperModelLights::CreateModelLight( type, timeline );
        modelScene->AddLight( std::shared_ptr< model::IModelLight >( light ) );
        
        return true;
    }

    return false;
}

// *******************************
//
bool            BVProjectEditor::RemoveLight                ( model::SceneModelPtr modelScene, UInt32 idx )
{
    if( modelScene )
    {
        auto light = modelScene->GetLight( idx );
        if( light )
        {
            return modelScene->RemoveLight( idx );
        }
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

        auto engineNode = GetEngineNode( node );

        m_project->DetachEffect( engineNode );

        BVProjectTools::UpdateSceneNodeEffect( engineNode, modelNode );
        return true;
    }

    return false;
}

// ***********************
//
bool                        BVProjectEditor::SetNodeEffect   ( const std::string & sceneName, const std::string & nodePath, const std::string & timelinePath, const std::string & effectName )
{
    auto scene = GetModelScene( sceneName );

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
        auto effect = SerializationHelper::String2T< NodeEffectType >( effectName , NodeEffectType::NET_DEFAULT );
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( effect, effectName, timeEval );
        auto node = GetNode( sceneName, nodePath );
        return SetNodeEffect( node, newEffect );
    }

    return false;
}

// *******************************
//
bool						BVProjectEditor::LoadGlobalEffectAsset( const std::string & sceneName, const std::string & nodePath, const std::string & timelinePath, const std::string &, IDeserializer & serializedAssetData, SizeType idx )
{
    auto scene = GetModelScene( sceneName );

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
        auto node = GetNode( sceneName, nodePath );
        auto modelNode = QueryTyped( node );
        node->GetNodeEffect()->AddAsset( AssetManager::GetInstance().CreateDesc( serializedAssetData ), idx );

        BVProjectTools::UpdateSceneNodeEffect( GetEngineNode( node ), modelNode );

        return true;
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
        auto scene = GetModelScene( sceneName );

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

    auto bv = modelNode->GetBoundingVolume();
    assert( bv );
    bv->UpdateVAC( modelNode->GetFinalizePlugin()->GetVertexAttributesChannel().get() );
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
void                    BVProjectEditor::MappingsCleanup        ( model::SceneModelPtr modelScene )
{
    if( modelScene )
    {
        RemoveSceneMapping( modelScene );
        UnregisterUpdaters( modelScene );
    }
}

// *******************************
//
void                    BVProjectEditor::RemoveSceneMapping    ( model::SceneModelPtr modelScene )
{
    assert( m_scenesMapping.find( modelScene.get() ) != m_scenesMapping.end() );
    m_scenesMapping.erase( modelScene.get() );
    
    auto root = modelScene->GetRootNode();
    if( root )
    {
        RemoveNodeMapping( root );
    }
}

// *******************************
//
void                    BVProjectEditor::UnregisterUpdaters   ( model::SceneModelPtr modelScene )
{
    UpdatersManager::Get().RemoveSceneUpdater( modelScene.get() );

    auto root = modelScene->GetRootNode();
    if( root )
    {
        UnregisterUpdaters( root );
    }
}

// *******************************
//
model::IModelNodePtr	BVProjectEditor::GetNode			( const std::string & sceneName, const std::string & nodePath, const std::string & separator ) const
{
    model::IModelNodePtr node = nullptr;

    auto scene = m_project->GetModelScene( sceneName );
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
model::IModelNodePtr	BVProjectEditor::FindNode            ( model::BasicNodePtr node, const std::string & nodeName ) const
{
    if( node->GetName() == nodeName )
        return node;

    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto foundNode = FindNode( node->GetChild( i ), nodeName );
        if( foundNode != nullptr )
            return foundNode;
    }

    return nullptr;
}

// ***********************
// If nullptr, there's no intersection.
model::IModelNodePtr	BVProjectEditor::FindIntersectingNode    ( glm::vec3 rayStart, glm::vec3 rayDirection )
{
    glm::mat4 transform = glm::mat4( 1 );   // Identity
    auto result = BVProjectTools::NodeIntersection( m_rootNode, transform, rayStart, rayDirection );

    return result.first;
}

// ***********************
//
bool                    BVProjectEditor::SelectNode              ( model::IModelNodePtr node, glm::vec4 color )
{
//    auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_BOUNDING_BOX, "BoundingBox", GetTimeEvaluator( DEFAULT_TIMELINE_NAME ) );
    
    //bool result = SetNodeEffect( node, newEffect );
    //if( !result )
    //    return false;
    
    model::ModelState::GetInstance().Select( node, color );

    return true;
}

// ***********************
//
void                    BVProjectEditor::UnselectNodes           ()
{
    //auto noEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( NodeEffectType::NET_DEFAULT, "no effect", GetTimeEvaluator( DEFAULT_TIMELINE_NAME ) );
    //auto selected = model::ModelState::GetInstance().GetSelectedNodes();

    //for( auto node : selected )
    //{
    //    SetNodeEffect( node, noEffect );
    //}

    model::ModelState::GetInstance().UnselectAll();
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

    return PrefixHelper::PrefixCopy( num + 1 ) + name;
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
        scene->GetModelSceneEditor()->SetRootNode( m_rootNode, modelSceneRootNode );
    }
}

// *******************************
//
bool				    BVProjectEditor::IsTimelineEditable     ( const model::ITimeEvaluator * timeEval ) const
{
    auto sceneName = model::TimelineHelper::GetSceneName( timeEval );
    auto scene = GetModelScene( sceneName );
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
