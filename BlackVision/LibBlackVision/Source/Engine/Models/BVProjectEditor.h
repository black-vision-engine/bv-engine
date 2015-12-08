#pragma once

#include <hash_map>
#include <string>
#include <regex>

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

	static const std::string	COPY_PREFIX;
	static const std::string	COPY_REGEX;
	static const std::string	COPY_REPLACE;
	
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

    void                    AddScene			( model::SceneModelPtr scene );
    void                    AddEmptyScene		( const std::string & sceneName );

    bool                    RemoveScene			( const std::string & sceneName );
    void                    RemoveAllScenes		();

    bool                    DetachScene			( const std::string & sceneName );
    bool                    AttachScene			( const std::string & sceneName );
    void                    DeleteDetachedScenes();

	void					SetSceneVisible		( const std::string & sceneName, bool visible );
    void                    SetSceneRootNode	( const std::string & sceneName, model::IModelNodePtr rootNode );
    bool                    DeleteSceneRootNode	( const std::string & sceneName );

	model::SceneModelPtr    GetScene			( const std::string & sceneName );
	void					RenameScene			( const std::string & oldSceneName, std::string newSceneName );

	/** Add a copy of scene with given name to the project.
	@return Returns copied scene. */
	model::SceneModelPtr	AddSceneCopy		( const std::string & sceneNameToCopy );


    void                    AddChildNode        ( const std::string & sceneName, model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    bool                    DeleteChildNode     ( const std::string & sceneName, model::IModelNodePtr parentNode, const std::string & childNodeName );

    bool                    AttachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent );
    bool                    DetachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent, const std::string & nodeToDetach );

    void                    DeleteDetachedNodes ( const std::string & sceneName );

	/** Add a copy of node to the destParentNode from the given scene.
	@param[ destParentNode ] If nullptr, set node copy as root node of the scene.
	@return Returns copied node. */
	model::BasicNodePtr		AddNodeCopy			( const std::string & destSceneName, model::BasicNodePtr destParentNode, const std::string & srcSceneName, model::BasicNodePtr nodeToCopy );


	bool                    AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, unsigned int idx );
    bool                    DeletePlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    DeletePlugin		( model::BasicNodePtr node, const std::string & name );

    bool                    AttachPlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    AttachPlugin		( model::BasicNodePtr sourceNode, model::BasicNodePtr targetNode, unsigned int idx );
    bool                    DetachPlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    DetachPlugin		( model::BasicNodePtr node, const std::string & name );
    
	model::IPluginPtr		GetDetachedPlugin	( model::BasicNodePtr node );
    void                    ResetDetachedPlugin	( model::BasicNodePtr node );

	/** Add a copy of plugin from srcNode with given name to the destNode on destIdx position.
	@return Returns copied plugin. */
	model::IPluginPtr		AddPluginCopy		( model::BasicNodePtr destNode, model::BasicNodePtr srcNode, const std::string & pluginNameToCopy, unsigned int destIdx );



	model::IModelNodeEffectPtr	GetNodeEffect   ( model::IModelNodePtr node );
    void						SetNodeEffect   ( model::IModelNodePtr node, model::IModelNodeEffectPtr nodeEffect );

private:

    void                    MappingsCleanup     ( model::IModelNodePtr node );

    void                    RemoveNodeMapping   ( model::IModelNodePtr node );
    void                    UnregisterUpdaters  ( model::IModelNodePtr node );

    SceneNode *             GetEngineNode       ( model::IModelNodePtr node );


	std::string				GetScenePrefix		( const std::string & name );

    friend class BVProject;
};

} //bv
