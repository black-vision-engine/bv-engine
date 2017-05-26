#pragma once

#include "Engine/Events/Events.h"


namespace bv
{

class BVAppLogic;
class BVProjectEditor;

typedef std::shared_ptr<model::IParameter> ParameterPtr;

class PluginEventsHandlers
{
private:

    BVProjectEditor *       m_projectEditor;

public:

                            PluginEventsHandlers    ( BVAppLogic * logic );
                            ~PluginEventsHandlers   ();

    void                    ParamHandler        ( bv::IEventPtr eventPtr );
    void                    TimerHandler        ( bv::IEventPtr eventPtr );
    void                    ParamDescHandler    ( bv::IEventPtr eventPtr );

private:

    //Helpers
    bool                    AddParameter        ( ParameterPtr & param, const std::string & stringValue, TimeType keyTime );
    bool                    RemoveParameter     ( ParameterPtr & param, TimeType keyTime );

    bool                    AddTransformKey         ( ParameterPtr & param, const std::string & paramSubName, TimeType keyTime, const std::string & strValue );
    bool                    RemoveTransformKey      ( ParameterPtr & param, const std::string & paramSubName, TimeType keyTime );
    bool                    MoveTransformKey        ( ParameterPtr & param, const std::string & paramSubName, TimeType keyTime, TimeType newTime );

};

} //bv
