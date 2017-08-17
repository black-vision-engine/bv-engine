#pragma once


#include "Engine/Models/BasicNode.h"


#include <map>


namespace bv
{


// ***********************
//
struct GizmoData
{
    model::BasicNodeWeakPtr     GizmoRoot;
};


// ***********************
//
class GizmoRegister
{
    typedef std::map< std::string, GizmoData > FunctionalityMap;
    typedef std::map< model::BasicNodeWeakPtr, FunctionalityMap, std::owner_less< model::BasicNodeWeakPtr > > GizmosMap;
private:

    GizmosMap       m_gizmosMap;

public:

    bool                Register        ( model::BasicNodePtr ownerNode, model::BasicNodePtr gizmoRoot, const std::string & functionality );
    bool                Unregister      ( model::BasicNodePtr ownerNode, const std::string & functionality );

    bool                Exists          ( model::BasicNodePtr ownerNode, const std::string & functionality );
    const GizmoData &   QueryGizmo      ( model::BasicNodePtr ownerNode, const std::string & functionality );
};


}	// bv

