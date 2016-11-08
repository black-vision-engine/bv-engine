#pragma once

#include "TextPluginBase.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/Font/2D/TextAtlas.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Timeline/TimeSegmentEvalImpl.h"

#include <map>


namespace bv {

struct Glyph;

namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultTimerPluginDesc : public BasePluginDescriptor
{
public:

    DefaultTimerPluginDesc                                      ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();
};

class Resource;
class FontExtraData;
class TextAtlas;
class Text;

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

// *************************************
//
bool            SetTimeTimerPlugin( IPluginPtr timerPlugin, TimeType time );

// *************************************
//
bool            StartTimerPlugin( IPluginPtr timerPlugin );

// *************************************
//
bool            StopTimerPlugin( IPluginPtr timerPlugin );

// *************************************
//
bool            ResetTimerPlugin( IPluginPtr timerPlugin );


// ***************************** PLUGIN ********************************** 
class DefaultTimerPlugin : public TextPluginBase
{
private:
    ParamFloatPtr                   m_precisionParam;

    bool                            m_started;
    unsigned long                   m_globalStartTime;
    unsigned long                   m_localStartTime;
    unsigned long                   m_currentLocalTime;

    TimeValue                       m_currentTimeValue;
    wchar_t                         m_defaultSeparator;
    wchar_t                         m_secSeparator;
    wchar_t                         m_widestGlyph;

    std::wstring                    m_timePatern;
    TimeInfo                        m_timePaternInfo;

    bool            CheckTimeConsistency( const std::wstring & time ) const;

    void            SetValue            ( unsigned int connComp, wchar_t wch );
    const Glyph *   GetGlyph            ( wchar_t wch ) const;
    void            Refresh             ( bool isPaused );

    void            SetTimePatern       ( const std::wstring & patern );

    std::wstring    GenerateTimePatern  ( double time );
    void            InitBigestGlyph     ();
private:

    
    void            SetTime             ( TimeType time );

    void            SetTime             ( int h, int m, int s, int hoSec );

    virtual bool    LoadResource        ( AssetDescConstPtr assetDescr ) override;

private:

    virtual void    Update              ( TimeType t ) override;

    void            Start               ();
    void            Stop                ();
    void            Reset               ( float localTime );

private:

    friend bool     SetTimeTimerPlugin  ( IPluginPtr timerPlugin, TimeType time );
    friend bool     StartTimerPlugin    ( IPluginPtr timerPlugin );
    friend bool     StopTimerPlugin     ( IPluginPtr timerPlugin );
    friend bool     ResetTimerPlugin    ( IPluginPtr timerPlugin );

public:
    explicit        DefaultTimerPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                    ~DefaultTimerPlugin ();
};




} // model
} // bv

