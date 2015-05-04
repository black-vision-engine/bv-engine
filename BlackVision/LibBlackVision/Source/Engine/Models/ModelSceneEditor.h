#pragma once

#include "Engine/Models/BasicNode.h"


namespace bv { namespace model {

class ModelSceneEditor
{
private:

    BasicNodePtr &          m_rootNode;
    BasicNodePtr            m_detachedNode;

private:

                            ModelSceneEditor    ( const ModelSceneEditor & other ); 
    ModelSceneEditor &      operator =          ( const ModelSceneEditor & other );

public:

                            ModelSceneEditor    ( BasicNodePtr & rootNode );

                            void                    SetRootNode         ( BasicNodePtr rootNode );
    bool                    DeleteRootNode      ();

    void                    AddChildNode        ( BasicNodePtr parentNode, BasicNodePtr childNode );
    bool                    DeleteChildNode     ( BasicNodePtr parentNode, const std::string & childNodeName );

    void                    AttachRootNode      ();
    bool                    DetachRootNode      ();

    bool                    AttachChildNode     ( BasicNodePtr parent );
    bool                    DetachChildNode     ( BasicNodePtr parent, const std::string & nodeToDetach );

    BasicNodePtr            GetDetachedNode     ();
    void                    DeleteDetachedNode  ();

    BasicNodePtr            GetRootNode         ();

};

} // model
} //bv
