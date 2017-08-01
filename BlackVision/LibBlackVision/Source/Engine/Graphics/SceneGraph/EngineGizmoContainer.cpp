#include "stdafx.h"

#include "EngineGizmoContainer.h"

#include <algorithm>



namespace bv {


// ***********************
//
void            EngineGizmoContainer::AddGizmo        ( SceneNode * gizmoRoot )
{
    m_gizmoRoots.push_back( gizmoRoot );
}

// ***********************
//
void            EngineGizmoContainer::AddGizmo        ( SceneNode * gizmoRoot, UInt32 idx )
{
    if( idx > m_gizmoRoots.size() )
        idx = ( UInt32 )m_gizmoRoots.size();

    m_gizmoRoots.insert( m_gizmoRoots.begin() + idx, gizmoRoot );
}

// ***********************
//
void            EngineGizmoContainer::RemoveGizmo     ( SceneNode * gizmoRoot )
{
    auto iter = std::find( m_gizmoRoots.begin(), m_gizmoRoots.end(), gizmoRoot );
    m_gizmoRoots.erase( iter );
}

// ***********************
//
void            EngineGizmoContainer::RemoveGizmo     ( UInt32 idx )
{
    m_gizmoRoots.erase( m_gizmoRoots.begin() + idx );
}

// ***********************
//
SceneNode *     EngineGizmoContainer::GetGizmo        ( UInt32 idx ) const
{
    assert( idx < m_gizmoRoots.size() );
    return m_gizmoRoots[ idx ];
}

// ***********************
//
UInt32          EngineGizmoContainer::GetNumGizmos    () const
{
    return ( UInt32 )m_gizmoRoots.size();
}


}	// bv


