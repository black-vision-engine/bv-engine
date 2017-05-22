#include "stdafx.h"

#include "DefaultTimerPlugin.h"

#include "Assets/Font/TextHelper.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultFontDescriptor.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"
#include "Assets/Font/Glyph.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/DefaultAssets.h"

#include "Tools/SimpleTimer.h"

#include "Application/ApplicationContext.h"

#include "Memory/MemoryLeaks.h"

#include <algorithm>
#include <regex>


namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

const std::string        DefaultTimerPlugin::PARAM::PRECISION           = "precision";
const std::string        DefaultTimerPlugin::PARAM::PRECISION_STOP		= "precisionStop";

// *******************************
//
DefaultTimerPluginDesc::DefaultTimerPluginDesc                            ()
    : TextPluginBaseDesc( UID(), "timer", "txt" ) // FIXME: it does not really look right :/
{
}

// *******************************
//
IPluginPtr              DefaultTimerPluginDesc::CreatePlugin             ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTimerPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTimerPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    auto model = TextPluginBaseDesc::CreateDefaultModel( timeEvaluator );

    ModelHelper h( timeEvaluator, model );

    h.SetOrCreatePluginModel();

    h.AddSimpleParam( DefaultTimerPlugin::PARAM::PRECISION, 1 );
	h.AddSimpleParam( DefaultTimerPlugin::PARAM::PRECISION_STOP, 2 );

    return model;
}

// *******************************
//
std::string             DefaultTimerPluginDesc::UID                      ()
{
    return "DEFAULT_TIMER";
}

// *******************************
//
std::string             DefaultTimerPluginDesc::TextureName              ()
{
    return "AtlasTex0";
}

////////////////////////////
//
namespace
{

////////////////////////////
//
bool IsPlaceHolderChar( wchar_t wch )
{
	wch = towlower( wch );

	const static wchar_t schars [] = { L'h', L'm', L's', L'd' };

	static std::set< wchar_t > scSet( schars, schars + sizeof( schars ) / sizeof( schars[ 0 ] ) );

	return scSet.find( wch ) != scSet.end();
}

////////////////////////////
//
std::vector< std::pair< std::wstring, Int32 > > GetTimePaternGroups( std::wstring s )
{
	std::wregex word_regex( L"H+|M+|S+|D+|[hmsd]|[^HMSDhmsd]+" );
	auto words_begin = std::wsregex_iterator( s.begin(), s.end(), word_regex );
	auto words_end = std::wsregex_iterator();

	std::vector< std::pair< std::wstring, Int32 > > ret;

	for ( std::wsregex_iterator i = words_begin; i != words_end; ++i )
	{
		std::wsmatch match = *i;

		auto matchStr = match.str();

		if( IsPlaceHolderChar( matchStr[ 0 ] ) )
		{
			ret.push_back( std::make_pair( matchStr.substr( 0, 1 ), isupper( matchStr[ 0 ] ) ? ( Int32 )matchStr.size() : 0 ) ); // H, M, S, D (length), h, m, s, d (0) 
		}
		else
		{
			ret.push_back( std::make_pair( matchStr, -1 ) ); // other strings.
		}

		
		// std::wcout << L"  " << match_str << L'\n';
	}

	return ret;
}

////////////////////////////
//
void AddToTimeStringU( Int32 value, std::wstring & zeroTimeString, std::wstring & timeString, Int32 groupLength, wchar_t widestChar )
{
	auto rounded = value % Int32 ( std::pow( 10, groupLength ) );

	const auto str = std::to_wstring( rounded );

	for( auto i = 0; i < groupLength; ++i )
	{
		zeroTimeString.push_back( widestChar );
		if( i < groupLength - str.size() )
			timeString.push_back( L'0' );
	}

	timeString.append( str );
}

////////////////////////////
//
void AddToTimeStringL( Int32 value, std::wstring & zeroTimeString, std::wstring & timeString, wchar_t widestChar )
{
	auto str = std::to_wstring( value );

	zeroTimeString.append( std::wstring( str.size(), widestChar ) );

	timeString.append( str );
}

////////////////////////////
//
std::pair< std::wstring, std::wstring > CreateTimeString( std::vector< std::pair< std::wstring, Int32 > > timePaternGroups, TimeValue timeValue, wchar_t widestChar, std::set< SizeType > & consCharactersIdx )
{
	std::wstring zeroTimeString;
	std::wstring timeString;

	for( const auto & group : timePaternGroups )
	{
		if( group.first == L"H" )
		{
			AddToTimeStringU( timeValue.hour, zeroTimeString, timeString, group.second, widestChar );
		}
		else if ( group.first == L"M" )
		{
			AddToTimeStringU( timeValue.minute, zeroTimeString, timeString, group.second, widestChar );
		}
		else if( group.first == L"S" )
		{
			AddToTimeStringU( timeValue.second, zeroTimeString, timeString, group.second, widestChar );
		}
		else if( group.first == L"D" )
		{
			AddToTimeStringU( timeValue.fracOfSecond, zeroTimeString, timeString, group.second, widestChar );
		}
		else if( group.first == L"h" )
		{
			AddToTimeStringL( timeValue.hour, zeroTimeString, timeString, widestChar );
		}
		else if( group.first == L"m" )
		{
			AddToTimeStringL( timeValue.minute, zeroTimeString, timeString, widestChar );
		}
		else if( group.first == L"s" )
		{
			AddToTimeStringL( timeValue.second, zeroTimeString, timeString, widestChar );
		}
		else if( group.first == L"d" )
		{
			AddToTimeStringL( timeValue.fracOfSecond, zeroTimeString, timeString, widestChar );
		}
		else
		{
			for( SizeType i = zeroTimeString.size(); i < zeroTimeString.size() + group.first.size(); ++i )
			{
				consCharactersIdx.insert( i );
			}
			
			zeroTimeString.append( group.first );
			timeString.append( group.first );
		}
	}

	return std::make_pair( zeroTimeString, timeString );
}

////////////////////////////
//
void TimeFormatError()
{
    throw std::exception("wrong time format");
}

////////////////////////////
//
std::pair< std::wstring, std::wstring > ParseTimePatern( const std::wstring& timePatern, TimeValue timeValue, wchar_t widestChar, std::set< SizeType > & consCharactersIdx )
{
	auto groups = GetTimePaternGroups( timePatern );

	auto timeStrings = CreateTimeString( groups, timeValue, widestChar, consCharactersIdx );

	return timeStrings;
}

} // anonymous


