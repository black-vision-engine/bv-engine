#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Resources/IResource.h"

#include <map>


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultTimerPluginDesc : public BasePluginDescriptor
{
public:

    DefaultTimerPluginDesc                                      ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();

    static  std::string                     VertexShaderSource  ();
    static  std::string                     PixelShaderSource   ();

    static  std::string                     TextureName         ();
    static  std::string                     FontFileName        ();
};

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

struct TimeValue
{
    int     fracOfSecond;
    int     second;
    int     minute;
    int     hour;

    explicit    TimeValue( double time, int accuracy = 2 );

    bool        operator!=(const TimeValue& other) const;
};

struct TimeInfo
{
    int hoursPHStart;
    int hoursPlaceholderSize; // 0..
    int minutesPHStart;
    int minutesPlaceHolderSize; // 0..2
    int secondsPHStart;
    int secondsPlaceHolderSize; // 0..2
    int fosPHStart;
    int fracOfSecondsPlaceholderSize; // 0..

    int GetSize() const;
};

// ***************************** PLUGIN ********************************** 
class DefaultTimerPlugin : public BasePlugin< IPlugin >
{
    ParamFloat                  m_timeParam;
    TimeValue                   m_currentTime;
    const wchar_t               m_defaultSeparator;

    VertexAttributesChannelPtr  m_vertexAttributeChannel;

    TextureInfoVec              m_textures;

    ResourceHandleConstPtr      m_fontResource;
    const TextAtlas *           m_currentAtlas;

    std::wstring                m_timePatern;
    TimeInfo                    m_timePaternInfo;

    explicit                    TimerPlugin     ( const ParamFloat& timeParam, unsigned int fontSize );

    bool                        CheckTimeConsistency ( const std::wstring & time ) const;

    void                        SetValue        ( unsigned int connComp, wchar_t wch );
    const GlyphCoords &         GetGlyphCoords  ( wchar_t wch ) const;
    void                        Refresh         ();

public:

    static TimerPlugin *                        Create                      ( const ParamFloat & timeParam, unsigned int fontSize );

    void                                        SetTimePatern               ( const std::wstring & patern );
    void                                        SetTime                     ( const std::wstring & time );
    void                                        SetTime                     ( double time );
    void                                        SetHOSecond                 ( int hoSec );
    void                                        SetSecond                   ( int sec );
    void                                        SetMinute                   ( int min );
    void                                        SetHour                     ( int h );

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual TextureInfoVec                      GetTextures                 () const; //FIXME: remove

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const;
};

} // model
} // bv