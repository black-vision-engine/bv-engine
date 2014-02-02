#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Resources/IResource.h"

#include <map>


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

    std::map< int, glm::vec2 >  m_digits;

    VertexAttributesChannelPtr  m_vertexAttributeChannel;

    Textures                    m_textures;

    const ResourceHandle*       m_fontResource;
    const TextAtlas*            m_currentAtlas;

    std::string                 m_timePatern;

    explicit                    TimerPlugin     ( const ParamFloat& timeParam, unsigned int fontSize );

    unsigned int                GetSecond       ( float t );
    unsigned int                GetHOAS         ( float t );
    unsigned int                GetMinute       ( float t );
    unsigned int                GetHour         ( float t );

    void                        BuildDigitsMap  ();

    bool                        CheckTimeConsistency ( const std::string& time ) const;

public:

    static TimerPlugin*                         Create                      ( const ParamFloat& timeParam, unsigned int fontSize );

    void                                        SetTimePatern               ( const std::string& patern );
    void                                        SetTime                     ( const std::string& time );

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel          () const override;
    virtual Textures                            GetTextures                 () const override;

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;
};

} // model
} // bv