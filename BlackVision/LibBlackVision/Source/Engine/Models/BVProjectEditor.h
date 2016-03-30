#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/SceneModel.h"

namespace bv {

namespace model {
    class ModelSceneEditor;

    class ITimeline;
    DEFINE_PTR_TYPE( ITimeline );
} // model

class BVProject;
class SceneNode;
class SceneEditor;

class BVProjectEditor
{
private:

    typedef std::hash_map< model::IModelNode *, SceneNode * >	TNodesMapping;
    typedef std::vector< model::SceneModelPtr >					SceneModelVec;

private:

    BVProject *					m_project;
    model::BasicNodePtr			m_rootNode;

    SceneEditor *               m_engineSceneEditor;
    
    TNodesMapping	            m_nodesMapping;

    SceneModelVec				m_detachedScenes;

private:

                            BVProjectEditor       ( BVProject * project );

public:

/* SCENES */

    void                    AddScene			( const std::string & sceneName );
    void                    AddScene			( model::SceneModelPtr scene );

    bool                    RemoveScene			( const std::string & sceneName );
    bool                    RemoveScene			( model::SceneModelPtr scene );
    void                    RemoveAllScenes		();

    /** Insert scene at the end. */
    bool                    AttachScene			( const std::string & sceneName );
    /** Insert scene at destIdx position. */
    bool                    AttachScene			( const std::string & sceneName, UInt32 posIdx );
    bool                    DetachScene			( const std::string & sceneName );
    void                    DeleteDetachedScenes();

    bool					SetSceneVisible		( const std::string & sceneName, bool visible );

    model::SceneModelPtr    GetScene			( const std::string & sceneName ) const;

    bool					RenameScene			( const std::string & oldSceneName, const std::string & newSceneName );

    /** Add a copy of scene with given name to the project.
    @return Returns copied scene. */
    model::SceneModelPtr	AddSceneCopy		( const std::string & sceneNameToCopy );

    bool					MoveScene			( const std::string & sceneName, UInt32 posIdx );


/* NODES */

    model::IModelNodePtr	GetNode             ( const std::string & sceneName, const std::string & nodePath, const std::string & separator = "/" ) const;
    model::IModelNodePtr	GetParentNode       ( const std::string & sceneName, const std::string & nodePath, const std::string & separator = "/" ) const;
    
    // Find node with nodeName in substree
    model::IModelNodePtr	FindNode            ( model::BasicNodePtr node, const std::string & nodeName ) const;

    // Selection and intersection
    model::IModelNodePtr	FindIntersectingNode    ( glm::vec3 rayStart, glm::vec3 rayDirection );
    bool                    SelectNode              ( model::IModelNodePtr node, glm::vec4 color );
    void                    UnselectNodes           ();

    /* paths */
    bool                    AddChildNode        ( const std::string & sceneName, const std::string & parentPath, const std::string & newNodeName );
    bool                    DeleteChildNode     ( const std::string & sceneName, const std::string & nodePath );

    /** Insert node at the end. */
    bool                    AttachChildNode     ( const std::string & sceneName, const std::string & parentPath );
    /** Insert node at destIdx position. */
    bool                    AttachChildNode     ( const std::string & sceneName, const std::string & parentPath, UInt32 posIdx );
    bool                    DetachChildNode     ( const std::string & sceneName, const std::string & nodeToDetachPath );
    
    model::BasicNodePtr		AddNodeCopy			( const std::string & destSceneName, const std::string & destParentPath, const std::string & srcSceneName, const std::string & srcNodePath );
    
    bool					MoveNode			( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath );

    bool					SetNodeVisible		( const std::string & sceneName, const std::string & nodePath, bool visible );
    
    bool					RenameNode			( const std::string & sceneName, const std::string & nodePath, const std::string & newNodeName );

    void                    DeleteDetachedNodes ( const std::string & sceneName );

    /* objects */
    bool                    AddChildNode        ( model::SceneModelPtr scene, model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    bool                    DeleteChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    
    /** Insert node at the end. */
    bool                    AttachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent );
    /** Insert node at destIdx position. */
    bool                    AttachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent, UInt32 posIdx );
    bool                    DetachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent, model::IModelNodePtr nodeToDetach );

    /** Add a copy of node to the destParentNode from the given scene.
    @param[ destParentNode ] If nullptr, set node copy as root node of the scene.
    @return Returns copied node. */
    model::BasicNodePtr		AddNodeCopy			( model::SceneModelPtr destScene, model::BasicNodePtr destParentNode, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode );
    
    bool					MoveNode			( model::SceneModelPtr destScene, model::BasicNodePtr destParentNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcParentNode, model::BasicNodePtr srcNode );

    bool					SetNodeVisible		( model::IModelNodePtr node, bool visible );

    bool					RenameNode			( model::IModelNodePtr node, const std::string & newNodeName );

