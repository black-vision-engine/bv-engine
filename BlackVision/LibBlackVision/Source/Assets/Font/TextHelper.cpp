#include "TextHelper.h"
#include "TextAtlas.h"

#include "Assets/Assets.h"
#include "Assets/Font/Glyph.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"



#include <assert.h>

namespace bv {

///////////////////////////////
//
// @todo Raplace all references with LoadTypedAsset
FontAssetConstPtr      TextHelper::LoadFont( const FontAssetDescConstPtr & fontAssetDesc )
{
	return std::static_pointer_cast< const FontAsset >( AssetManager::GetInstance().LoadAsset( fontAssetDesc ) );
}

// *********************************
//
model::VertexAttributesChannelPtr   TextHelper::CreateEmptyVACForText()
{
    model::VertexAttributesChannelDescriptor vacDesc;

    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

	return std::make_shared< model::VertexAttributesChannel>( PrimitiveType::PT_TRIANGLE_STRIP, vacDesc );
}

namespace
{
///////////////////////////////
// Helper function for getting proper atlas from font asset.
TextConstPtr				GetFont( const AssetConstPtr & asset )
{
	auto fontRes = QueryTypedRes< FontAssetConstPtr >( asset );
    assert( fontRes != nullptr );

	return fontRes->GetText();
}

model::ConnectedComponentPtr         CreateEmptyCC()
{
    model::ConnectedComponentPtr connComp = model::ConnectedComponent::Create();

    model::AttributeChannelDescriptor * desc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto posAttribChannel = new model::Float3AttributeChannel( desc, "vertexPosition", true );

    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );

    connComp->AddAttributeChannel( model::AttributeChannelPtr( posAttribChannel ) );

    model::AttributeChannelDescriptor * desc1 = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    auto verTex0AttrChannel = new model::Float2AttributeChannel( desc1, "textAtlasPosition", true );

    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );

    connComp->AddAttributeChannel( model::AttributeChannelPtr( verTex0AttrChannel ) );

    return connComp;
}

} // anonymous


TextAtlasConstPtr				TextHelper::GetAtlas            ( const AssetConstPtr & asset )
{
    auto f = GetFont( asset );


    if( f )
    {
		return f->GetAtlas();
	}
	else
    {
        return nullptr;
    }
}

#define viewWidth   (1080 / 2)
#define viewHeight  (1080 / 2)

// *********************************
//
float                    TextHelper::BuildVACForText     ( model::VertexAttributesChannel * vertexAttributeChannel, const TextAtlasConstPtr & textAtlas, const std::wstring & text, unsigned int blurSize, float spacing, TextAlignmentType tat, SizeType outlineSize, bool useKerning )
{
    assert( vertexAttributeChannel );
    assert( textAtlas );

    glm::vec3 translate(0.f);
    glm::vec3 interspace( spacing, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

	bool outline = false;

	if( outlineSize != 0 )
		outline = true;

    float blurTexSize = float( blurSize );
    float blurLenghtX = float( blurSize ) / viewWidth;
    float blurLenghtY = float( blurSize ) / viewHeight;

    float ccPaddingX = 1.f / viewWidth;
    float ccPaddingY = 1.f / viewHeight;

    float texPadding = 1.f;

    auto spaceGlyphWidth    = (float)textAtlas->GetGlyph( L'0', outline )->width / viewWidth  + spacing;
	auto newLineShift       = -(float) 1.5f * textAtlas->GetGlyph( L'0', outline )->height / viewHeight;

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

        model::ConnectedComponentPtr connComp = model::ConnectedComponent::Create();

        model::AttributeChannelDescriptor * desc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        auto posAttribChannel = new model::Float3AttributeChannel( desc, "vertexPosition", true );

        if( auto glyph = textAtlas->GetGlyph( wch, outline ) )
        {
            glm::vec3 bearing = glm::vec3( (float)glyph->bearingX / (float)viewWidth, (float)( glyph->bearingY - (int)glyph->height ) / (float)viewHeight, 0.f );

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
                quadBottomRight    = glm::vec3( (float)glyph->width / (float)viewWidth, 0.f, 0.f ) +  glm::vec3( blurLenghtX, -blurLenghtY, 0.f ) + glm::vec3( ccPaddingX, -ccPaddingY, 0.f );
                quadTopLeft        = glm::vec3( 0.f, (float)glyph->height / (float)viewHeight, 0.f ) + glm::vec3( -blurLenghtX, blurLenghtY, 0.f ) + glm::vec3( -ccPaddingX, ccPaddingY, 0.f );
                quadTopRight       = glm::vec3( (float)glyph->width / (float)viewWidth, (float)glyph->height / (float)viewHeight, 0.f ) + glm::vec3( blurLenghtX, blurLenghtY, 0.f ) + glm::vec3( ccPaddingX, ccPaddingY, 0.f );
            }

            posAttribChannel->AddAttribute( quadBottomLeft    + translate + bearing + newLineTranslation );
            posAttribChannel->AddAttribute( quadBottomRight   + translate + bearing + newLineTranslation );
            posAttribChannel->AddAttribute( quadTopLeft       + translate + bearing + newLineTranslation );
            posAttribChannel->AddAttribute( quadTopRight      + translate + bearing + newLineTranslation );

            connComp->AddAttributeChannel( model::AttributeChannelPtr( posAttribChannel ) );

            model::AttributeChannelDescriptor * desc1 = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            auto verTex0AttrChannel = new model::Float2AttributeChannel( desc1, "textAtlasPosition", true );

            float texLeft;
            float texTop;
            float texWidth;
            float texHeight;

            {
                texLeft   = ( (float)glyph->textureX - blurTexSize - texPadding )  / textAtlas->GetWidth();
                texTop    = ( (float)glyph->textureY - blurTexSize - texPadding )  / textAtlas->GetHeight();
                texWidth  = ( (float)glyph->width + 2 * blurTexSize + 2 * texPadding )     / textAtlas->GetWidth();
                texHeight = ( (float)glyph->height + 2 * blurTexSize  + 2 * texPadding )    / textAtlas->GetHeight();
            }


            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop + texHeight ) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop + texHeight ) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop) );
            verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop ) );

            connComp->AddAttributeChannel( model::AttributeChannelPtr( verTex0AttrChannel ) );

            vertexAttributeChannel->AddConnectedComponent( connComp );

            {
				translate += glm::vec3( ( glyph->advanceX ) / (float)viewWidth, 0.f, 0.f ) + interspace;
            }
        }
        else
        {
			translate += glm::vec3( spaceGlyphWidth, 0.f, 0.f )+ interspace;
            //assert( !( "Cannot find glyph for char " + wch) );
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
            auto & verts = std::static_pointer_cast< model::Float3AttributeChannel >( cc->GetAttributeChannels()[ 0 ] )->GetVertices();

            for ( auto & v : verts )
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

} // bv
