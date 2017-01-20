#pragma once

#include "Engine/Events/EventManager.h"

namespace bv {

class BVAppLogic;


class TestInnerEvents
{
private:

public:
    explicit                TestInnerEvents();
    virtual                 ~TestInnerEvents();

    void          NodeAdded         ( bv::IEventPtr evt );
    void          NodeRemoved       ( bv::IEventPtr evt );
    void          NodeMoved         ( bv::IEventPtr evt );
    void          PluginAdded       ( bv::IEventPtr evt );
    void          PluginMoved       ( bv::IEventPtr evt );
    void          PluginRemoved     ( bv::IEventPtr evt );
    void          LogicAdded        ( bv::IEventPtr evt );
    void          LogicRemoved      ( bv::IEventPtr evt );
    void          EffectAdded       ( bv::IEventPtr evt );
    void          EffectRemoved     ( bv::IEventPtr evt );

private:
};

} //bv


