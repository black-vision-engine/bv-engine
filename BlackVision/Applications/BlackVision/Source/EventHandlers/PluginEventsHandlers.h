#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;

class PluginEventsHandlers
{
private:
    BVAppLogic*             m_appLogic;
public:
    PluginEventsHandlers( BVAppLogic* logic );
    ~PluginEventsHandlers();

    void AddParamKey         ( bv::IEventPtr eventPtr );
    void UpdateParamKey      ( bv::IEventPtr eventPtr );
    void RemoveParamKey      ( bv::IEventPtr eventPtr );

    void LoadAsset           ( bv::IEventPtr eventPtr );
};


} //bv
