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
bool    TimeValue::operator!=(const TimeValue& other) const
{
    return  other.setSecond == this->setSecond
        ||  other.second == this->second
        ||  other.minute == this->minute
        ||  other.hour == this->hour;
}

////////////////////////////
//
TimeValue::TimeValue( double time )
{
    this->setSecond = int( time * 100 ) % 100;
    this->second    = int( time ) % 60;
    this->minute    = int( time / 60 ) % 60;
    this->hour      = int( time / (60 * 60 ) ) % 60;
}

////////////////////////////
//
TimerPlugin::TimerPlugin( const ParamFloat& timeParam, unsigned int fontSize )
    : BasePlugin( nullptr )
    , m_timeParam( timeParam )
    , m_fontResource()
    , m_currentAtlas()
    , m_timePatern( L"##:##:##" )
    , m_currentTime( 0.0 )
{
    m_fontResource = TextHelper::LoadFont( "../dep/Media/fonts/digital-7.ttf", fontSize, L"../dep/Media/fonts/TimerChars.txt" );

    m_currentAtlas = TextHelper::GetAtlas( m_fontResource );

    auto textureResource = TextHelper::GetAtlasTextureInfo( m_currentAtlas );

    m_textures.push_back( new TextureInfo( textureResource, "AtlasTex" ) );

    m_vertexAttributeChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );

    TextHelper::BuildVACForText( m_vertexAttributeChannel.get(), m_currentAtlas, L"00:00:00", m_timePatern );
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
    return m_currentAtlas->GetGlyphCoords( wch );
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

    if( m_timePatern[ connComp ] == L'#' )
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
    TimeValue   newTime( time );

    if( m_currentTime  != newTime )
    {
        std::wstring newTimeString =        ( std::to_wstring( newTime.minute ).size() == 2 ? std::to_wstring( newTime.minute ) : ( L"0" + std::to_wstring( newTime.minute ) ) )
                                        +   L":"
                                        +   ( std::to_wstring( newTime.second ).size() == 2 ? std::to_wstring( newTime.second ) : ( L"0" + std::to_wstring( newTime.second ) ) )
                                        +   L":"
                                        +   ( std::to_wstring( newTime.setSecond ).size() == 2 ? std::to_wstring( newTime.setSecond ) : ( L"0" + std::to_wstring( newTime.setSecond ) ) );
        SetTime( newTimeString );
        m_currentTime = newTime;
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
Textures                            TimerPlugin::GetTextures                 () const
{
    return m_textures;
}

////////////////////////////
//
void                                TimerPlugin::Update                      ( TimeType t )
{
    float time = m_timeParam.Evaluate( t );

    SetTime( time );
}

////////////////////////////
//
void                                TimerPlugin::Print                       ( std::ostream & out, int tabs ) const
{
}



} // model
} // bv