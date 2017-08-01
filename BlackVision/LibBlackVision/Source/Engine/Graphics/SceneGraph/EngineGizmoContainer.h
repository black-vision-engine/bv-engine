#pragma once


#include <vector>



namespace bv
{

class SceneNode;


// ***********************
//
class EngineGizmoContainer
{
private:

    std::vector< SceneNode * >      m_gizmoRoots;

public:

    void            AddGizmo        ( SceneNode * gizmoRoot );
    void            AddGizmo        ( SceneNode * gizmoRoot, UInt32 idx );
    void            RemoveGizmo     ( SceneNode * gizmoRoot );
    void            RemoveGizmo     ( UInt32 idx );

    SceneNode *     GetGizmo        ( UInt32 idx ) const;
    UInt32          GetNumGizmos    () const;


};

DEFINE_UPTR_TYPE( EngineGizmoContainer );



}	// bv

