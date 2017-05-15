#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Graphics/SceneGraph/Scene.h"


namespace bv {

namespace model {
    class ModelSceneEditor;

    class ITimeline;
    DEFINE_PTR_TYPE( ITimeline );

    class BasePlugin;
    DEFINE_PTR_TYPE( BasePlugin );

    class INodeLogic;
    DEFINE_PTR_TYPE( INodeLogic );

    class IModelNodeEffect;
    DEFINE_PTR_TYPE( IModelNodeEffect );
} // model

class BVProject;
class SceneNode;
class SceneEditor;

class BVProjectEditor
{
private:

    typedef std::hash_map< model::IModelNode *, SceneNode * >   TNodesMapping;
    typedef std::hash_map< model::SceneModel *, Scene * >       TScenesMapping;
    typedef std::vector< model::SceneModelPtr >                 SceneModelVec;

private:

    BVProject *                 m_project;
    model::BasicNodePtr         m_rootNode;

    SceneEditor *               m_engineSceneEditor;
    
    TNodesMapping               m_nodesMapping;
    TScenesMapping              m_scenesMapping;

    SceneModelVec               m_detachedScenes;

private:

                            BVProjectEditor       ( BVProject * project );

public:

/* SCENES */

    void                    AddScene            ( const std::string & sceneName );
    void                    AddScene            ( model::SceneModelPtr scene );

    bool                    RemoveScene         ( const std::string & sceneName );
    bool                    RemoveScene         ( model::SceneModelPtr scene );
    void                    RemoveAllScenes     ();

    /** Insert scene at the end. */
    bool                    AttachScene         ( const std::string & sceneName );
    /** Insert scene at destIdx position. */
    bool                    AttachScene         ( const std::string & sceneName, UInt32 posIdx );
    bool                    DetachScene         ( const std::string & sceneName );
    void                    DeleteDetachedScenes();

    bool                    SetSceneVisible     ( const std::string & sceneName, bool visible );
    void                    SetSceneOutputChannel   ( const std::string & sceneName, UInt32 channel );

    model::SceneModelPtr    GetModelScene       ( const std::string & sceneName ) const;

    bool                    RenameScene         ( const std::string & oldSceneName, const std::string & newSceneName );

    /** Add a copy of scene with given name to the project.
    @return Returns copied scene. */
    model::SceneModelPtr    AddSceneCopy        ( const std::string & sceneNameToCopy );

    bool                    MoveScene           ( const std::string & sceneName, UInt32 posIdx );


/* NODES */

    model::IModelNodePtr    GetNode             ( const std::string & sceneName, const std::string & nodePath, const std::string & separator = "/" ) const;
    model::IModelNodePtr    GetParentNode       ( const std::string & sceneName, const std::string & nodePath, const std::string & separator = "/" ) const;
    
    // Find node with nodeName in substree
    model::IModelNodePtr    FindNode            ( model::BasicNodePtr node, const std::string & nodeName ) const;

    // Selection and intersection
    bool                    SelectNode              ( model::IModelNodePtr node, glm::vec4 color );
    void                    UnselectNodes           ();

    std::pair< model::BasicNodePtr, Float32 >       FindIntersectingNode    ( model::SceneModelPtr scene, glm::vec3 rayStart, glm::vec3 rayDirection );
    std::pair< model::BasicNodePtr, Float32 >       FindIntersectingNode    ( const std::string & sceneName, glm::vec3 rayStart, glm::vec3 rayDirection );

    /* paths */
    bool                    AddChildNode        ( const std::string & sceneName, const std::string & parentPath, const std::string & newNodeName, bool enableUndo = false );
    bool                    DeleteChildNode     ( const std::string & sceneName, const std::string & nodePath, bool enableUndo = false );

    /** Insert node at the end. */
    bool                    AttachChildNode     ( const std::string & sceneName, const std::string & parentPath );
    /** Insert node at destIdx position. */
    bool                    AttachChildNode     ( const std::string & sceneName, const std::string & parentPath, UInt32 posIdx );
    bool                    DetachChildNode     ( const std::string & sceneName, const std::string & nodeToDetachPath );
    
    model::BasicNodePtr     AddNodeCopy         ( const std::string & destSceneName, const std::string & destParentPath, const std::string & srcSceneName, const std::string & srcNodePath, bool enableUndo = false );
    
