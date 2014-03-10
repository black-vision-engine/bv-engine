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
struct GlyphCoords;

// ***************************** UID **********************************
class TimerPluginUID
{
public:
    static const char*       GetName()        { return "timer_plugin"; }
};

// ***************************** PLUGIN ********************************** 
class TimerPlugin : public BasePlugin< IPlugin >
{
    ParamFloat                  m_timeParam;
    std::wstring                m_currentTime;

    VertexAttributesChannelPtr  m_vertexAttributeChannel;

    TextureInfoVec              m_textures;

    const ResourceHandle*       m_fontResource;
    const TextAtlas*            m_currentAtlas;

    std::wstring                m_timePatern;

    explicit                    TimerPlugin     ( const ParamFloat& timeParam, unsigned int fontSize );

    bool                        CheckTimeConsistency ( const std::wstring& time ) const;

    void                        SetValue        ( unsigned int connComp, wchar_t wch );
    const GlyphCoords&          GetGlyphCoords  ( wchar_t wch ) const;

public:

    static TimerPlugin*                         Create                      ( const ParamFloat& timeParam, unsigned int fontSize );

    void                                        SetTimePatern               ( const std::wstring& patern );
    void                                        SetTime                     ( const std::wstring& time );

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;
    virtual TextureInfoVec                      GetTextures                 () const override;

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const;
};

} // model
} // bv