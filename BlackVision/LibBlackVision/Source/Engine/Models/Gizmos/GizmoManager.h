#pragma once


#include "GizmoLogicBase.h"
#include "GizmoFunMapper.h"

#include "Engine/Models/BasicNode.h"

#include "GizmoRegister.h"



namespace bv
{

namespace model
{
    class SceneModel;
    DEFINE_PTR_TYPE( SceneModel );
}

class BVProjectEditor;



// ***********************
// Per scene class for creating gizmos.
class GizmoManager
{
private:

    GizmoFunMapper      m_nodesGizmos;
    GizmoFunMapper      m_sceneGizmos;

    GizmoRegister       m_existingGizmos;

public:


    void                RegisterDefaultGizmos       ();


    bool                CreateGizmo                 ( BVProjectEditor * editor, model::SceneModelPtr scene, model::BasicNodePtr gizmoOwner, model::GizmoType type, const std::string & ownerTypeName, const std::string & functionalityName );
    bool                RemoveGizmo                 ( BVProjectEditor * editor, model::SceneModelPtr scene, model::BasicNodePtr gizmoOwner, const std::string & functionalityName );


    const GizmoData *   GetGizmoRootNode            ( model::BasicNodePtr gizmoOwner, const std::string & functionalityName );

private:

    std::string         QueryGizmoLogicName         ( model::GizmoType type, const std::string & ownerTypeName, const std::string & functionalityName );

    std::string         QueryPluginGizmoLogicName   ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QueryEffectGizmoLogicName   ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QueryLogicGizmoLogicName    ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QueryNodeGizmoLogicName     ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QuerySceneGizmoLogicName    ( const std::string & ownerTypeName, const std::string & functionalityName );


};


}	// bv


