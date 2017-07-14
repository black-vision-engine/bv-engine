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



class RemoveNodeOperation : public IRevertable
{
private:

    model::SceneModelPtr        m_scene;
    model::IModelNodePtr        m_parentNode;
    model::IModelNodePtr        m_removedNode;

public:

    RemoveNodeOperation         ( model::SceneModelPtr scene, model::IModelNodePtr parent, model::IModelNodePtr removedNode );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( RemoveNodeOperation )

}	// bv
