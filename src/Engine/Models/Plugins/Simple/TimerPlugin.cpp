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
    : BasePlugin( nullptr )
    , m_timeParam( timeParam )
    , m_fontResource()
    , m_currentAtlas()
{
    m_fontResource = TextHelper::LoadFont( "../dep/Media/fonts/digital-7.ttf", fontSize, L"../dep/Media/fonts/TimerChars.txt" );

    m_currentAtlas = TextHelper::GetAtlas( m_fontResource );

    auto textureResource = TextHelper::GetAtlasTextureInfo( m_currentAtlas );

    BuildDigitsMap();

    m_textures.push_back( new TextureInfo( textureResource, "AtlasTex" ) );

    m_vertexAttributeChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );

    TextHelper::BuildVACForText( m_vertexAttributeChannel.get(), m_currentAtlas, L"00:00:00" );
}

////////////////////////////
//
void TimerPlugin::BuildDigitsMap()
{
    auto fontExtraData = static_cast< const FontExtraData* >( m_fontResource->GetExtra() );

    std::wstring digits = L"0123456789";

    auto font = fontExtraData->GetFont();

    unsigned int d = 0;
    for( auto ch : digits )
    {
        auto glyph = font->GetGlyph( ch );

        m_digits[ d ] = glm::vec2( (float)glyph->textureX / font->GetAtlas()->GetWidth(), (float)glyph->textureY / font->GetAtlas()->GetHeight() );
        d++;
    }
}

////////////////////////////
//
TimerPlugin*                        TimerPlugin::Create     ( const ParamFloat& timeParam, unsigned int fontSize )
{
    return new TimerPlugin( timeParam, fontSize );
}

////////////////////////////
//
void                                TimerPlugin::SetTimePatern  ( const std::string& patern )
{
    m_timePatern = patern;
}

////////////////////////////
//
bool                                TimerPlugin::CheckTimeConsistency ( const std::string& time ) const
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
void                                TimerPlugin::SetTime        ( const std::string& time )
{
    if( !CheckTimeConsistency( time ) )
    {
        std::cerr << time << " doesn't match patern " << m_timePatern << std::endl;
        return;
    }

    assert(!"Implement");
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
    //m_currentTime = t;

    //auto sec = GetSecond( m_currentTime);

    //auto firstDigit = sec % 10;
    //auto sekondDigit = sec / 10;

    //for( unsigned int i = 0; i < m_vertexAttributeChannel->GetComponents().size(); ++i )
    //{
    //    auto connComp = static_cast< const model::ConnectedComponent* >( m_vertexAttributeChannel->GetComponents()[ i ] );
    //    auto compChannels = connComp->GetAttributeChannels();

    //    if( auto posChannel = AttributeChannel::GetPositionChannel( compChannels ) )
    //        if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, 1 ) )
    //        {
    //            auto & verts  = dynamic_cast< Float3AttributeChannel* >(posChannel)->GetVertices();
    //            auto & uvs    = dynamic_cast< Float2AttributeChannel* >(uvChannel)->GetVertices();

    //            for( unsigned int i = 0; i < verts.size(); ++i )
    //            {
    //                uvs[ i ].x = m_digits[ firstDigit ].x;
    //                uvs[ i ].y = m_digits[ firstDigit ].y;
    //            }
    //        }
    //}

    m_vertexAttributeChannel->SetNeedsAttributesUpdate( true );

    m_vertexAttributeChannel->Update( t );
}

////////////////////////////
//
unsigned int                TimerPlugin::GetSecond( float t )
{
    return unsigned int( t ) % 60;
}

////////////////////////////
//
unsigned int                TimerPlugin::GetHOAS( float t )
{
    return unsigned int( t * 100.f ) % 100;
}

////////////////////////////
//
unsigned int                TimerPlugin::GetMinute( float t )
{
    return unsigned int( t / 60.f ) % 60;
}

////////////////////////////
//
unsigned int                TimerPlugin::GetHour( float t )
{
    return unsigned int( t / 3600.f );
}

////////////////////////////
//
void                                TimerPlugin::Print                       ( std::ostream & out, int tabs ) const
{
}



} // model
} // bv