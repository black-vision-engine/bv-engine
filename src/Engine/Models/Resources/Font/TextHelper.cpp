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
ResourceHandleConstPtr      TextHelper::LoadFont( const std::string& fontFileName, size_t size, size_t blurSize, size_t outlineSize, const std::wstring& atlasCharSetFile )
{
    auto fRes = FontResource( fontFileName, size, blurSize, outlineSize, atlasCharSetFile );

    FontLoader fLoader;

    auto ret = ResourceHandleConstPtr( fLoader.LoadResource( &fRes ) );

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

ConnectedComponentPtr         CreateEmptyCC()
{
    ConnectedComponentPtr connComp = ConnectedComponent::Create();

    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto posAttribChannel = new Float3AttributeChannel( desc, "vertexPosition", true );

    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );

    connComp->AddAttributeChannel( AttributeChannelPtr( posAttribChannel ) );

    AttributeChannelDescriptor * desc1 = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    auto verTex0AttrChannel = new model::Float2AttributeChannel( desc1, "textAtlasPosition", true );

    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );

    connComp->AddAttributeChannel( AttributeChannelPtr( verTex0AttrChannel ) );

    return connComp;
}

} // anonymous


const TextAtlas *           TextHelper::GetAtlas            ( const ResourceHandle * fontResource, bool bolded , bool italic )
{
    auto f = GetFont( fontResource, bolded, italic );

    const TextAtlas* textAtlas = nullptr;

    if( f )
        return f->GetAtlas();
    else
        return nullptr;
}

///////////////////////////////
//
ResourceHandleConstPtr      TextHelper::GetAtlasTextureInfo ( const TextAtlas * textAtlas )
{
    assert( textAtlas );

    unsigned int texSize = textAtlas->GetWidth() * textAtlas->GetHeight() * 4; //FIXME: Add format to atlas

    TextureExtraData* atlasExtraData = new TextureExtraData( textAtlas->GetWidth(), textAtlas->GetHeight(), 32, TextureFormat::F_A8R8G8B8, TextureType::T_2D );
    auto altasHandle = ResourceHandleConstPtr( new ResourceHandle( textAtlas->GetData(), texSize, atlasExtraData ) );

    return altasHandle;
}

#define viewWidth   (1080 / 2)
#define viewHeight  (1080 / 2)

