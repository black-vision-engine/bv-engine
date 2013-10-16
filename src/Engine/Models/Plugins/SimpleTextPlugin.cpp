#include "SimpleTextPlugin.h"

#include "Engine\Models\Resources\IResource.h"
#include "Engine\Models\Resources\TextureLoader.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"

namespace bv { namespace model {

const std::string SimpleTextPluginPD::pluginName = "SimpleTextPlugin";

SimpleTextPlugin::SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName )
    : m_text( new Text( text, fontFileName ) ) // FIXME:
{
    m_textures.push_back( LoadAtlas( "Tex0" ) );

    EvalGeometryChannel();
}

SimpleTextPlugin::~SimpleTextPlugin   ()
{}

SimpleTextPlugin::TexturePair SimpleTextPlugin::LoadAtlas( const std::string& name )   const
{
    auto atlas = m_text->GetAtlas();
    unsigned int texSize = atlas->GetWidth() * atlas->GetHeight() * 4; //FIXME: Add format to atlas

    TextureExtraData* atlasExtraData = new TextureExtraData( atlas->GetWidth(), atlas->GetHeight(), 32, TextureFormat::F_A8R8G8B8, TextureType::T_2D );
    ResourceHandle* altasHandle = new ResourceHandle( const_cast< char* >(atlas->GetData()), texSize, atlasExtraData );

    return SimpleTextPlugin::TexturePair( altasHandle, name );
}

void                SimpleTextPlugin::EvalGeometryChannel( )
{
    auto texExtraData = static_cast< const TextureExtraData* > ( m_textures[ 0 ].first->GetExtra() );

    GeometryChannelDescriptor geomChannelDesc;

    geomChannelDesc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    geomChannelDesc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    m_geomChannel = new GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, geomChannelDesc, true, true );

    glm::vec3 translate(0.f);
    glm::vec3 interspace( 0.07f, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

    for( auto wch : m_text->GetText() )
    {
        if( wch == L' ' )
        {
            translate += glm::vec3( 0.3, 0.f, 0.f );
            continue;
        }

        if( wch == L'\n' )
        {
            translate = glm::vec3( 0.f );
            newLineTranslation += glm::vec3( 0.f, -1.1f, 0.f );
            continue;
        }

        ConnectedComponent* connComp = new ConnectedComponent();

        VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        auto posAttribChannel = new Float3VertexAttributeChannel( desc, "vertexPosition", true );

        auto glyph = m_text->GetGlyph( wch );

        glm::vec3 baring = glm::vec3( glyph->bearingX / (float)glyph->size, glyph->bearingY / (float)glyph->size, 0.f );

        posAttribChannel->AddVertexAttribute( glm::vec3( -0.5f, -0.5f, 0.f ) + translate + baring + newLineTranslation );
        posAttribChannel->AddVertexAttribute( glm::vec3( 0.5f, -0.5f, 0.f ) + translate + baring + newLineTranslation );
        posAttribChannel->AddVertexAttribute( glm::vec3( -0.5f, 0.5f, 0.f ) + translate + baring + newLineTranslation );
        posAttribChannel->AddVertexAttribute( glm::vec3( 0.5f, 0.5f, 0.f ) + translate + baring + newLineTranslation );

        connComp->m_vertexAttributeChannels.push_back( posAttribChannel );

        VertexAttributeChannelDescriptor * desc1 = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

        auto verTex0AttrChannel = new model::Float2VertexAttributeChannel( desc1, m_textures[ 0 ].second, true );

        
        float left = ((float)glyph->textureX ) / texExtraData->GetWidth();
        float top = ((float)glyph->textureY ) / texExtraData->GetHeight();

        float width = ((float)glyph->size - 2) / texExtraData->GetWidth();
        float height = ((float)glyph->size - 2) / texExtraData->GetHeight();

        verTex0AttrChannel->AddVertexAttribute( glm::vec2( left, top + height ) );
        verTex0AttrChannel->AddVertexAttribute( glm::vec2( left + width, top + height ) );
        verTex0AttrChannel->AddVertexAttribute( glm::vec2( left, top) );
        verTex0AttrChannel->AddVertexAttribute( glm::vec2( left + width, top ) );

        connComp->m_vertexAttributeChannels.push_back( verTex0AttrChannel );

        m_geomChannel->AddConnectedComponent( connComp );

        translate += glm::vec3( glyph->width / (float)glyph->size, 0.f, 0.f ) + interspace;
    } 
}

void                SimpleTextPlugin::Update                      ( float t )
{
    BasePlugin::Update( t );
}

void                SimpleTextPlugin::Print                       ( std::ostream & out, int tabs ) const
{
}


} // model
} // bv