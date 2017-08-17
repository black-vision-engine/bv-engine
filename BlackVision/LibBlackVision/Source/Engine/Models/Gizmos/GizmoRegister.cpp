#include "stdafx.h"
#include "GizmoRegister.h"




namespace bv
{




// ***********************
//
bool            GizmoRegister::Register     ( model::BasicNodePtr ownerNode, model::BasicNodePtr gizmoRoot, const std::string & functionality )
{
    auto & functionsMap = m_gizmosMap[ ownerNode ];
    functionsMap[ functionality ].GizmoRoot = gizmoRoot;

    return true;
}

// ***********************
//
bool            GizmoRegister::Unregister   ( model::BasicNodePtr ownerNode, const std::string & functionality )
{
    auto iter = m_gizmosMap.find( ownerNode );
    if( iter == m_gizmosMap.end() )
        return false;

    auto & functionsMap = iter->second;

    auto funIter = functionsMap.find( functionality );
    if( funIter == functionsMap.end() )
        return false;

    functionsMap.erase( funIter );

    if( functionsMap.empty() )
        m_gizmosMap.erase( iter );

    return true;
}

// ***********************
//
bool            GizmoRegister::Exists           ( model::BasicNodePtr ownerNode, const std::string & functionality )
{
    auto iter = m_gizmosMap.find( ownerNode );
    if( iter == m_gizmosMap.end() )
        return false;

    auto & functionsMap = iter->second;

    auto funIter = functionsMap.find( functionality );
    if( funIter == functionsMap.end() )
        return false;

    return true;
}

// ***********************
//
const GizmoData &       GizmoRegister::QueryGizmo       ( model::BasicNodePtr ownerNode, const std::string & functionality )
{
    auto & functionsMap = m_gizmosMap[ ownerNode ];
    return functionsMap[ functionality ];
}

}	// bv

