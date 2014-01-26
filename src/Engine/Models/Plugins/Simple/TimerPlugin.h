#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Resources/IResource.h"


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
    ParamFloat                  m_timeParam;
    float                       m_currentTime;

    VertexAttributesChannelPtr  m_vertexAttributeChannel;

    Textures                    m_textures;

    explicit                TimerPlugin( const ParamFloat& timeParam );

public:

    static TimerPlugin*                         Create                      ( const ParamFloat& timeParam );


    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel          () const override;
    virtual Textures                            GetTextures                 () const override;

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;
};

} // model
} // bv