    bool                    MoveNode            ( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath, bool enableUndo = false );

    bool                    SetNodeVisible      ( const std::string & sceneName, const std::string & nodePath, bool visible );
    
    bool                    RenameNode          ( const std::string & sceneName, const std::string & nodePath, const std::string & newNodeName );

    void                    DeleteDetachedNodes ( const std::string & sceneName );

    /* objects */
    bool                    AddChildNode        ( model::SceneModelPtr scene, model::IModelNodePtr parentNode, model::IModelNodePtr childNode, bool enableUndo = false );
    bool                    DeleteChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parentNode, model::IModelNodePtr childNode, bool enableUndo = false );
    
    /** Insert node at the end. */
    bool                    AttachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent );
    /** Insert node at destIdx position. */
    bool                    AttachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent, UInt32 posIdx );
    bool                    DetachChildNode     ( model::SceneModelPtr scene, model::IModelNodePtr parent, model::IModelNodePtr nodeToDetach );

    /** Add a copy of node to the destParentNode from the given scene.
    @param[ destParentNode ] If nullptr, set node copy as root node of the scene.
    @return Returns copied node. */
    model::BasicNodePtr     AddNodeCopy         ( model::SceneModelPtr destScene, model::BasicNodePtr destParentNode, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode, bool enableUndo = false );
    
    bool                    MoveNode            ( model::SceneModelPtr destScene, model::BasicNodePtr destParentNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcParentNode, model::BasicNodePtr srcNode, bool enableUndo = false );

    bool                    SetNodeVisible      ( model::IModelNodePtr node, bool visible );

    bool                    RenameNode          ( model::IModelNodePtr node, const std::string & newNodeName );

/* PLUGINS */
    
    /* paths */
    bool                    AddPlugin           ( const std::string & sceneName, const std::string & nodePath, const std::string & pluginUID, const std::string & pluginName, const std::string & timelinePath, UInt32& posIdx, bool enableUndo = false );
    bool                    DeletePlugin        ( const std::string & sceneName, const std::string & nodePath, UInt32 pluginIdx );
    bool                    DeletePlugin        ( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, bool enableUndo = false );

    bool                    AttachPlugin        ( const std::string & sceneName, const std::string & nodePath, UInt32 posIdx );
    bool                    AttachPlugin        ( const std::string & destSceneName, const std::string & destNodePath, const std::string & srcSceneName, const std::string & srcNodePath, UInt32 posIdx );
    bool                    DetachPlugin        ( const std::string & sceneName, const std::string & nodePath, UInt32 pluginIdx );
    bool                    DetachPlugin        ( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName );

    model::IPluginPtr       GetDetachedPlugin   ( const std::string & sceneName, const std::string & nodePath );
    void                    ResetDetachedPlugin ( const std::string & sceneName, const std::string & nodePath );

    model::IPluginPtr       AddPluginCopy       ( const std::string & destSceneName, const std::string & destNodePath, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcNodePath, const std::string & pluginNameToCopy, bool enableUndo = false );

    bool                    MovePlugin          ( const std::string & destSceneName, const std::string & destParentNode, UInt32 destIdx, const std::string & srcSceneName, const std::string & srcParentNode, const std::string & pluginName, bool enableUndo = false );

    /* objects */
    bool                    AddPlugin           ( model::BasicNodePtr node, model::IPluginPtr plugin, UInt32& posIdx );
    bool                    DeletePlugin        ( model::BasicNodePtr node, UInt32 pluginIdx );
    model::PluginWithIdx    DeletePlugin        ( model::BasicNodePtr node, const std::string & name );

    bool                    AttachPlugin        ( model::BasicNodePtr node, UInt32 posIdx );
    bool                    AttachPlugin        ( model::BasicNodePtr destNode, model::BasicNodePtr srcNode, UInt32 posIdx );
    bool                    DetachPlugin        ( model::BasicNodePtr node, UInt32 posIdx );
    bool                    DetachPlugin        ( model::BasicNodePtr node, const std::string & name );
    
    model::IPluginPtr       GetDetachedPlugin   ( model::BasicNodePtr node ) const;
    void                    ResetDetachedPlugin ( model::BasicNodePtr node );

    /** Add a copy of plugin from srcNode with given name to the destNode on destIdx position.
    @return Returns copied plugin. */
    model::IPluginPtr       AddPluginCopy       ( model::SceneModelPtr destScene, model::BasicNodePtr destNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode, const std::string & pluginNameToCopy, bool enableUndo = false );

    bool                    MovePlugin          ( model::SceneModelPtr destScene, model::BasicNodePtr destNode, UInt32 destIdx, model::SceneModelPtr srcScene, model::BasicNodePtr srcNode, const std::string & pluginName, bool enableUndo = false );

