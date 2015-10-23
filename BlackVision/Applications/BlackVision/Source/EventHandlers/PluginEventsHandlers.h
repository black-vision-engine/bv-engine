#pragma once

#include "Engine/Events/Events.h"

namespace bv
{

class PluginEventsHandlers
{
public:
    PluginEventsHandlers();
    ~PluginEventsHandlers();

    static void AddParamKey         ( bv::IEventPtr eventPtr );
    static void UpdateParamKey      ( bv::IEventPtr eventPtr );
    static void RemoveParamKey      ( bv::IEventPtr eventPtr );

    static void LoadAsset           ( bv::IEventPtr eventPtr );
};


} //bv
