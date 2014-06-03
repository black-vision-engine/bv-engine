#include "TimerPlugin.h"

#include "Engine/Models/Resources/Font/TextHelper.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Resources/Font/FontLoader.h"
#include "Engine/Models/Resources/Font/Text.h"

namespace bv { namespace model {

////////////////////////////
//
namespace
{

bool IsPlaceHolder(wchar_t wch)
{
    return  wch == L'H'
        ||  wch == L'M'
        ||  wch == L'S'
        ||  wch == L's';
}

void TimeFormatError()
{
    throw std::exception("wrong time format");
}

TimeInfo ParseTimePatern(const std::wstring& timePatern)
{
    bool HPHPossible = true;
    bool MPHPossible = true;
    bool SPHPossible = true;
    bool FSPHPossible = true;

    TimeInfo timeInfo = {0,0,0,0};

    int i = 0;
    int whiteSpaces = 0;
    for( auto wch : timePatern )
    {
        if(IsPlaceHolder(wch))
        {
            switch ( wch )
            {
            case L'H':
                if( !HPHPossible ) TimeFormatError();
                if( timeInfo.hoursPlaceholderSize == 0 ) timeInfo.hoursPHStart = i - whiteSpaces;
                timeInfo.hoursPlaceholderSize++;
                break;
            case L'M':
                if(!MPHPossible) TimeFormatError();
                if( timeInfo.minutesPlaceHolderSize == 0 ) timeInfo.minutesPHStart = i - whiteSpaces;
                HPHPossible = false;
                if(timeInfo.minutesPlaceHolderSize < 2)
                    timeInfo.minutesPlaceHolderSize++;
                else
                    TimeFormatError();
                break;
            case L'S':
                if(!SPHPossible) TimeFormatError();
                if( timeInfo.secondsPlaceHolderSize == 0 ) timeInfo.secondsPHStart = i - whiteSpaces;
                HPHPossible = false;
                MPHPossible = false;
                if(timeInfo.secondsPlaceHolderSize < 2)
                    timeInfo.secondsPlaceHolderSize++;
                else
                    TimeFormatError();
                break;
            case L's':
                if( timeInfo.fracOfSecondsPlaceholderSize == 0 ) timeInfo.fosPHStart = i - whiteSpaces;
                HPHPossible = false;
                MPHPossible = false;
                SPHPossible = false;
                if(!FSPHPossible) TimeFormatError();
                timeInfo.fracOfSecondsPlaceholderSize++;
                break;
            }
        }

        if(isspace(wch))
            whiteSpaces++;

        ++i;
    }

    return timeInfo;
}

} // anonymous


////////////////////////////
//
bool    TimeValue::operator!=(const TimeValue& other) const
{
    return  other.fracOfSecond == this->fracOfSecond
        ||  other.second == this->second
        ||  other.minute == this->minute
        ||  other.hour == this->hour;
}

////////////////////////////
//
TimeValue::TimeValue( double time, int accuracy )
{
    this->fracOfSecond = int( ( time - floor( time ) ) * pow( 10, accuracy ) );
    this->second    = int( time ) % 60;
    this->minute    = int( time / 60 ) % 60;
    this->hour      = int( time / (60 * 60 ) );
}

////////////////////////////
//
TimerPlugin::TimerPlugin( const ParamFloat& timeParam, unsigned int fontSize )
    : BasePlugin( "dupa", "dupa", nullptr, nullptr )
    , m_timeParam( timeParam )
    , m_fontResource()
    , m_currentAtlas()
    , m_timePatern( L"HHHH:MM:SS:ssssss" )
    , m_currentTime( 0.0 )
    , m_defaultSeparator(L':')
{
    m_timePaternInfo = ParseTimePatern( m_timePatern );

    m_fontResource = TextHelper::LoadFont( "../dep/Media/fonts/digital-7.ttf", fontSize, L"../dep/Media/fonts/TimerChars.txt" );

    m_currentAtlas = TextHelper::GetAtlas( m_fontResource );

    auto textureResource = TextHelper::GetAtlasTextureInfo( m_currentAtlas );

    m_textures.push_back( new TextureInfo( textureResource, "AtlasTex" ) );

    m_vertexAttributeChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );

    TextHelper::BuildVACForText( m_vertexAttributeChannel.get(), m_currentAtlas, L"0000:00:00:000000", m_timePatern );
}

////////////////////////////
//
TimerPlugin*                        TimerPlugin::Create     ( const ParamFloat& timeParam, unsigned int fontSize )
{
    return new TimerPlugin( timeParam, fontSize );
}

////////////////////////////
//
void                                TimerPlugin::SetTimePatern  ( const std::wstring& patern )
{
    m_timePatern = patern;
}

////////////////////////////
//
const GlyphCoords&                  TimerPlugin::GetGlyphCoords  ( wchar_t wch ) const
{
    auto glyphCoords = m_currentAtlas->GetGlyphCoords( wch );
    if( glyphCoords )
        return *glyphCoords;
    else
    {
        assert( !( "Cannot find glyph for char " + wch) );
        throw;
    }
}

////////////////////////////
//
int TimeInfo::GetSize() const
{
    return  hoursPlaceholderSize
        +   minutesPlaceHolderSize
        +   secondsPlaceHolderSize
        +   fracOfSecondsPlaceholderSize;
}

////////////////////////////
//
void                                TimerPlugin::Refresh         ()
{
    int hPHSize = m_timePaternInfo.hoursPlaceholderSize;
    int shift = m_timePaternInfo.hoursPHStart;
    if( hPHSize > 0 )
    {
        int hour = m_currentTime.hour;
        auto hourStr = std::to_wstring( hour );

        int zerosBefore = hPHSize > (int)hourStr.size() ? ( hPHSize - (int)hourStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < hPHSize; ++i )
        {
            SetValue( shift + i, hourStr[i - zerosBefore] );
        }       
    }

    shift = m_timePaternInfo.minutesPHStart;
    int mPHSize = m_timePaternInfo.minutesPlaceHolderSize;
    if( mPHSize > 0 )
    {
        int minute = m_currentTime.minute;
        auto minuteStr = std::to_wstring( minute );

        int zerosBefore = mPHSize > (int)minuteStr.size() ? ( mPHSize - (int)minuteStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < mPHSize; ++i )
        {
            SetValue( shift + i, minuteStr[i - zerosBefore] );
        }       
    }

    shift = m_timePaternInfo.secondsPHStart;
    int sPHSize = m_timePaternInfo.secondsPlaceHolderSize;
    if( sPHSize > 0 )
    {
        int second = m_currentTime.second;
        auto secondStr = std::to_wstring( second );

        int zerosBefore = sPHSize > (int)secondStr.size() ? ( sPHSize - (int)secondStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < sPHSize; ++i )
        {
            SetValue( shift + i, secondStr[i - zerosBefore] );
        }       
    }

    shift = m_timePaternInfo.fosPHStart;
    int fosPHSize = m_timePaternInfo.fracOfSecondsPlaceholderSize;
    if( fosPHSize > 0 )
    {
        int fos = m_currentTime.fracOfSecond;
        auto fosStr = std::to_wstring( fos );

        int zerosBefore = fosPHSize > (int)fosStr.size() ? ( fosPHSize - (int)fosStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < fosPHSize; ++i )
        {
            SetValue( shift + i, fosStr[i - zerosBefore] );
        }       
    }
}

////////////////////////////
//
void                                TimerPlugin::SetValue       ( unsigned int connComp, wchar_t wch )
{
    auto comps = m_vertexAttributeChannel->GetComponents();

    auto& coords = GetGlyphCoords( wch );

    auto textureXNorm = ( float )coords.textureX / m_currentAtlas->GetWidth();
    auto textureYNorm = ( float )coords.textureY / m_currentAtlas->GetHeight();

    auto widthNorm  = ( float )coords.width / m_currentAtlas->GetWidth();
    auto heightNorm = ( float )coords.height / m_currentAtlas->GetHeight();

    if( IsPlaceHolder( m_timePatern[ connComp ] ) )
        if( connComp < comps.size() )
        {
            if( comps[ connComp ]->GetNumVertices() == 4 )
            {
                auto channels = comps[ connComp ]->GetAttributeChannels();

                auto uvChannel = static_cast< Float2AttributeChannel* >( AttributeChannel::GetUVChannel( channels, 1 ) );

                auto& verts = uvChannel->GetVertices();

                verts[ 0 ] = glm::vec2( textureXNorm, textureYNorm + heightNorm );
                verts[ 1 ] = glm::vec2( textureXNorm  + widthNorm, textureYNorm + heightNorm );
                verts[ 2 ] = glm::vec2( textureXNorm ,textureYNorm );
                verts[ 3 ] = glm::vec2( textureXNorm + widthNorm, textureYNorm );
            }
        }
}

////////////////////////////
//
bool                                TimerPlugin::CheckTimeConsistency ( const std::wstring& time ) const
{
    if( m_timePatern.size() != time.size() )
        return false;

    for( unsigned int i = 0; i < m_timePatern.size(); ++i )
    {
        if( m_timePatern[ i ] == '#' )
        {
            if( time[ i ] >= '0' && time[ i ] <= '9' )
            {
                continue;
            }
        }
        else
        {
            if( m_timePatern[ i ] == time[ i ] )
                continue;
        }

        return false;
    }

    return true;
}

////////////////////////////
//
void                                TimerPlugin::SetTime        ( const std::wstring& time )
{
    if( !CheckTimeConsistency( time ) )
    {
        //std::cerr << time << L" doesn't match patern " << m_timePatern. << std::endl;
        return;
    }

    for( unsigned int i = 0; i < time.size(); ++i )
    {
        SetValue( i, time[ i ] );
    }
}

////////////////////////////
//
void                                TimerPlugin::SetTime        ( double time )
{
    TimeValue   newTime( time, m_timePaternInfo.fracOfSecondsPlaceholderSize );

    if( m_currentTime  != newTime )
    {
        m_currentTime = newTime;
        Refresh();
        m_vertexAttributeChannel->SetNeedsAttributesUpdate( true );
    }
}

////////////////////////////
//
const IVertexAttributesChannel *    TimerPlugin::GetVertexAttributesChannel          () const
{
    return m_vertexAttributeChannel.get();
}

////////////////////////////
//
TextureInfoVec                      TimerPlugin::GetTextures                 () const
{
    return m_textures;
}

////////////////////////////
//
void                                TimerPlugin::Update                      ( TimeType t )
{
    //FIXME: UPDATER TO FIX
    float time = m_timeParam.Evaluate();

    SetTime( time );
}

////////////////////////////
//
void                                TimerPlugin::Print                       ( std::ostream & out, int tabs ) const
{
}



} // model
} // bv