/* NODE LOGICS */
    bool                    SetLogic            ( model::BasicNodePtr node, model::INodeLogicPtr logic, bool enableUndo = false );
    bool                    RemoveLogic         ( model::BasicNodePtr node, bool enableUndo = false );

/* EFFECTS */

    model::IModelNodeEffectPtr  GetNodeEffect   ( model::IModelNodePtr node ) const;
    bool                        SetNodeEffect   ( model::IModelNodePtr node, model::IModelNodeEffectPtr nodeEffect );
    bool                        SetNodeEffect   ( const std::string & sceneName, const std::string & nodePath, const std::string & timelinePath, const std::string & effectName, bool enableUndo = false );
    bool                        LoadGlobalEffectAsset( const std::string & sceneName, const std::string & nodePath, const std::string & timelinePath, const std::string & effectName, IDeserializer & serializedAssetData, SizeType idx );

/* TIMELINES */

    bool                    AddTimeline                 ( const std::string & parentTimelinePath, const std::string & timelineName, TimelineType timelineType, bool enableUndo = false );
    bool                    AddTimeline                 ( model::ITimeEvaluatorPtr parentTimeline, model::ITimeEvaluatorPtr timeline, bool enableUndo = false );
    
    bool                    DeleteTimeline              ( const std::string & timelinePath, bool enableUndo = false );
    bool                    ForceDeleteTimeline         ( const std::string & timelinePath, const std::string & newTimelinePath = std::string(), bool enableUndo = false );
    void                    ReplaceTimeline             ( model::SceneModelPtr scene, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline );

    bool                    RenameTimeline              ( const std::string & timelinePath, const std::string & newName, bool enableUndo = false );

    bool                    AssignTimeline              ( const std::string & sceneName, model::IParameterPtr param, const std::string & timeline, bool enableUndo = false );
    bool                    AssignTimeline              ( const std::string & sceneName, model::IParameterPtr param, model::ITimeEvaluatorPtr timeline, bool enableUndo = false );

    bool                    SetTimelineDuration         ( const std::string & timelinePath, TimeType duration );
    bool                    SetTimelineWrapPreBehavior  ( const std::string & timelinePath, TimelineWrapMethod preMethod );
    bool                    SetTimelineWrapPostBehavior ( const std::string & timelinePath, TimelineWrapMethod postMethod );

    model::ITimeEvaluatorPtr    GetTimeEvaluator        ( const std::string & timelinePath ) const;
    model::ITimelinePtr         GetTimeline             ( const std::string & timelinePath ) const;


/* ASSETS */

    bool                    LoadAsset                   ( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, IDeserializer & serializedAssetData );
    bool                    LoadAsset                   ( model::IPluginPtr plugin, AssetDescConstPtr assetDesc );

/* LIGHTS */

    bool                    AddLight                    ( const std::string & sceneName, const std::string & lightType, const std::string & timelinePath, bool enableUndo = false );
    bool                    RemoveLight                 ( const std::string & sceneName, UInt32 idx, bool enableUndo = false );

    bool                    AddLight                    ( model::SceneModelPtr scene, LightType type, model::ITimeEvaluatorPtr timeline, bool enableUndo = false );
    bool                    RemoveLight                 ( model::SceneModelPtr scene, UInt32 idx, bool enableUndo = false );

/* CAMERAS */

    bool                    AddCamera                   ( const std::string & sceneName );
    bool                    RemoveCamera                ( const std::string & sceneName, UInt32 idx );
    bool                    SetCurrentCamera            ( const std::string & sceneName, UInt32 idx );
    
    bool                    AddCamera                   ( model::SceneModelPtr scene );
    bool                    RemoveCamera                ( model::SceneModelPtr scene, UInt32 idx );
    bool                    SetCurrentCamera            ( model::SceneModelPtr scene, UInt32 idx );

