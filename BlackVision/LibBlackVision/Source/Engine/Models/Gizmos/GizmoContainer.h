#pragma once

#include "Engine/Models/Interfaces/IGizmoLogic.h"
#include "Engine/Models/Interfaces/IModelNode.h"



#include <vector>


namespace bv {
namespace model
{



// ***********************
//
class GizmoContainer
{
private:

    std::vector< IModelNodePtr >        m_gizmoRoots;

public:

    void            AddGizmo        ( IModelNodePtr gizmoRoot );
    void            AddGizmo        ( IModelNodePtr gizmoRoot, UInt32 idx );
    void            RemoveGizmo     ( IModelNodePtr gizmoRoot );
    void            RemoveGizmo     ( UInt32 idx );

    IModelNodePtr   GetGizmo        ( UInt32 idx ) const;
    UInt32          GetNumGizmos    () const;

    void            Update			    ( TimeType t );
    void            PreNodeUpdate       ( TimeType t );
    void            PostChildrenUpdate  ( TimeType t );
};


DEFINE_UPTR_TYPE( GizmoContainer );


}
}	// bv

