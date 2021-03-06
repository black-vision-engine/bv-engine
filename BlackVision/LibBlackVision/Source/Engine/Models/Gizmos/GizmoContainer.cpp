#include "stdafx.h"

#include "GizmoContainer.h"


#include <algorithm>



namespace bv {
namespace model
{


// ***********************
//
void            GizmoContainer::AddGizmo        ( IModelNodePtr gizmoRoot )
{
    m_gizmoRoots.push_back( gizmoRoot );
}

// ***********************
//
void            GizmoContainer::AddGizmo        ( IModelNodePtr gizmoRoot, UInt32 idx )
{
    if( idx > m_gizmoRoots.size() )
        idx = (UInt32)m_gizmoRoots.size();

    m_gizmoRoots.insert( m_gizmoRoots.begin() + idx, gizmoRoot );
}

// ***********************
//
void            GizmoContainer::RemoveGizmo     ( IModelNodePtr gizmoRoot )
{
    auto iter = std::find( m_gizmoRoots.begin(), m_gizmoRoots.end(), gizmoRoot );
    m_gizmoRoots.erase( iter );
}

// ***********************
//
void            GizmoContainer::RemoveGizmo     ( UInt32 idx )
{
    m_gizmoRoots.erase( m_gizmoRoots.begin() + idx );
}

// ***********************
//
IModelNodePtr   GizmoContainer::GetGizmo        ( UInt32 idx ) const
{
    assert( idx < m_gizmoRoots.size() );
    return m_gizmoRoots[ idx ];
}

// ***********************
//
UInt32          GizmoContainer::GetNumGizmos    () const
{
    return (UInt32)m_gizmoRoots.size();
}

// ========================================================================= //
// Gizmos update
// ========================================================================= //


// ***********************
//
void            GizmoContainer::Update              ( TimeType t )
{
    for( auto & gizmoRoot : m_gizmoRoots )
    {
        BasicNodePtr node = std::static_pointer_cast< BasicNode >( gizmoRoot );
        auto gizmoLogic = GetGizmoLogic( gizmoRoot );

        node->Update( t );
    }
}

// ***********************
//
void            GizmoContainer::PreOwnerUpdate       ( TimeType t )
{
    for( auto & gizmoRoot : m_gizmoRoots )
    {
        auto gizmoLogic = GetGizmoLogic( gizmoRoot );
        gizmoLogic->PreOwnerUpdate( t );
    }
}

// ***********************
//
void            GizmoContainer::PostOwnerUpdate  ( TimeType t )
{
    for( auto & gizmoRoot : m_gizmoRoots )
    {
        auto gizmoLogic = GetGizmoLogic( gizmoRoot );
        gizmoLogic->PostOwnerUpdate( t );
    }
}

// ========================================================================= //
// Private implementation
// ========================================================================= //

// ***********************
//
IGizmoLogicPtr  GizmoContainer::GetGizmoLogic   ( const IModelNodePtr & modelNode )
{
    BasicNodePtr node = std::static_pointer_cast< BasicNode >( modelNode );
    auto gizmoLogic = std::static_pointer_cast< model::IGizmoLogic >( node->GetLogic() );

    assert( gizmoLogic );

    return gizmoLogic;
}


}   // model
}	// bv


