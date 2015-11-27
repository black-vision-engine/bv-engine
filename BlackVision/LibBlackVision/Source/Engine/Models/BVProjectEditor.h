#pragma once

#include <hash_map>
#include <string>

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

private:
	BVProject *					m_scene;
    model::BasicNodePtr			m_rootNode;

    SceneEditor *               m_engineSceneEditor;
    
	TNodesMapping	            m_nodesMapping;

	model::IPluginPtr			m_copiedPlugin;

private:

                            BVProjectEditor       ( BVProject * scene );

public:

    void                    AddScene			( model::SceneModelPtr scene );
    bool                    RemoveScene			( const std::string & sceneName );
    void                    RemoveAllScenes		();

	model::SceneModelPtr    GetScene			( const std::string & sceneName );
    void					SetSceneVisible		( const std::string & sceneName, bool visible );

    void                    SetSceneRootNode	( const std::string & sceneName, model::IModelNodePtr rootNode );
    bool                    DeleteSceneRootNode	( const std::string & sceneName );


    void                    AddChildNode        ( const std::string & sceneName, model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    bool                    DeleteChildNode     ( const std::string & sceneName, model::IModelNodePtr parentNode, const std::string & childNodeName );

    bool                    AttachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent );
    bool                    DetachChildNode     ( const std::string & sceneName, model::IModelNodePtr parent, const std::string & nodeToDetach );

    void                    DeleteDetachedNodes ( const std::string & sceneName );

	bool                    AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, unsigned int idx );
    bool                    DeletePlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    DeletePlugin		( model::BasicNodePtr node, const std::string & name );

    bool                    AttachPlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    AttachPlugin		( model::BasicNodePtr sourceNode, model::BasicNodePtr targetNode, unsigned int idx );
    bool                    DetachPlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    DetachPlugin		( model::BasicNodePtr node, const std::string & name );
    
	model::IPluginPtr		GetDetachedPlugin	( model::BasicNodePtr node );
    void                    ResetDetachedPlugin	( model::BasicNodePtr node );


	void					CopyPlugin			( model::BasicNodePtr node, const std::string & name );
	model::IPluginPtr		GetCopiedPlugin		();



	model::IModelNodeEffectPtr	GetNodeEffect   ( model::IModelNodePtr node );
    void						SetNodeEffect   ( model::IModelNodePtr node, model::IModelNodeEffectPtr nodeEffect );

private:

    void                    MappingsCleanup     ( model::IModelNodePtr node );

    void                    RemoveNodeMapping   ( model::IModelNodePtr node );
    void                    UnregisterUpdaters  ( model::IModelNodePtr node );

    SceneNode *             GetEngineNode       ( model::IModelNodePtr node );

    friend class BVProject;
};

} //bv
