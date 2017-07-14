#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"

namespace bv {

namespace model {

class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
class IModelLight;
DEFINE_PTR_TYPE( IModelLight );

} // model

class AddLightOperation : public IRevertable
{
    model::SceneModelPtr    m_parent;
    model::IModelLightPtr   m_light;

public:
                            AddLightOperation  ( model::SceneModelPtr parent, model::IModelLightPtr light );

    virtual bool            Undo                ( BVProjectEditor * editor );
    virtual bool            Redo                ( BVProjectEditor * editor );
};

class DeleteLightOperation : public IRevertable
{
    model::SceneModelPtr    m_parent;
    model::IModelLightPtr   m_light;
    int                     m_idx;

public:
                            DeleteLightOperation ( model::SceneModelPtr parent, model::IModelLightPtr light, int idx );

    virtual bool            Undo                ( BVProjectEditor * editor );
    virtual bool            Redo                ( BVProjectEditor * editor );
};

} // bv
