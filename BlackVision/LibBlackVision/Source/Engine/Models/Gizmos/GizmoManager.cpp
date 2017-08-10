#include "stdafx.h"
#include "GizmoManager.h"


#include "Engine/Editors/BVProjectEditor.h"

#include "Engine/Models/NodeLogics/NodeLogicFactory.h"


namespace bv
{



// ========================================================================= //
// Gizmo creation
// ========================================================================= //


// ***********************
//
bool                    GizmoManager::CreateGizmo           ( BVProjectEditor * editor, model::SceneModelPtr scene, model::BasicNodePtr gizmoOwner, model::GizmoType type, const std::string & ownerTypeName, const std::string & functionalityName )
{
    // If we create scene gizmo, we attach it always to root node. In this case user can pass nullptr.
    if( !gizmoOwner && type == model::GizmoType::Scene )
        gizmoOwner = scene->GetRootNode();

    if( gizmoOwner )
    {
        auto gizmoLogicName = QueryGizmoLogicName( type, ownerTypeName, functionalityName );

        if( !gizmoLogicName.empty() )
        {
            auto gizmoRoot = model::BasicNode::Create( "Gizmo_" + functionalityName, nullptr );
            auto gizmoLogic = GetNodeLogicFactory()->CreateGizmoLogic( gizmoLogicName, gizmoRoot, gizmoOwner, editor );

            if( gizmoLogic && gizmoRoot )
            {
                gizmoRoot->SetLogic( gizmoLogic );
                return editor->AddGizmoNode( scene, gizmoOwner, gizmoRoot );
            }
        }
    }

    return false;
}


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