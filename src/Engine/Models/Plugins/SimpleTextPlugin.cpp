#include "SimpleTextPlugin.h"

#include "Engine\Models\Resources\IResource.h"
#include "Engine\Models\Resources\TextureLoader.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"

namespace bv { namespace model {

const std::string SimpleTextPluginPD::pluginName = "SimpleTextPlugin";

SimpleTextPlugin* SimpleTextPlugin::Create( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize )
{
    return new SimpleTextPlugin( text, fontFileName, fontSize );
}

SimpleTextPlugin::SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize )
    : m_text( new Text( text, fontFileName, int( fontSize * (1.25f) ) /* points to pixel proportion */) ) // FIXME:
{
    m_textures.push_back( LoadAtlas( "AtlasTex" ) );

    EvalGeometryChannel();
}

SimpleTextPlugin::~SimpleTextPlugin   ()
{}

TextureInfo* SimpleTextPlugin::LoadAtlas( const std::string& name )   const
{
    auto atlas = m_text->GetAtlas();
    unsigned int texSize = atlas->GetWidth() * atlas->GetHeight() * 4; //FIXME: Add format to atlas

    TextureExtraData* atlasExtraData = new TextureExtraData( atlas->GetWidth(), atlas->GetHeight(), 32, TextureFormat::F_A8R8G8B8, TextureType::T_2D );
    ResourceHandle* altasHandle = new ResourceHandle( const_cast< char* >(atlas->GetData()), texSize, atlasExtraData );

    return new TextureInfo( altasHandle, name );
}

#define viewWidth   100
#define viewHeight  100

void                SimpleTextPlugin::EvalGeometryChannel( )
{
    auto texExtraData = static_cast< const TextureExtraData* > ( m_textures[ 0 ]->m_resHandle->GetExtra() );

    GeometryChannelDescriptor geomChannelDesc;

    geomChannelDesc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    geomChannelDesc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    m_geomChannel = new GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, geomChannelDesc, true, true );

    glm::vec3 translate(0.f);
    glm::vec3 interspace( 0.07f, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

    auto glyphH = m_text->GetAtlas()->GetGlyphHeight();
    auto glyphW = m_text->GetAtlas()->GetGlyphWidth();

    for( auto wch : m_text->GetText() )
    {
        if( wch == L' ' )
        {
            translate += glm::vec3( 0.3*(float)glyphW * 0.5/viewWidth, 0.f, 0.f );
            continue;
        }

        if( wch == L'\n' )
        {
            translate = glm::vec3( 0.f );
            newLineTranslation += glm::vec3( 0.f, (float)glyphH /viewHeight, 0.f );
            continue;
        }

        ConnectedComponent* connComp = new ConnectedComponent();

        VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        auto posAttribChannel = new Float3VertexAttributeChannel( desc, "vertexPosition", true );

        auto glyph = m_text->GetGlyph( wch );

        glm::vec3 baring = glm::vec3( 0.f, (glyph->height - glyph->bearingY) / (float)viewHeight, 0.f );

        auto quadBottomLeft     = glm::vec3( 0.f, 0.f, 0.f );
        auto quadBottomRight    = glm::vec3( (float)glyph->width / (float)viewWidth, 0.f, 0.f );
        auto quadTopLeft        = glm::vec3( 0.f, (float)glyph->height / (float)viewHeight, 0.f );
        auto quadTopRight       = glm::vec3( (float)glyph->width / (float)viewWidth, (float)glyph->height / (float)viewHeight, 0.f );

        posAttribChannel->AddVertexAttribute( quadBottomLeft    + translate - baring + newLineTranslation );
        posAttribChannel->AddVertexAttribute( quadBottomRight   + translate - baring + newLineTranslation );
        posAttribChannel->AddVertexAttribute( quadTopLeft       + translate - baring + newLineTranslation );
        posAttribChannel->AddVertexAttribute( quadTopRight      + translate - baring + newLineTranslation );

        connComp->m_vertexAttributeChannels.push_back( posAttribChannel );

        VertexAttributeChannelDescriptor * desc1 = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

        auto verTex0AttrChannel = new model::Float2VertexAttributeChannel( desc1, m_textures[ 0 ]->m_texName, true );

        float texLeft   = ((float)glyph->textureX) / texExtraData->GetWidth();
        float texTop    = ((float)glyph->textureY) / texExtraData->GetHeight();
        float texWidth  = ((float)glyph->width) / texExtraData->GetWidth();
        float texHeight = ((float)glyph->height) / texExtraData->GetHeight();


        verTex0AttrChannel->AddVertexAttribute( glm::vec2( texLeft, texTop + texHeight ) );
        verTex0AttrChannel->AddVertexAttribute( glm::vec2( texLeft + texWidth, texTop + texHeight ) );
        verTex0AttrChannel->AddVertexAttribute( glm::vec2( texLeft, texTop) );
        verTex0AttrChannel->AddVertexAttribute( glm::vec2( texLeft + texWidth, texTop ) );

        connComp->m_vertexAttributeChannels.push_back( verTex0AttrChannel );

        m_geomChannel->AddConnectedComponent( connComp );

        translate += glm::vec3( glyph->width / (float)viewWidth, 0.f, 0.f ) + interspace;
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