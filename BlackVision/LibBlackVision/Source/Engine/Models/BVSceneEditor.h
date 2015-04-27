#pragma once

#include <hash_map>
#include <string>

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"


namespace bv {

class BVScene;
class SceneNode;
class Renderer;


class BVSceneEditor
{
private:

    typedef std::hash_map< model::IModelNode *, SceneNode * >	TNodesMapping;

private:

    TNodesMapping	m_nodesMapping;

    BVScene *       m_pScene;

private:

                            BVSceneEditor       ( BVScene * scene );

public:

    void                    SetRootNode         ( model::IModelNodePtr rootNode );
    bool                    DeleteRootNode      ();

    void                    AttachRootNode      ( model::IModelNodePtr rootNode );
    model::IModelNodePtr    DetachRootNode      ();

    void                    AddChildNode        ( model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    bool                    DeleteChildNode     ( model::IModelNodePtr parentNode, const std::string & childNodeName );

    void                    AttachChildNode     ( model::IModelNodePtr parent, model::IModelNodePtr nodeToAttach );
    model::IModelNodePtr    DetachChildNode     ( model::IModelNodePtr parent, const std::string & nodeToDetach );


private:

    void                    UnregisterUpdaters  ( model::BasicNodePtr node );
    void                    RemoveNodeMappings  ( model::BasicNodePtr node );

    void                    DetachChildNode     ( model::BasicNodePtr parentNode, model::BasicNodePtr childNode );
    void                    DeleteDetachedNodes ( model::BasicNodePtr modelNode, SceneNode * engineNode );

    BVScene *               S                   ();

    friend class BVScene;

};

} //bv

#include "BVSceneEditor.inl"
