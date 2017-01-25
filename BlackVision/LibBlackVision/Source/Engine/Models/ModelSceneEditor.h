#pragma once

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"


namespace bv { namespace model {

class ModelSceneEditor
{
private:

    BasicNodePtr &			m_rootNode;
    BasicNodePtr            m_detachedNode;

private:

                            ModelSceneEditor    ( const ModelSceneEditor & other ); 
    ModelSceneEditor &      operator =          ( const ModelSceneEditor & other );

public:

                            ModelSceneEditor    ( BasicNodePtr & rootNode );

    void                    SetRootNode         ( BasicNodePtr mainRootNode, BasicNodePtr sceneRootNode );
    bool                    DeleteRootNode      ( BasicNodePtr mainRootNode );

    void                    AddChildNode        ( BasicNodePtr parentNode, BasicNodePtr childNode );
    bool                    DeleteChildNode     ( BasicNodePtr parentNode, BasicNodePtr childNode );

    void                    AttachRootNode      ();
    bool                    DetachRootNode      ();

    bool                    AttachChildNode     ( BasicNodePtr parent );
    bool                    AttachChildNode     ( BasicNodePtr parent, UInt32 destIdx );
    bool                    DetachChildNode     ( BasicNodePtr parent, BasicNodePtr nodeToDetach );

    BasicNodePtr            GetDetachedNode     () const;
    void                    DeleteDetachedNode  ();

    BasicNodePtr            GetRootNode         () const;

    IModelNodePtr           GetNode             ( const std::string & path, const std::string & separator = "/" ) const;

};

} // model
} //bv
