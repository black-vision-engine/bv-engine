#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;


class NodeLogicHandlers
{
private:
    BVAppLogic*         m_appLogic;
public:
    NodeLogicHandlers( BVAppLogic* logic );
    ~NodeLogicHandlers();


    void        WidgetHandler       ( bv::IEventPtr evt );
};


} //bv