////////////////////////////
//
bool    TimeValue::operator!=(const TimeValue& other) const
{
    return  other.fracOfSecond != this->fracOfSecond
        ||  other.second != this->second
        ||  other.minute != this->minute
        ||  other.hour != this->hour;
}

////////////////////////////
//
TimeValue::TimeValue( TimeType time, int accuracy )
{
    this->fracOfSecond = int( ( time - std::floor( time ) ) * pow( 10, accuracy ) );
    this->second    = int( std::floor( time ) ) % 60;
    this->minute    = int( std::floor( time ) / 60 ) % 60;
    this->hour      = int( std::floor( time ) / (60 * 60 ) );
}

// *************************************
//
DefaultTimerPlugin::DefaultTimerPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : TextPluginBase( name, uid, prev, model )
    , m_timePatern( )
    , m_globalStartTime( 0 )
    , m_localStartTime( 0 )
    , m_currentTimeValue( 0.0 )
    , m_defaultSeparator(L':')
    , m_secSeparator(L'.')
    , m_widestGlyph( L'0' ) 
	, m_highestGlyph( L'0' )
    , m_started(false)
{
    SetPrevPlugin( prev );

	m_precisionParam = QueryTypedParam< ParamIntPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( PARAM::PRECISION ) );
	m_precisionStopParam = QueryTypedParam< ParamIntPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( PARAM::PRECISION_STOP ) );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< FontAssetDesc >() );
}

// *************************************
//
DefaultTimerPlugin::~DefaultTimerPlugin  ( )
{}