// *********************************
//
float                    TextHelper::BuildVACForText     ( VertexAttributesChannel* vertexAttributeChannel, const TextAtlas * textAtlas, const std::wstring& text, unsigned int blurSize, float spacing, TextAlignmentType tat, bool useKerning )
{
    assert( vertexAttributeChannel );
    assert( textAtlas );

    glm::vec3 translate(0.f);
    glm::vec3 interspace( spacing, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

    float blurTexSize = float( blurSize );
    float blurLenghtX = float( blurSize ) / viewWidth;
    float blurLenghtY = float( blurSize ) / viewHeight;

    float ccPaddingX = 1.f / viewWidth;
    float ccPaddingY = 1.f / viewHeight;

    float texPadding = 1.f;

    auto spaceGlyphWidth    = (float)textAtlas->GetGlyphCoords( L'0' )->glyphWidth / viewWidth  + spacing;
    auto newLineShift       = -(float)textAtlas->GetGlyphHeight( L'0' ) / viewHeight;

    for( unsigned int i = 0; i < text.size(); ++i )
    {
        auto wch = text[i];

        if( wch == L' ' )
        {
            translate += glm::vec3( spaceGlyphWidth, 0.f, 0.f )+ interspace;
            continue;
        }

        if( wch == L'\n' || wch == L'\r' )
        {
            translate = glm::vec3( 0.f );
            newLineTranslation += glm::vec3( 0.f, newLineShift , 0.f );
            continue;
        }

        auto glyphH = textAtlas->GetGlyphHeight( wch );
        auto glyphW = textAtlas->GetGlyphWidth( wch );

        ConnectedComponentPtr connComp = ConnectedComponent::Create();

        AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        auto posAttribChannel = new Float3AttributeChannel( desc, "vertexPosition", true );

        if( auto glyphCoord = textAtlas->GetGlyphCoords( wch ) )
        {
            glm::vec3 bearing = glm::vec3( (float)glyphCoord->bearingX / (float)viewWidth, (float)( glyphCoord->bearingY - (int)glyphCoord->glyphHeight ) / (float)viewHeight, 0.f );

            glm::vec3 quadBottomLeft;
            glm::vec3 quadBottomRight;
            glm::vec3 quadTopLeft;
            glm::vec3 quadTopRight;

            auto kerningShift = glm::vec3( 0.f, 0.f, 0.f );

            if( useKerning && i > 0 )
            {
                auto kerShift = textAtlas->GetKerning( text[ i - 1 ], text[ i ] );
                kerningShift.x = kerShift / (float)viewWidth;
                translate += kerningShift;
            }

            {
                quadBottomLeft     = glm::vec3( 0.f, 0.f, 0.f ) + glm::vec3( -blurLenghtX, -blurLenghtY, 0.f ) + glm::vec3( -ccPaddingX, -ccPaddingY, 0.f );
                quadBottomRight    = glm::vec3( (float)glyphCoord->glyphWidth / (float)viewWidth, 0.f, 0.f ) +  glm::vec3( blurLenghtX, -blurLenghtY, 0.f ) + glm::vec3( ccPaddingX, -ccPaddingY, 0.f );
                quadTopLeft        = glm::vec3( 0.f, (float)glyphCoord->glyphHeight / (float)viewHeight, 0.f ) + glm::vec3( -blurLenghtX, blurLenghtY, 0.f ) + glm::vec3( -ccPaddingX, ccPaddingY, 0.f );
                quadTopRight       = glm::vec3( (float)glyphCoord->glyphWidth / (float)viewWidth, (float)glyphCoord->glyphHeight / (float)viewHeight, 0.f ) + glm::vec3( blurLenghtX, blurLenghtY, 0.f ) + glm::vec3( ccPaddingX, ccPaddingY, 0.f );
            }

            posAttribChannel->AddAttribute( quadBottomLeft    + translate + bearing + newLineTranslation );
            posAttribChannel->AddAttribute( quadBottomRight   + translate + bearing + newLineTranslation );
            posAttribChannel->AddAttribute( quadTopLeft       + translate + bearing + newLineTranslation );
            posAttribChannel->AddAttribute( quadTopRight      + translate + bearing + newLineTranslation );

            connComp->AddAttributeChannel( AttributeChannelPtr( posAttribChannel ) );

            AttributeChannelDescriptor * desc1 = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            auto verTex0AttrChannel = new model::Float2AttributeChannel( desc1, "textAtlasPosition", true );

            float texLeft;
            float texTop;
            float texWidth;
            float texHeight;

            {
                texLeft   = ( (float)glyphCoord->textureX + (float)glyphCoord->glyphX - blurTexSize - texPadding )  / textAtlas->GetWidth();
                texTop    = ( (float)glyphCoord->textureY + (float)glyphCoord->glyphY - blurTexSize - texPadding )  / textAtlas->GetHeight();
                texWidth  = ( (float)glyphCoord->glyphWidth + 2 * blurTexSize + 2 * texPadding )     / textAtlas->GetWidth();
                texHeight = ( (float)glyphCoord->glyphHeight + 2 * blurTexSize  + 2 * texPadding )    / textAtlas->GetHeight();
            }


            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop + texHeight ) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop + texHeight ) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop ) );

            connComp->AddAttributeChannel( AttributeChannelPtr( verTex0AttrChannel ) );

            vertexAttributeChannel->AddConnectedComponent( connComp );

            {
                translate += glm::vec3( glyphCoord->advanceX / (float)viewWidth, 0.f, 0.f ) + interspace;
            }
        }
        else
        {
            assert( !( "Cannot find glyph for char " + wch) );
        }
    }

    auto alignmentTranslation = 0.f;
    switch (tat)
    {
        case TextAlignmentType::Center:
            alignmentTranslation = -translate.x / 2.f;
            break;
        case TextAlignmentType::Right:
            alignmentTranslation = -translate.x;
            break;

    }
    

    if( tat != TextAlignmentType::Left )
    {
        for( auto cc : vertexAttributeChannel->GetComponents() )
        {
            auto& verts = std::static_pointer_cast< Float3AttributeChannel >( cc->GetAttributeChannels()[ 0 ] )->GetVertices();

            for ( auto& v : verts )
            {
                v.x += alignmentTranslation;
            }
        }

    }


    if( vertexAttributeChannel->GetComponents().empty() ) // FIXME: We add one empty CC because of bug #72174842
    {
        vertexAttributeChannel->AddConnectedComponent( CreateEmptyCC() );
    }

    return translate.x; // FIXME: This does not work for multiline text
}


} // model
} // bv
