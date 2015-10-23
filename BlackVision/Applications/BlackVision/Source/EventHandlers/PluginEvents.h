#pragma once

#include "Engine/Events/Events.h"

namespace bv
{

class PluginEvents
{
public:
    PluginEvents();
    ~PluginEvents();

    static void SetParam            ( bv::IEventPtr evt );
    static void LoadAsset           ( bv::IEventPtr evt );
};


} //bv
