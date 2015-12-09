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

    void                    AddParamKey         ( bv::IEventPtr eventPtr );
    void                    UpdateParamKey      ( bv::IEventPtr eventPtr );
    void                    RemoveParamKey      ( bv::IEventPtr eventPtr );

    void                    LoadAsset           ( bv::IEventPtr eventPtr );
    void                    TimerHandler        ( bv::IEventPtr eventPtr );

private:    //Helpers
    void                    AddParameter        ( std::shared_ptr<model::IParameter>& param, const std::wstring& value, TimeType keyTime );
    void                    UpdateParameter     ( std::shared_ptr<model::IParameter>& param, const std::wstring& value, TimeType keyTime );
    void                    RemoveParameter     ( std::shared_ptr<model::IParameter>& param, TimeType keyTime );
};


} //bv
