#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Events/BaseEvent.h"


namespace bv { namespace model {

class Resource;
class FontExtraData;
class TextAtlas;
class Text;

// ***************************** UID **********************************
class TimerPluginUID
{
public:
    static const char*       GetName()        { return "timer_plugin"; }
};

// ***************************** PLUGIN ********************************** 
class TimerPlugin : public BasePlugin< IPlugin, TimerPluginUID >
{
private:
};

} // model
} // bv