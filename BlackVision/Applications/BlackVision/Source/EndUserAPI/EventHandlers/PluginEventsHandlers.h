#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;


typedef std::shared_ptr<model::IParameter> ParameterPtr;

class PluginEventsHandlers
{
private:
    BVAppLogic*             m_appLogic;
public:
    PluginEventsHandlers( BVAppLogic* logic );
    ~PluginEventsHandlers();

    void                    ParamHandler        ( bv::IEventPtr eventPtr );

    void                    LoadAsset           ( bv::IEventPtr eventPtr );
    void                    TimerHandler        ( bv::IEventPtr eventPtr );

private:    //Helpers
    void                    AddParameter        ( ParameterPtr& param, const std::wstring& value, TimeType keyTime );
    void                    RemoveParameter     ( ParameterPtr& param, TimeType keyTime );


    ParameterPtr            GetPluginParameter      (   const std::string& sceneName,
                                                        const std::string& nodeName,
                                                        const std::string& pluginName,
                                                        const std::string& paramName );
    ParameterPtr            GetGlobalEffectParameter(   const std::string& sceneName,
                                                        const std::string& nodeName,
                                                        const std::string& paramName );
    ParameterPtr            GetResourceParameter    (   const std::string& sceneName,
                                                        const std::string& nodeName,
                                                        const std::string& pluginName,
                                                        const std::string& textureName,
                                                        const std::string& paramName );
};

} //bv
