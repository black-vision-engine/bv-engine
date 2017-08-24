#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;


class GizmoHandlers
{
private:

    BVAppLogic*             m_appLogic;

public:

    explicit        GizmoHandlers( BVAppLogic* logic );
                    ~GizmoHandlers();

    void    GizmoHandler            ( IEventPtr evt );


private:

    void     ListGizmos             ( GizmoEventPtr gizmoEvt, const std::string & sceneName, const std::string gizmoOwnerNodeName, model::GizmoType type, const std::string & ownerTypeName );
};

} //bv
