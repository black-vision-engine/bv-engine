#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"



namespace bv
{

namespace model
{

class BasicNode;
class IModelNode;
class SceneModel;
class INodeLogic;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_PTR_TYPE( BasicNode );
DEFINE_PTR_TYPE( INodeLogic );
DEFINE_PTR_TYPE( IModelNode );

} // model



class RemoveNodeLogicOperation : public IRevertable
{
private:

    model::SceneModelPtr        m_scene;
    model::BasicNodePtr         m_parentNode;
    model::INodeLogicPtr        m_logic;

public:

    RemoveNodeLogicOperation    ( model::SceneModelPtr scene, model::IModelNodePtr parent, model::INodeLogicPtr logic );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( RemoveNodeLogicOperation )

}	// bv
