#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"



namespace bv
{

namespace model
{

class IModelNode;
class BasicNode;
class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_PTR_TYPE( IModelNode );
DEFINE_PTR_TYPE( BasicNode );

} // model



class MoveNodeOperation : public IRevertable
{
private:

    model::SceneModelPtr        m_scene;
    model::BasicNodePtr         m_srcParentNode;
    model::BasicNodePtr         m_destParentNode;
    model::BasicNodePtr         m_movedNode;

    UInt32                      m_srcIdx;
    UInt32                      m_destIdx;

public:

    MoveNodeOperation           (   model::SceneModelPtr scene,
                                    model::IModelNodePtr srcParent,
                                    model::IModelNodePtr destParent,
                                    model::IModelNodePtr movedNode,
                                    UInt32 srcIdx,
                                    UInt32 destIdx );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( MoveNodeOperation )

}	// bv