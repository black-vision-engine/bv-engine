#include "TextHelper.h"

#include "Engine/Models/Resources/Font/FontLoader.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Resources/Font/Text.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include <assert.h>

namespace bv { namespace model {

///////////////////////////////
//
const ResourceHandle *      TextHelper::LoadFont( const std::string& fontFileName, size_t size, const std::wstring& atlasCharSetFile )
{
    auto fRes = FontResource( fontFileName, size, atlasCharSetFile );

    FontLoader fLoader;

    auto ret = fLoader.LoadResource( &fRes );

    assert( ret->GetExtra() );
    assert( ret->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_FONT );

    return ret;
}

// *********************************
//
VertexAttributesChannel*    TextHelper::CreateEmptyVACForText()
{
    VertexAttributesChannelDescriptor vacDesc;

    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    return new VertexAttributesChannel( PrimitiveType::PT_TRIANGLE_STRIP, vacDesc);
}

namespace
{
///////////////////////////////
// Helper function for getting proper atlas from font resource. Plain, italic, bold or italic-bold.
const Text*             GetFont( const ResourceHandle * fontResource, bool bolded, bool italic )
{
    assert( fontResource->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_FONT );
    auto fontExtraData = static_cast< const FontExtraData* >( fontResource->GetExtra() );

    if( !bolded && !italic )
    { 
        assert( fontExtraData->GetFont() );
        return fontExtraData->GetFont();
    }

    if( bolded && italic )
    {
        assert( fontExtraData->GetFontBoldItalic() );
        return fontExtraData->GetFontBoldItalic();
    }

    if( italic )
    {
        assert( fontExtraData->GetFontItalic() );
        return fontExtraData->GetFontItalic();
    }

    if( bolded )
    {
        assert( fontExtraData->GetFontBold() );
        return fontExtraData->GetFontBold();
    }

    assert( !"Should never be here!" );

    return nullptr;
}

} // anonymous


///////////////////////////////
//
const ResourceHandle *      TextHelper::GetAtlasTextureInfo ( const ResourceHandle * fontResource, bool bolded, bool italic )
{
    auto f = GetFont( fontResource, bolded, italic );

    const TextAtlas* textAtlas = nullptr;

    if( f )
        textAtlas = f->GetAtlas();

    if( !textAtlas )
    {
        assert(!"Cannot load atlas");
    }

    unsigned int texSize = textAtlas->GetWidth() * textAtlas->GetHeight() * 4; //FIXME: Add format to atlas

    TextureExtraData* atlasExtraData = new TextureExtraData( textAtlas->GetWidth(), textAtlas->GetHeight(), 32, TextureFormat::F_A8R8G8B8, TextureType::T_2D );
    ResourceHandle* altasHandle = new ResourceHandle( const_cast< char* >(textAtlas->GetData()), texSize, atlasExtraData );

    return altasHandle;
}

#define viewWidth   100
#define viewHeight  100

// *********************************
//
void                    TextHelper::BuildVACForText     ( VertexAttributesChannel* vertexAttributeChannel, const ResourceHandle * fontResource, const std::wstring& text, bool bolded, bool italic )
{
    assert( vertexAttributeChannel );
    assert( fontResource );

    auto fontExtraData = static_cast< const FontExtraData* > ( fontResource->GetExtra() );

    auto f = GetFont( fontResource, bolded, italic );

    auto atlas = f->GetAtlas();

    glm::vec3 translate(0.f);
    glm::vec3 interspace( 0.07f, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

    auto glyphH = atlas->GetGlyphHeight();
    auto glyphW = atlas->GetGlyphWidth();

    for( auto wch : text )
    {
        if( wch == L' ' )
        {
            translate += glm::vec3( 0.3*(float)glyphW * 0.5/viewWidth, 0.f, 0.f )+ interspace;
            continue;
        }

        if( wch == L'\n' || wch == L'\r' )
        {
            translate = glm::vec3( 0.f );
            newLineTranslation += glm::vec3( 0.f, -(float)glyphH /viewHeight, 0.f );
            continue;
        }

        ConnectedComponent* connComp = new ConnectedComponent();

        AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        auto posAttribChannel = new Float3AttributeChannel( desc, "vertexPosition", true );

        auto glyph = f->GetGlyph( wch );

        glm::vec3 baring = glm::vec3( 0.f, (glyph->height - glyph->bearingY) / (float)viewHeight, 0.f );

        auto quadBottomLeft     = glm::vec3( 0.f, 0.f, 0.f );
        auto quadBottomRight    = glm::vec3( (float)glyph->width / (float)viewWidth, 0.f, 0.f );
        auto quadTopLeft        = glm::vec3( 0.f, (float)glyph->height / (float)viewHeight, 0.f );
        auto quadTopRight       = glm::vec3( (float)glyph->width / (float)viewWidth, (float)glyph->height / (float)viewHeight, 0.f );

        posAttribChannel->AddAttribute( quadBottomLeft    + translate - baring + newLineTranslation );
        posAttribChannel->AddAttribute( quadBottomRight   + translate - baring + newLineTranslation );
        posAttribChannel->AddAttribute( quadTopLeft       + translate - baring + newLineTranslation );
        posAttribChannel->AddAttribute( quadTopRight      + translate - baring + newLineTranslation );

        connComp->AddAttributeChannel( AttributeChannelPtr( posAttribChannel ) );

        AttributeChannelDescriptor * desc1 = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

        auto verTex0AttrChannel = new model::Float2AttributeChannel( desc1, "textAtlasPosition", true );

        float texLeft   = ((float)glyph->textureX) / atlas->GetWidth();
        float texTop    = ((float)glyph->textureY) / atlas->GetHeight();
        float texWidth  = ((float)glyph->width) / atlas->GetWidth();
        float texHeight = ((float)glyph->height) / atlas->GetHeight();


        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop + texHeight ) );
        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop + texHeight ) );
        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop) );
        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop ) );

        connComp->AddAttributeChannel( AttributeChannelPtr( verTex0AttrChannel ) );

        vertexAttributeChannel->AddConnectedComponent( connComp );

        translate += glm::vec3( glyph->width / (float)viewWidth, 0.f, 0.f ) + interspace;
    } 
}


} // model
} // bv