/* PLUGINS */
    
    /* paths */
    bool                    AddPlugin			( const std::string & sceneName, const std::string & nodePath, const std::string & pluginUID, const std::string & pluginName, const std::string & timelinePath, UInt32 posIdx );
    bool                    DeletePlugin		( const std::string & sceneName, const std::string & nodePath, UInt32 pluginIdx );
    bool                    DeletePlugin		( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName );

    bool                    AttachPlugin		( const std::string & sceneName, const std::string & nodePath, UInt32 posIdx );
    bool                    AttachPlugin		( const std::string & destSceneName, const std::string & destNodePath, const std::string & srcSceneName, const std::string & srcNodePath, UInt32 posIdx );
    bool                    DetachPlugin		( const std::string & sceneName, const std::string & nodePath, UInt32 pluginIdx );
    bool                    DetachPlugin		( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName );

    model::IPluginPtr		GetDetachedPlugin	( const std::string & sceneName, const std::string & nodePath );
    void                    ResetDetachedPlugin	( const std::string & sceneName, const std::string & nodePath );

    model::IPluginPtr		AddPluginCopy		( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath, const std::string & pluginNameToCopy );

    bool					MovePlugin			( const std::string & destSceneName, const std::string & destParentNode, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcParentNode, const std::string & pluginName );

    /* objects */
    bool                    AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, UInt32 posIdx );
    bool                    DeletePlugin		( model::BasicNodePtr node, UInt32 pluginIdx );
    bool                    DeletePlugin		( model::BasicNodePtr node, const std::string & name );

    bool                    AttachPlugin		( model::BasicNodePtr node, UInt32 posIdx );
    bool                    AttachPlugin		( model::BasicNodePtr destNode, model::BasicNodePtr srcNode, UInt32 posIdx );
    bool                    DetachPlugin		( model::BasicNodePtr node, UInt32 posIdx );
    bool                    DetachPlugin		( model::BasicNodePtr node, const std::string & name );
    
    model::IPluginPtr		GetDetachedPlugin	( model::BasicNodePtr node ) const;
    void                    ResetDetachedPlugin	( model::BasicNodePtr node );

    /** Add a copy of plugin from srcNode with given name to the destNode on destIdx position.
    @return Returns copied plugin. */
    model::IPluginPtr		AddPluginCopy		( model::SceneModelPtr destScene, model::BasicNodePtr destNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode, const std::string & pluginNameToCopy );

    bool					MovePlugin			( model::SceneModelPtr destScene, model::BasicNodePtr destNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode, const std::string & pluginName );


/* EFFECTS */

    model::IModelNodeEffectPtr	GetNodeEffect   ( model::IModelNodePtr node ) const;
    bool						SetNodeEffect   ( model::IModelNodePtr node, model::IModelNodeEffectPtr nodeEffect );
    bool                        SetNodeEffect   ( const std::string & sceneName, const std::string & nodePath, const std::string & timelinePath, const std::string & effectName );
    bool						LoadGlobalEffectAsset( const std::string & sceneName, const std::string & nodePath, const std::string & timelinePath, const std::string & effectName, IDeserializer & serializedAssetData, SizeType idx );

/* TIMELINES */

    bool                    AddTimeline					( const std::string & parentTimelinePath, const std::string & timelineName, TimelineType timelineType );
    bool                    AddTimeline					( model::ITimeEvaluatorPtr parentTimeline, model::ITimeEvaluatorPtr timeline );
    
    bool                    DeleteTimeline				( const std::string & timelinePath );
    bool                    ForceDeleteTimeline			( const std::string & timelinePath, const std::string & newTimelinePath = std::string() );

    bool                    RenameTimeline				( const std::string & timelinePath, const std::string & newName );

    bool                    SetTimelineDuration			( const std::string & timelinePath, TimeType duration );
    bool                    SetTimelineWrapPreBehavior	( const std::string & timelinePath, TimelineWrapMethod preMethod );
    bool                    SetTimelineWrapPostBehavior	( const std::string & timelinePath, TimelineWrapMethod postMethod );

    model::ITimeEvaluatorPtr    GetTimeEvaluator        ( const std::string & timelinePath ) const;
    model::ITimelinePtr         GetTimeline             ( const std::string & timelinePath ) const;


/* ASSETS */

    bool					LoadAsset					( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, IDeserializer & serializedAssetData );
    bool					LoadAsset					( model::IPluginPtr plugin, AssetDescConstPtr assetDesc );

private:

    /* engine scene helpers */
    void					RefreshNode			( model::BasicNodePtr modelNode, SceneNode * sceneNode, Renderer * renderer );
    void                    MappingsCleanup     ( model::IModelNodePtr node );

    void                    RemoveNodeMapping   ( model::IModelNodePtr node );
    void                    UnregisterUpdaters  ( model::IModelNodePtr node );

    SceneNode *             GetEngineNode       ( model::IModelNodePtr node ) const;
    
    
    /* model scene helpers */

    bool                    AddModelScene       ( model::SceneModelPtr scene, UInt32 idx );
    bool                    RemoveModelScene    ( model::SceneModelPtr scene );

    void                    SetModelSceneRootNode   ( model::SceneModelPtr scene, model::IModelNodePtr rootNode );
    void                    DeleteModelSceneRootNode( model::SceneModelPtr scene );


    /* renaming helpers */
    std::string             PrefixSceneName		( const std::string & name ) const;

    friend class BVProject;


// CUSTOM DEFAULTS

public:

    model::ITimelinePtr         GetSceneDefaultTimeline ( model::SceneModelConstPtr scene ) const;

private:

    void                        InitDefaultScene	    ( model::SceneModelPtr scene );
    
    bool                        IsTimelineEditable      ( const model::ITimeEvaluator * timeEval ) const;
    
    static const std::string    DEFAULT_ROOT_NAME;
    static const std::string    DEFAULT_TIMELINE_NAME;

};

} //bv
