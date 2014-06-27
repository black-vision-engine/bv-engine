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
ResourceHandleConstPtr      TextHelper::LoadFont( const std::string& fontFileName, size_t size, size_t blurSize, const std::wstring& atlasCharSetFile )
{
    auto fRes = FontResource( fontFileName, size, blurSize, atlasCharSetFile );

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
    auto altasHandle = ResourceHandleConstPtr( new ResourceHandle( const_cast< char* >(textAtlas->GetData()), texSize, atlasExtraData ) );

    return altasHandle;
}

#define viewWidth   (1080 / 2)
#define viewHeight  (1080 / 2)

// *********************************
//
void                    TextHelper::BuildVACForText     ( VertexAttributesChannel* vertexAttributeChannel, const TextAtlas * textAtlas, const std::wstring& text, unsigned int blurSize, float spacing, TextAlignmentType tat, const std::wstring& textPatern )
{
    assert( vertexAttributeChannel );
    assert( textAtlas );

    glm::vec3 translate(0.f);
    glm::vec3 interspace( spacing, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

    float blurTexSize = float( blurSize );
    float blurLenghtX = float( blurSize ) / viewWidth;
    float blurLenghtY = float( blurSize ) / viewHeight;

    auto spaceGlyphWidth    = 0.3*(float)textAtlas->GetGlyphHeight( L'0' ) * 0.5/viewWidth;
    auto newLineShift       = -(float)textAtlas->GetGlyphWidth( L'0' ) / viewHeight;

    bool usePatern = false;

    if( !textPatern.empty() )
    {
        assert( textPatern.size() == text.size() );
        usePatern = true;
    }

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
            glm::vec3 baring = glm::vec3( 0.f, (glyphCoord->glyphHeight - glyphCoord->bearingY) / (float)viewHeight, 0.f );

            glm::vec3 quadBottomLeft;
            glm::vec3 quadBottomRight;
            glm::vec3 quadTopLeft;
            glm::vec3 quadTopRight;

            if( usePatern && textPatern[i] == L'#' )
            {
                quadBottomLeft     = glm::vec3( 0.f, 0.f, 0.f ) + glm::vec3( -blurLenghtX, -blurLenghtY, 0.f );
                quadBottomRight    = glm::vec3( (float)glyphCoord->width / (float)viewWidth, 0.f, 0.f ) +  glm::vec3( blurLenghtX, -blurLenghtY, 0.f );
                quadTopLeft        = glm::vec3( 0.f, (float)glyphCoord->height / (float)viewHeight, 0.f ) + glm::vec3( -blurLenghtX, blurLenghtY, 0.f );
                quadTopRight       = glm::vec3( (float)glyphCoord->width / (float)viewWidth, (float)glyphCoord->height / (float)viewHeight, 0.f ) + glm::vec3( blurLenghtX, blurLenghtY, 0.f );
            }
            else
            {
                quadBottomLeft     = glm::vec3( 0.f, 0.f, 0.f ) + glm::vec3( -blurLenghtX, -blurLenghtY, 0.f );
                quadBottomRight    = glm::vec3( (float)glyphCoord->glyphWidth / (float)viewWidth, 0.f, 0.f ) +  glm::vec3( blurLenghtX, -blurLenghtY, 0.f );
                quadTopLeft        = glm::vec3( 0.f, (float)glyphCoord->glyphHeight / (float)viewHeight, 0.f ) + glm::vec3( -blurLenghtX, blurLenghtY, 0.f );
                quadTopRight       = glm::vec3( (float)glyphCoord->glyphWidth / (float)viewWidth, (float)glyphCoord->glyphHeight / (float)viewHeight, 0.f ) + glm::vec3( blurLenghtX, blurLenghtY, 0.f );
            }

            posAttribChannel->AddAttribute( quadBottomLeft    + translate - baring + newLineTranslation );
            posAttribChannel->AddAttribute( quadBottomRight   + translate - baring + newLineTranslation );
            posAttribChannel->AddAttribute( quadTopLeft       + translate - baring + newLineTranslation );
            posAttribChannel->AddAttribute( quadTopRight      + translate - baring + newLineTranslation );

            connComp->AddAttributeChannel( AttributeChannelPtr( posAttribChannel ) );

            AttributeChannelDescriptor * desc1 = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            auto verTex0AttrChannel = new model::Float2AttributeChannel( desc1, "textAtlasPosition", true );

            float texLeft;
            float texTop;
            float texWidth;
            float texHeight;

            if( usePatern && textPatern[i] == L'#' )
            {
                texLeft   = ((float)glyphCoord->textureX)  / textAtlas->GetWidth();
                texTop    = ((float)glyphCoord->textureY)  / textAtlas->GetHeight();
                texWidth  = ((float)glyphCoord->width)     / textAtlas->GetWidth();
                texHeight = ((float)glyphCoord->height)    / textAtlas->GetHeight();
            }
            else
            {
                texLeft   = ((float)glyphCoord->textureX + (float)glyphCoord->glyphX - blurTexSize)  / textAtlas->GetWidth();
                texTop    = ((float)glyphCoord->textureY + (float)glyphCoord->glyphY - blurTexSize)  / textAtlas->GetHeight();
                texWidth  = ((float)glyphCoord->glyphWidth + 2 * blurTexSize)     / textAtlas->GetWidth();
                texHeight = ((float)glyphCoord->glyphHeight + 2 * blurTexSize)    / textAtlas->GetHeight();
            }


            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop + texHeight ) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop + texHeight ) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop ) );

            connComp->AddAttributeChannel( AttributeChannelPtr( verTex0AttrChannel ) );

            vertexAttributeChannel->AddConnectedComponent( connComp );

            if( usePatern && textPatern[i] == L'#' )
            {
                translate += glm::vec3( glyphCoord->width / (float)viewWidth, 0.f, 0.f ) + interspace;
            }
            else
            {
                translate += glm::vec3( glyphCoord->glyphWidth / (float)viewWidth, 0.f, 0.f ) + interspace;
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
}


} // model
} // bv