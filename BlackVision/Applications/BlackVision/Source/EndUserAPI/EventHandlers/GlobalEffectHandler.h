#pragma once

#include "Engine/Events/Events.h"

namespace bv
{

class BVAppLogic;


class GlobalEffectHandler
{
private:
    BVAppLogic*         m_appLogic;
public:
    GlobalEffectHandler( BVAppLogic* appLogic );
    ~GlobalEffectHandler();

    void        GlobalEffectEventHandler			( bv::IEventPtr evt );
};



} //bv