// *************************************
// 
bool            DefaultTimerPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto success = TextPluginBase::LoadResource( assetDescr, DefaultTimerPluginDesc::TextureName() );
	auto txAssetDescr = QueryTypedDesc< FontAssetDescConstPtr >( assetDescr );

	InitWidestGlyph();

    if( success )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        m_zerosTimeString = L""; // force reseting zeros time patern. New font's loading.

        SetTimePatern( L"h:m:s.dd" );

        SetTime( m_currentLocalTime / 1000.f, true );

        return true;
    }    
    else
    {
        return false;
    }
}

// *************************************
// 
void                                DefaultTimerPlugin::Update                      ( TimeType t )
{
    //FIXME: UPDATER TO FIX
    //FIXME: why GetTickCount() instead of TimeGetTime or something similar and more stable
    if( m_started )
    {
        m_currentLocalTime = m_localStartTime + (SimpleTimer::GetTickCount() -  m_globalStartTime);
        /*m_currentLocalTime = m_localStartTime + ((unsigned long)( t * 1000.0f ) -  m_globalStartTime);*/
    }

    SetTime( m_currentLocalTime / 1000.f );

    TextPluginBase::Update( t );

    //assumption that text plugin provides vertices, so no need for backward topology propagation
    HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, GetPrevPlugin() );
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );
    
    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
// 
void                                DefaultTimerPlugin::Start                       ()
{
    if(!m_started)
    {
        m_started = true;
        m_globalStartTime = SimpleTimer::GetTickCount(); // FIXME: why not use TimeGetTime which is more stable?
    }
}

// *************************************
// 
void                                DefaultTimerPlugin::Stop                        ()
{
    if(m_started)
    {
        m_started = false;
        m_localStartTime = m_currentLocalTime;
        Refresh( true );
    }
}

