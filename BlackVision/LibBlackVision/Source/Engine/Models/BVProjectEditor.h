#pragma once

#include <hash_map>
#include <string>
//#include <regex>

#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/SceneModel.h"


namespace bv {

namespace model {
    class ModelSceneEditor;
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

    void                    SetSceneRootNode	( const std::string & sceneName, model::IModelNodePtr rootNode );
    bool                    DeleteSceneRootNode	( const std::string & sceneName );

	model::SceneModelPtr    GetScene			( const std::string & sceneName );

	bool					RenameScene			( const std::string & oldSceneName, const std::string & newSceneName );

	/** Add a copy of scene with given name to the project.
	@return Returns copied scene. */
	model::SceneModelPtr	AddSceneCopy		( const std::string & sceneNameToCopy );

	bool					MoveScene			( const std::string & sceneName, UInt32 posIdx );


/* NODES */
    model::IModelNodePtr		GetNode         ( const std::string & sceneName, const std::string & nodePath );
	model::IModelNodePtr		GetParentNode   ( const std::string & sceneName, const std::string & nodePath );

	/* paths */
    void                    AddChildNode        ( const std::string & sceneName, const std::string & parentPath, const std::string & newNodeName );
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

	/* objects */
    void                    AddChildNode        ( const std::string & sceneName, model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    bool                    DeleteChildNode     ( const std::string & sceneName, model::IModelNodePtr parentNode, const std::string & childNodeName );
	
	/** Insert node at the end. */
    bool                    AttachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent );
	/** Insert node at destIdx position. */
    bool                    AttachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent, UInt32 posIdx );
	bool                    DetachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent, const std::string & nodeToDetach );

	/** Add a copy of node to the destParentNode from the given scene.
	@param[ destParentNode ] If nullptr, set node copy as root node of the scene.
	@return Returns copied node. */
	model::BasicNodePtr		AddNodeCopy			( const std::string & destSceneName, model::BasicNodePtr destParentNode, const std::string & srcSceneName, model::BasicNodePtr srcNode );
	
	bool					MoveNode			( const std::string & destSceneName, model::BasicNodePtr destParentNode, UInt32 destIdx, const std::string & srcSceneName, model::BasicNodePtr srcParentNode, const std::string & srcNodeName );

	void					SetNodeVisible		( model::IModelNodePtr node, bool visible );

	bool					RenameNode			( model::IModelNodePtr node, const std::string & newNodeName );


    void                    DeleteDetachedNodes ( const std::string & sceneName );

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
    
	model::IPluginPtr		GetDetachedPlugin	( model::BasicNodePtr node );
    void                    ResetDetachedPlugin	( model::BasicNodePtr node );

	/** Add a copy of plugin from srcNode with given name to the destNode on destIdx position.
	@return Returns copied plugin. */
	model::IPluginPtr		AddPluginCopy		( const std::string & destSceneName, model::BasicNodePtr destNode, UInt32 destIdx, const std::string & srcSceneName, model::BasicNodePtr srcNode, const std::string & pluginNameToCopy );

	bool					MovePlugin			( const std::string & destSceneName, model::BasicNodePtr destNode, UInt32 destIdx, const std::string & srcSceneName, model::BasicNodePtr srcNode, const std::string & pluginName );


/* EFFECTS */

	model::IModelNodeEffectPtr	GetNodeEffect   ( model::IModelNodePtr node );
    void						SetNodeEffect   ( model::IModelNodePtr node, model::IModelNodeEffectPtr nodeEffect );
    bool                        SetNodeEffect   ( const std::string& sceneName, const std::string& nodePath, const std::string& timelinePath, const std::string& effectName );

/* TIMELINES */

    model::ITimeEvaluatorPtr	GetTimeline     ( const std::string & timelinePath );

	//FIXME: what about ConstTimeEvaluator & OffsetTimeEvaluator?
	void                    AddTimeline					( const std::string & sceneName, const std::string & timelineName, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );
	void                    AddTimeline					( const std::string & sceneName, model::ITimeEvaluatorPtr timeline );
	
	bool                    DeleteTimeline				( const std::string & timelinePath );
	void                    ForceDeleteTimeline			( const std::string & timelinePath, const std::string & newTimelinePath = std::string() );

	bool                    RenameTimeline				( const std::string & timelinePath, const std::string & newName );

	void                    SetTimelineDuration			( const std::string & timelinePath, TimeType duration );
	void                    SetTimelineWrapPreBehavior	( const std::string & timelinePath, TimelineWrapMethod preMethod );
	void                    SetTimelineWrapPostBehavior	( const std::string & timelinePath, TimelineWrapMethod postMethod );

/* ASSETS */

	bool					LoadAsset					( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, const std::string & serializedAssetData );
	bool					LoadAsset					( model::IPluginPtr plugin, AssetDescConstPtr assetDesc );

private:

	/* engine scene helpers */
    void                    MappingsCleanup     ( model::IModelNodePtr node );

    void                    RemoveNodeMapping   ( model::IModelNodePtr node );
    void                    UnregisterUpdaters  ( model::IModelNodePtr node );

    SceneNode *             GetEngineNode       ( model::IModelNodePtr node );

	
	/* model scene helpers */
	std::string					GetNodeName			( const std::string & nodePath );


	/* renaming helpers */
	std::string				PrefixSceneName		( const std::string & name );

    friend class BVProject;
};

} //bv
