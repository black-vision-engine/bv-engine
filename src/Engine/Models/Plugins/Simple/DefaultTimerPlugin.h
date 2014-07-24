#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Resources/IResource.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Timeline/TimeSegmentEvalImpl.h"

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
};

class Resource;
class FontExtraData;
class TextAtlas;
class Text;
struct GlyphCoords;

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
private:

    ParamFloatPtr                   m_timeParam;
    ParamFloatPtr                   m_fontSizeParam;
    ParamFloatPtr                   m_blurSizeParam;
    ParamFloatPtr                   m_spacingParam;
    ParamFloatPtr                   m_alignmentParam;

    TimeSegmentEvalImpl             m_timeEvaluator;

    TimeValue                       m_currentTime;
    const wchar_t                   m_defaultSeparator;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    TextureInfoVec                  m_textures;

    ResourceHandleConstPtr          m_fontResource;
    const TextAtlas *               m_textAtlas;

    std::wstring                    m_timePatern;
    TimeInfo                        m_timePaternInfo;
    DefaultPluginParamValModelPtr   m_paramValModel;

    explicit                        DefaultTimerPlugin     ( const ParamFloat& timeParam, unsigned int fontSize );

    bool                            CheckTimeConsistency ( const std::wstring & time ) const;

    void                            SetValue        ( unsigned int connComp, wchar_t wch );
    const GlyphCoords &             GetGlyphCoords  ( wchar_t wch ) const;
    void                            Refresh         ();

    void                            SetTimePatern   ( const std::wstring & patern );

    std::wstring                    GenerateTimePatern( double time );
public:

    
    void                                        SetTime                     ( double time );

    void                                        SetTime                     ( int h, int m, int s, int hoSec );

    explicit                                    DefaultTimerPlugin          ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTimerPlugin         ();

    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    void                                        Start                       ();
    void                                        Stop                        ();
    void                                        Reset                       ();

private:

    void                                        InitAttributesChannel       ( IPluginPtr prev );
};


// *************************************
//
bool            StartTimerPlugin( IPluginPtr timerPlugin );

// *************************************
//
bool            StopTimerPlugin( IPluginPtr timerPlugin );

// *************************************
//
bool            ResetTimerPlugin( IPluginPtr timerPlugin );

} // model
} // bv

