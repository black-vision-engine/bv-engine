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
    GizmoHandlers( BVAppLogic* logic );
    ~GizmoHandlers();

    void    GizmoHandler            ( IEventPtr evt );

};

} //bv
