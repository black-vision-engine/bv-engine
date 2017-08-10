#pragma once


#include "GizmoLogicBase.h"
#include "GizmoFunMapper.h"

#include "Engine/Models/BasicNode.h"



namespace bv
{



// ***********************
// Per scene class for creating gizmos.
class GizmoManager
{
private:

    GizmoFunMapper      m_nodesGizmos;
    GizmoFunMapper      m_sceneGizmos;

public:


    void                RegisterDefaultGizmos       ();


private:

    std::string         QueryGizmoLogicName         ( model::GizmoType type, const std::string & ownerTypeName, const std::string & functionalityName );

    std::string         QueryPluginGizmoLogicName   ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QueryEffectGizmoLogicName   ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QueryLogicGizmoLogicName    ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QueryNodeGizmoLogicName     ( const std::string & ownerTypeName, const std::string & functionalityName );
    std::string         QuerySceneGizmoLogicName    ( const std::string & ownerTypeName, const std::string & functionalityName );


};


}	// bv


