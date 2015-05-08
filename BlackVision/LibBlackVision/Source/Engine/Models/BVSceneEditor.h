#pragma once

#include <hash_map>
#include <string>

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"

namespace bv {

namespace model {
    class ModelSceneEditor;
} // model

class BVScene;
class SceneNode;
class SceneEditor;


class BVSceneEditor
{
private:

    typedef std::hash_map< model::IModelNode *, SceneNode * >	TNodesMapping;
    typedef std::pair< model::IModelNodePtr, SceneNode * >      TNodesPair;

private:
	BVScene *					m_scene;
    TNodesMapping	            m_nodesMapping;

    model::ModelSceneEditor *   m_modelSceneEditor;
    SceneEditor *               m_engineSceneEditor;

private:

                            BVSceneEditor       ( BVScene * scene );

public:

    void                    SetRootNode         ( model::IModelNodePtr rootNode );
    bool                    DeleteRootNode      ();

    void                    AddChildNode        ( model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    bool                    DeleteChildNode     ( model::IModelNodePtr parentNode, const std::string & childNodeName );

    void                    AttachRootNode      ();
    bool                    DetachRootNode      ();

    bool                    AttachChildNode     ( model::IModelNodePtr parent );
    bool                    DetachChildNode     ( model::IModelNodePtr parent, const std::string & nodeToDetach );

    void                    DeleteDetachedNodes ();

    model::IModelNodePtr    GetRootNode         ();

	
	void                    AddPlugin			( model::BasicNodePtr node, model::IPluginPtr plugin, unsigned int idx );
    bool                    DeletePlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    DeletePlugin		( model::BasicNodePtr node, const std::string & name );

    bool                    AttachPlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    AttachPlugin		( model::BasicNodePtr sourceNode, model::BasicNodePtr targetNode, unsigned int idx );
    bool                    DetachPlugin		( model::BasicNodePtr node, unsigned int idx );
    bool                    DetachPlugin		( model::BasicNodePtr node, const std::string & name );
    

private:

    void                    MappingsCleanup     ( model::IModelNodePtr node );

    void                    RemoveNodeMapping   ( model::IModelNodePtr node );
    void                    UnregisterUpdaters  ( model::IModelNodePtr node );

    SceneNode *             GetEngineNode       ( model::IModelNodePtr node );

    friend class BVScene;
};

} //bv
