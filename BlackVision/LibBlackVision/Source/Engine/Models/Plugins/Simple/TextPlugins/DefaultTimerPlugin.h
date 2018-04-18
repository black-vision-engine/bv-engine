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
#include <set>


namespace bv {

struct Glyph;

namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultTimerPluginDesc : public TextPluginBaseDesc
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

    explicit    TimeValue( TimeType time, int accuracy = 2 );

    bool        operator!=(const TimeValue& other) const;
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

// *************************************
//
bool            SetTimePaternTimerPlugin( IPluginPtr timerPlugin, const std::wstring & patern );


// ***************************** PLUGIN ********************************** 
/// @ingroup PluginsList
class DefaultTimerPlugin : public TextPluginBase
{
public:
    struct PARAM 
    {
        static const std::string        PRECISION;
		static const std::string        PRECISION_STOP;
    };

private:
    ParamIntPtr                     m_precisionParam;
	ParamIntPtr                     m_precisionStopParam;

    bool                            m_started;
    unsigned long                   m_globalStartTime;
    unsigned long                   m_localStartTime;
    unsigned long                   m_currentLocalTime;

    TimeValue                       m_currentTimeValue;
    wchar_t                         m_defaultSeparator;
    wchar_t                         m_secSeparator;
    wchar_t                         m_widestGlyph;
	wchar_t                         m_highestGlyph;

    std::wstring                    m_timePatern;

	std::wstring                    m_zerosTimeString;
	std::wstring                    m_timeString;
	std::set< SizeType >			m_constCharactersIdxs;

    void            SetValue            ( unsigned int connComp, wchar_t wch );
    const Glyph *   GetGlyph            ( wchar_t wch ) const;
    void            Refresh             ( bool isPaused );

    void            SetTimePatern       ( const std::wstring & patern );

private:

    
    void            SetTime             ( TimeType time, bool forceUpdate = false );

    void            SetTime             ( int h, int m, int s, int hoSec );

    virtual bool    LoadResource        ( AssetDescConstPtr assetDescr ) override;

	void			InitWidestGlyph		();

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
    friend bool     SetTimePaternTimerPlugin ( IPluginPtr timerPlugin, const std::wstring & patern );

public:
    explicit        DefaultTimerPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                    ~DefaultTimerPlugin ();
};




} // model
} // bv

