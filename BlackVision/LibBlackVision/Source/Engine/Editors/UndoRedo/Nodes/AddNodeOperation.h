#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"



namespace bv
{

namespace model
{

class IModelNode;
class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_PTR_TYPE( IModelNode );

} // model


// Add node and copy node operation.
class AddNodeOperation : public IRevertable
{
private:

    model::SceneModelPtr        m_scene;
    model::IModelNodePtr        m_parentNode;
    model::IModelNodePtr        m_addedNode;

public:

    AddNodeOperation            ( model::SceneModelPtr scene, model::IModelNodePtr parent, model::IModelNodePtr addedNode );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( AddNodeOperation )

}	// bv
