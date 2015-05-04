#pragma once

#include <hash_map>
#include <string>

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

    TNodesMapping	        m_nodesMapping;

    BVScene *               m_pScene;

    model::IModelNodePtr    m_detachedModelNode;
    SceneNode *             m_detachedSceneNode;

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

    model::IModelNodePtr    GetDetachedNode     ();
    void                    DeleteDetachedNode  ();

    model::IModelNodePtr    GetRootNode         ();

private:

    //void                    AttachRootNodes     ( TNodesPair & nodesPair );

    //TNodesPair              DetachRootNodes     ();
    //TNodesPair              DetachNodes         ( model::BasicNodePtr parentNode, const std::string & childNodeName );

    //void                    AttachNodes         ( TNodesPair & parentPair, TNodesPair & childPair );

    //SceneNode *             GetEngineNode       ( model::BasicNodePtr node );

    //void                    UnregisterUpdaters  ( model::BasicNodePtr node );
    //void                    RemoveNodeMappings  ( model::BasicNodePtr node );

    //void                    DetachChildNode     ( model::BasicNodePtr parentNode, model::BasicNodePtr childNode );
    //void                    DeleteDetachedNodes ( model::BasicNodePtr modelNode, SceneNode * engineNode );

    BVScene *               S                   ();

    friend class BVScene;

};

} //bv

#include "BVSceneEditor.inl"
