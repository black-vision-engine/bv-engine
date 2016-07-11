#pragma once

#include "Engine/Models/UndoRedo/IRevertable.h"



namespace bv
{

namespace model
{

class BasicNode;
class IModelNode;
class SceneModel;
class INodeLogic;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_PTR_TYPE( IModelNode );
DEFINE_PTR_TYPE( BasicNode );
DEFINE_PTR_TYPE( INodeLogic );

} // model



class AddNodeLogicOperation : public IRevertable
{
private:

    model::SceneModelPtr        m_scene;
    model::BasicNodePtr         m_parentNode;
    model::INodeLogicPtr        m_logic;

public:

    AddNodeLogicOperation       ( model::SceneModelPtr scene, model::IModelNodePtr parent, model::INodeLogicPtr logic );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( AddNodeLogicOperation )

}	// bv