/* UNDO REDO */

    bool                    Undo                        ( const std::string & sceneName, UInt16 numSteps );
    bool                    Redo                        ( const std::string & sceneName, UInt16 numSteps );

/* HELPERS */

    SceneNode *             GetEngineNode               ( model::IModelNodePtr node ) const;
    model::BasicNodePtr     FindModelNodeByPlugin       ( const model::IPlugin * plugin ) const;
    model::BasicNodePtr     FindModelNodeByPlugin       ( model::BasicNodePtr node, const model::IPlugin * plugin ) const;

private:

    /* scene helpers */
    void                    RefreshNode         ( model::BasicNodePtr modelNode, SceneNode * sceneNode, Renderer * renderer );
    
    void                    MappingsCleanup     ( model::IModelNodePtr node );
    void                    RemoveNodeMapping   ( model::IModelNodePtr node );
    void                    UnregisterUpdaters  ( model::IModelNodePtr node );

    void                    MappingsCleanup     ( model::SceneModelPtr modelScene );
    void                    RemoveSceneMapping  ( model::SceneModelPtr modelScene );
    void                    UnregisterUpdaters  ( model::SceneModelPtr modelScene );
    
    void                    AddModelScene       ( model::SceneModelPtr modelScene, UInt32 idx );
    void                    AddEngineScene      ( model::SceneModelPtr modelScene, UInt32 idx );

    bool                    RemoveModelScene    ( model::SceneModelPtr modelScene );
    bool                    RemoveEngineScene   ( model::SceneModelPtr modelScene );

    void                    SetSceneRootNode    ( model::SceneModelPtr modelScene, model::IModelNodePtr rootNode );
    void                    DeleteSceneRootNode ( model::SceneModelPtr modelScene );

    bool                    IsTimelineUsed      ( model::ITimeEvaluatorPtr timeEval );

    /* node helpers */

    void                    NotifyRemovedNode   ( model::BasicNodePtr removedNode, model::BasicNodePtr parentNode );
    void                    NotifyAddedNode     ( model::BasicNodePtr addedNode, model::BasicNodePtr parentNode );
    void                    NotifyMovedNode     ( model::BasicNodePtr node, model::BasicNodePtr srcParent, model::BasicNodePtr dstParent );
	void					NotifyCopiedNode	( model::BasicNodePtr  srcNode, model::BasicNodePtr  copy );


    void                    NotifyPluginAdded   ( model::BasicNodePtr parentNode, model::BasePluginPtr plugin );
    void                    NotifyPluginRemoved ( model::BasicNodePtr parentNode, model::BasePluginPtr plugin );
    void                    NotifyPluginMoved   ( model::BasePluginPtr plugin, model::BasicNodePtr srcParentNode, model::BasicNodePtr dstParentNode );

    void                    NotifyLogicAdded    ( model::BasicNodePtr parentNode, model::INodeLogicPtr logic );
    void                    NotifyLogicRemoved  ( model::BasicNodePtr parentNode, model::INodeLogicPtr logic );

    void                    NotifyEffectAdded   ( model::BasicNodePtr parentNode, model::IModelNodeEffectPtr effect );
    void                    NotifyEffectRemoved ( model::BasicNodePtr parentNode, model::IModelNodeEffectPtr effect );

    void                    NotifyCameraAdded   ( model::CameraModelPtr camera );
    void                    NotifyCameraRemoved ( model::CameraModelPtr camera );

    void                    NotifyLightAdded    ( model::IModelLightPtr camera );
    void                    NotifyLightRemoved  ( model::IModelLightPtr camera );

    /* Undo/Redo */

    void                    AddMoveOperation        ( model::SceneModelPtr scene, model::IModelNodePtr srcParent, model::IModelNodePtr destParent, model::IModelNodePtr movedNode, UInt32 destIdx );

    /* renaming helpers */
    std::string             PrefixSceneName     ( const std::string & name ) const;

    friend class BVProject;


// CUSTOM DEFAULTS

public:

    model::ITimelinePtr         GetSceneDefaultTimeline ( model::SceneModelConstPtr scene ) const;

private:

    void                        InitDefaultScene        ( model::SceneModelPtr scene );
    
    bool                        IsTimelineEditable      ( const model::ITimeEvaluator * timeEval ) const;
    
    static const std::string    DEFAULT_ROOT_NAME;
    static const std::string    DEFAULT_TIMELINE_NAME;

};

} //bv
