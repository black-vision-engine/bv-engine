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
TimerPlugin::TimerPlugin( const ParamFloat& timeParam, unsigned int fontSize )
    : BasePlugin( "dupa", "dupa", nullptr, nullptr )
    , m_timeParam( timeParam )
    , m_fontResource()
    , m_currentAtlas()
    , m_timePatern( L"##:##:##" )
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

    int setSec = int(time * 100) % 100;
    int sec = int(time) % 60;
    int min = int(time / 60) % 60;

    std::wstring newTime =      ( std::to_wstring( min ).size() == 2 ? std::to_wstring( min ) : ( L"0" + std::to_wstring( min ) ) )
                            +   L":"
                            +   ( std::to_wstring( sec ).size() == 2 ? std::to_wstring( sec ) : ( L"0" + std::to_wstring( sec ) ) )
                            +   L":"
                            +   ( std::to_wstring( setSec ).size() == 2 ? std::to_wstring( setSec ) : ( L"0" + std::to_wstring( setSec ) ) );

    if( newTime != m_currentTime )
    {
        SetTime( newTime );
        m_vertexAttributeChannel->SetNeedsAttributesUpdate( true );
    }
}

////////////////////////////
//
void                                TimerPlugin::Print                       ( std::ostream & out, int tabs ) const
{
}



} // model
} // bv