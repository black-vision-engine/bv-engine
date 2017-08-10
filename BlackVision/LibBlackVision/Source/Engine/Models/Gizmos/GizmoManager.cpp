#include "stdafx.h"
#include "GizmoManager.h"




namespace bv
{



// ========================================================================= //
// Gizmo functionalities mapping
// ========================================================================= //


// ***********************
//
void                GizmoManager::RegisterDefaultGizmos     ()
{
    // TODO: Here place registration of default scene and node gizmos.
}

// ***********************
//
std::string         GizmoManager::QueryGizmoLogicName       ( model::GizmoType type, const std::string & ownerTypeName, const std::string & functionalityName )
{
    switch( type )
    {
    case model::GizmoType::Scene:
        return QuerySceneGizmoLogicName( ownerTypeName, functionalityName );
    case model::GizmoType::Node:
        return QueryNodeGizmoLogicName( ownerTypeName, functionalityName );
    case model::GizmoType::Logic:
        return QueryLogicGizmoLogicName( ownerTypeName, functionalityName );
    case model::GizmoType::Plugin:
        return QueryPluginGizmoLogicName( ownerTypeName, functionalityName );
    case model::GizmoType::Effect:
        return QueryEffectGizmoLogicName( ownerTypeName, functionalityName );
    }

    return "";
}

// ***********************
//
std::string         GizmoManager::QueryPluginGizmoLogicName ( const std::string & ownerTypeName, const std::string & functionalityName )
{
    ownerTypeName;
    functionalityName;
    assert( !"Implement me" );
    return std::string();
}

// ***********************
//
std::string         GizmoManager::QueryEffectGizmoLogicName ( const std::string & ownerTypeName, const std::string & functionalityName )
{
    ownerTypeName;
    functionalityName;
    assert( !"Implement me" );
    return std::string();
}

// ***********************
//
std::string         GizmoManager::QueryLogicGizmoLogicName  ( const std::string & ownerTypeName, const std::string & functionalityName )
{
    ownerTypeName;
    functionalityName;
    assert( !"Implement me" );
    return std::string();
}

// ***********************
//
std::string         GizmoManager::QueryNodeGizmoLogicName   ( const std::string & , const std::string & functionalityName )
{
    return m_nodesGizmos.QueryGizmoLogicName( functionalityName );
}

// ***********************
//
std::string         GizmoManager::QuerySceneGizmoLogicName  ( const std::string & , const std::string & functionalityName )
{
    return m_sceneGizmos.QueryGizmoLogicName( functionalityName );
}


}	// bv