// *************************************
// 
void                                DefaultTimerPlugin::Reset                       ( float localTime )
{
    m_currentLocalTime = unsigned long ( localTime * 1000.f );
    m_localStartTime = m_currentLocalTime;
    m_globalStartTime = SimpleTimer::GetTickCount(); // FIXME: why not use TimeGetTime which is more stable?
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetTimePatern  ( const std::wstring& patern )
{
    m_timePatern = patern;
	
	std::wstring zerosTimeString;

	auto acc = m_started ? m_precisionParam->Evaluate() : m_precisionStopParam->Evaluate();

	m_constCharactersIdxs.clear();

	std::tie( zerosTimeString, m_timeString ) = ParseTimePatern( m_timePatern, TimeValue( m_currentLocalTime / 1000.f, acc ), m_widestGlyph, m_constCharactersIdxs );

	if( zerosTimeString != m_zerosTimeString )
	{
		m_vaChannel->ClearAll();

		m_zerosTimeString = zerosTimeString;

		TextPluginBase::BuildVACForText( m_zerosTimeString, false, false );

		m_currentTimeValue = TimeValue( 0, acc ); // Force update time.

		HelperVertexAttributesChannel::SetTopologyUpdate(m_vaChannel);

		SetTime( m_currentLocalTime / 1000.f, true );
	}

	
}

////////////////////////////
//
const Glyph *                       DefaultTimerPlugin::GetGlyph        ( wchar_t wch ) const
{
    auto glyph = m_atlas->GetGlyph( wch );
    if( glyph )
    {
        return glyph;
    }
    else
    {
        assert( !( "Cannot find glyph for char " + wch) );
        throw;
    }
}

////////////////////////////
//
void                                DefaultTimerPlugin::Refresh         ( bool isPaused )
{
	{
		isPaused;
	}

	auto cp = m_timeString;
	cp.erase( std::remove( cp.begin(), cp.end(), L' ' ), cp.end() );
	cp.erase( std::remove( cp.begin(), cp.end(), L'\n' ), cp.end() );

	for( auto it = cp.begin(); it != cp.end(); ++it )
	{
		auto i = std::distance( cp.begin(), it );

		if( m_constCharactersIdxs.find( i ) == m_constCharactersIdxs.end() )
		{
			SetValue( ( UInt32 ) i, *it );
		}
	}
}

////////////////////////////
//
void                              DefaultTimerPlugin::InitWidestGlyph ()
{
	SizeType width = 0;
	SizeType height = 0;
	wchar_t widest = L'0';
	wchar_t highest = L'0';
	
	static const std::wstring numbers = L"0123456789";
	
	for( auto wch : numbers )
	{
		auto w = m_atlas->GetGlyph( wch )->width;
		if( w > width )
		{
			width = w;
			widest = wch;
		}

		auto h = m_atlas->GetGlyph( wch )->height;
		if( h > height )
		{
			height = h;
			highest = wch;
		}

	}
	
	m_highestGlyph = highest;
	m_widestGlyph = widest;
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetValue       ( unsigned int connComp, wchar_t wch )
{
    auto comps = m_vaChannel->GetComponents();

    if( wch != L' ' )
    {
        auto glyph = GetGlyph( wch );

		auto widestGlyph = GetGlyph( m_widestGlyph );
		auto highestGlyph = GetGlyph( m_highestGlyph );
		
		auto uvsWidestGlyph = TextHelper::GetAtlasCoordsForGlyph( widestGlyph, m_atlas->GetWidth(), m_atlas->GetHeight(), ( Float32 ) m_blurSize );
		auto uvsHighestGlyph = TextHelper::GetAtlasCoordsForGlyph( highestGlyph, m_atlas->GetWidth(), m_atlas->GetHeight(), ( Float32 ) m_blurSize );

		glm::vec2 wWH = uvsWidestGlyph[ 1 ] - uvsWidestGlyph[ 2 ];
		glm::vec2 hWH = uvsHighestGlyph[ 1 ] - uvsHighestGlyph[ 2 ];

        if( connComp < comps.size() )
        {
            if( comps[ connComp ]->GetNumVertices() == 4 )
            {
                auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( comps[ connComp ] );
                auto uvChannel = std::static_pointer_cast< Float2AttributeChannel >( prevConnComp->GetAttrChannel( AttributeSemantic::AS_ATLASCOORD ) );

                assert( uvChannel );

                auto& verts = uvChannel->GetVertices();

                auto uvs = TextHelper::GetAtlasCoordsForGlyph( glyph, m_atlas->GetWidth(), m_atlas->GetHeight(), ( Float32 ) m_blurSize );

                verts[ 0 ] = glm::vec2( uvs[ 1 ].x - wWH.x, uvs[ 1 ].y );
                verts[ 1 ] = glm::vec2( uvs[ 1 ].x, uvs[ 1 ].y );
                verts[ 2 ] = glm::vec2( uvs[ 1 ].x - wWH.x, uvs[ 1 ].y - hWH.y );
                verts[ 3 ] = glm::vec2( uvs[ 1 ].x, uvs[ 1 ].y - hWH.y );
            }
        }
    }
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetTime        ( TimeType time, bool forceUpdate )
{
	auto acc = m_started ? m_precisionParam->Evaluate() : m_precisionStopParam->Evaluate();

    TimeValue   newTime( time, acc );

    if( forceUpdate || m_currentTimeValue != newTime )
    {
        m_currentTimeValue = newTime;
		SetTimePatern( m_timePatern );

        Refresh( !m_started );

		HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
    }
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetTime         ( int h, int m, int s, int hoSec )
{
    assert( hoSec < 100 );
    SetTime( TimeType( h * 3600. + m * 60 + s + hoSec / 100.f ) );
}

// *************************************
//
bool            SetTimeTimerPlugin( IPluginPtr timerPlugin, TimeType time )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Reset( time );
        return true;
    }
    else
        return false;
}

// *************************************
//
bool            StartTimerPlugin( IPluginPtr timerPlugin )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Start();
        return true;
    }
    else
        return false;
}

// *************************************
//
bool            StopTimerPlugin( IPluginPtr timerPlugin )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Stop();
        return true;
    }
    else
        return false;
}

// *************************************
//
bool            ResetTimerPlugin( IPluginPtr timerPlugin )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Reset( 0.f );
        return true;
    }
    else
        return false;
}

// *************************************
//
bool            SetTimePaternTimerPlugin ( IPluginPtr timerPlugin, const std::wstring & patern )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->SetTimePatern( patern );
        return true;
    }
    else
        return false;
}

} // model
} // bv