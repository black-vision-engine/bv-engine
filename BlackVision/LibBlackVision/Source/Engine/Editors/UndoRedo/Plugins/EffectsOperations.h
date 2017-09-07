#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"

namespace bv {

namespace model {

class BasicNode;
DEFINE_PTR_TYPE( BasicNode );
class IModelNodeEffect;
DEFINE_PTR_TYPE( IModelNodeEffect );

} // model



class SetEffectOperation : public IRevertable
{
    model::BasicNodePtr     m_parent;
    model::IModelNodeEffectPtr m_oldEffect;
    model::IModelNodeEffectPtr m_newEffect;

public:
                            SetEffectOperation  ( model::BasicNodePtr parent, model::IModelNodeEffectPtr oldEffect, model::IModelNodeEffectPtr newEffect );

    virtual bool            Undo                ( BVProjectEditor * editor );
    virtual bool            Redo                ( BVProjectEditor * editor );
};

} // bv
