#pragma once

#include "Engine/Models/BasicNode.h"


namespace bv {


class ModelSceneEditor
{
private:

    model::BasicNodePtr &   m_rootNode;
    model::BasicNodePtr     m_detachedNode;

private:

                            ModelSceneEditor    ( const ModelSceneEditor & other ); 
    ModelSceneEditor &      operator =          ( const ModelSceneEditor & other );

private:

                            ModelSceneEditor    ( model::BasicNodePtr & rootNode );

public:

    void                    SetRootNode         ( model::BasicNodePtr rootNode );
    bool                    DeleteRootNode      ();

    void                    AddChildNode        ( model::BasicNodePtr parentNode, model::BasicNodePtr childNode );
    bool                    DeleteChildNode     ( model::BasicNodePtr parentNode, const std::string & childNodeName );

    void                    AttachRootNode      ();
    bool                    DetachRootNode      ();

    bool                    AttachChildNode     ( model::BasicNodePtr parent );
    bool                    DetachChildNode     ( model::BasicNodePtr parent, const std::string & nodeToDetach );

    model::BasicNodePtr     GetDetachedNode     ();
    void                    DeleteDetachedNode  ();

    model::BasicNodePtr     GetRootNode         ();

};

} //bv
