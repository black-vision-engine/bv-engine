#include "stdafx.h"

#include "TextHelper.h"

#include "Assets/Assets.h"
#include "Assets/Font/Glyph.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/TextArranger.h"


namespace bv {

///////////////////////////////
//
// @todo Raplace all references with LoadTypedAsset
FontAsset2DConstPtr     TextHelper::LoadFont( const FontAssetDescConstPtr & fontAssetDesc )
{
    return std::static_pointer_cast< const FontAsset2D >( AssetManager::GetInstance().LoadAsset( fontAssetDesc ) );
}

// *********************************
//
model::VertexAttributesChannelPtr   TextHelper::CreateEmptyVACForText()
{
    model::VertexAttributesChannelDescriptor vacDesc;

    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_ATLASCOORD, ChannelRole::CR_PROCESSOR );
    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_CUSTOM, ChannelRole::CR_PROCESSOR );
	vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_CUSTOM, ChannelRole::CR_PASSTHROUGH );

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

    auto desc = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto posAttribChannel = std::make_shared< model::Float3AttributeChannel >( desc, "vertexPosition", true );

    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );
    posAttribChannel->AddAttribute( glm::vec3() );

    connComp->AddAttributeChannel( model::AttributeChannelPtr( posAttribChannel ) );

    auto desc1 = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    auto verTex0AttrChannel = std::make_shared< model::Float2AttributeChannel >( desc1, "textAtlasPosition", true );

    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );
    verTex0AttrChannel->AddAttribute( glm::vec2() );

    connComp->AddAttributeChannel( model::AttributeChannelPtr( verTex0AttrChannel ) );

    auto desc2 = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    auto ccCenterChannel = std::make_shared< model::Float2AttributeChannel >( desc2, "ccCenter", true );

    ccCenterChannel->AddAttribute( glm::vec2() );
    ccCenterChannel->AddAttribute( glm::vec2() );
    ccCenterChannel->AddAttribute( glm::vec2() );
    ccCenterChannel->AddAttribute( glm::vec2() );

    connComp->AddAttributeChannel( model::AttributeChannelPtr( ccCenterChannel ) );

	auto desc3 = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_CUSTOM, ChannelRole::CR_PASSTHROUGH );

	auto ccIdChannel = std::make_shared< model::Float2AttributeChannel >( desc3, "cc_num", true );

	ccIdChannel->AddAttribute( glm::vec2() );
	ccIdChannel->AddAttribute( glm::vec2() );
	ccIdChannel->AddAttribute( glm::vec2() );
	ccIdChannel->AddAttribute( glm::vec2() );

	connComp->AddAttributeChannel( model::AttributeChannelPtr( ccIdChannel ) );

    return connComp;
}

} // anonymous


TextAtlasConstPtr				TextHelper::GetAtlas            ( const AssetConstPtr & asset )
{
    auto font2D = std::static_pointer_cast< const FontAsset2D >( asset );

    if( font2D )
    {
        return font2D->GetTextAtlas();
    }
    else
    {
        return nullptr;
    }
}

// *********************************
//
std::vector< glm::vec2 >		TextHelper::GetAtlasCoordsForGlyph	( const Glyph * glyph, SizeType atlasW, SizeType atlasH, Float32 atlasBlurSize )
{
	std::vector< glm::vec2 > ret;

	float texPadding = 1.f;

	auto texLeft = ( ( float ) glyph->textureX - atlasBlurSize - texPadding ) / atlasW;
	auto texTop = ( ( float ) glyph->textureY - atlasBlurSize - texPadding ) / atlasH;
	auto texWidth = ( ( float ) glyph->width + 2 * atlasBlurSize + 2 * texPadding ) / atlasW;
	auto texHeight = ( ( float ) glyph->height + 2 * atlasBlurSize + 2 * texPadding ) / atlasH;

	ret.push_back( glm::vec2( texLeft, texTop + texHeight ) );
	ret.push_back( glm::vec2( texLeft + texWidth, texTop + texHeight ) );
	ret.push_back( glm::vec2( texLeft, texTop ) );
	ret.push_back( glm::vec2( texLeft + texWidth, texTop ) );

	return ret;
}

// *********************************
//
float							TextHelper::BuildVACForText     ( model::VertexAttributesChannel * vertexAttributeChannel, const TextAtlasConstPtr & textAtlas, const std::wstring & text, SizeType blurSize, float spacing, TextAlignmentType tat, wchar_t alignChar, SizeType outlineSize, UInt32 viewWidth, UInt32 viewHeight, float newLineSize, glm::vec2 box, model::TextArranger * arranger, bool useKerning, bool useBox )
{
    assert( vertexAttributeChannel );
    assert( textAtlas );

    bool outline = false;
    if( outlineSize != 0 )
        outline = true;

    float aspectRatio = float( std::min( viewWidth, viewHeight ) ) / 2.f;

    float blurTexSize = float( blurSize );
    float blurLenghtX = float( blurSize ) / aspectRatio;
    float blurLenghtY = float( blurSize ) / aspectRatio;


    float ccPaddingX = 1.f / aspectRatio;
    float ccPaddingY = 1.f / aspectRatio;


    // Computing space character size
    // Space width should be get form : https://www.mail-archive.com/freetype@nongnu.org/msg01384.html
    auto spaceGlyphWidth    = (float)textAtlas->GetGlyph( L'0', outline )->advanceX / aspectRatio / 2 + spacing;
    auto newLineShift       = -(float)newLineSize * textAtlas->GetGlyph( L'0', outline )->height / aspectRatio;


    TextLayoutInfo layoutInfo;
    layoutInfo.AlignChar = alignChar;
    layoutInfo.AspectRatio = aspectRatio;
    layoutInfo.Interspace = spacing;
    layoutInfo.NewLineSize = newLineShift;
    layoutInfo.SpaceSize = spaceGlyphWidth;
    layoutInfo.MaxLength = box.x;
    layoutInfo.TextAlign = tat;
    layoutInfo.UseKerning = useKerning;
    layoutInfo.UseOutline = outline;


    auto textLayout = TextHelper::LayoutLetters( text, textAtlas, layoutInfo, useBox );

    for( unsigned int i = 0; i < text.size(); ++i )
    {
        auto wch = text[ i ];
        glm::vec3 translate = glm::vec3( textLayout[ i ].x, 0.0, 0.0 );
        glm::vec3 newLineTranslate = glm::vec3( 0.0f, textLayout[ i ].y, 0.0 );

        if( useBox && abs( newLineTranslate.y ) > box.y )
            break;

        if( IsWhitespace( wch ) )
            continue;

        model::ConnectedComponentPtr connComp = model::ConnectedComponent::Create();

        auto desc = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        auto posAttribChannel = std::make_shared< model::Float3AttributeChannel >( desc, "vertexPosition", true );

        if( auto glyph = textAtlas->GetGlyph( wch, outline ) )
        {
            glm::vec3 bearing = glm::vec3( ( (float)glyph->bearingX - float( outlineSize ) ) / aspectRatio, ( float( glyph->bearingY ) + float( outlineSize ) ) / aspectRatio, 0.f );

            glm::vec3 quadBottomLeft;
            glm::vec3 quadBottomRight;
            glm::vec3 quadTopLeft;
            glm::vec3 quadTopRight;

            // XYZ

            {
                quadTopLeft = glm::vec3( 0.f, 0.f, 0.f ) + glm::vec3( -blurLenghtX, blurLenghtY, 0.f ) + glm::vec3( -ccPaddingX, -ccPaddingY, 0.f );
                quadTopRight = glm::vec3( (float)glyph->width / aspectRatio, 0.f, 0.f ) + glm::vec3( blurLenghtX, blurLenghtY, 0.f ) + glm::vec3( ccPaddingX, -ccPaddingY, 0.f );
                quadBottomLeft = glm::vec3( 0.f, -(float)glyph->height / aspectRatio, 0.f ) + glm::vec3( -blurLenghtX, -blurLenghtY, 0.f ) + glm::vec3( -ccPaddingX, ccPaddingY, 0.f );
                quadBottomRight = glm::vec3( (float)glyph->width / aspectRatio, -(float)glyph->height / aspectRatio, 0.f ) + glm::vec3( blurLenghtX, -blurLenghtY, 0.f ) + glm::vec3( ccPaddingX, ccPaddingY, 0.f );
            }

            posAttribChannel->AddAttribute( quadBottomLeft + translate + bearing + newLineTranslate );
            posAttribChannel->AddAttribute( quadBottomRight + translate + bearing + newLineTranslate );
            posAttribChannel->AddAttribute( quadTopLeft + translate + bearing + newLineTranslate );
            posAttribChannel->AddAttribute( quadTopRight + translate + bearing + newLineTranslate );

            connComp->AddAttributeChannel( model::AttributeChannelPtr( posAttribChannel ) );

            // UV

            auto desc1 = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_ATLASCOORD, ChannelRole::CR_PROCESSOR );

            auto verTex0AttrChannel = std::make_shared< model::Float2AttributeChannel >( desc1, "textAtlasPosition", true );

            auto uvs = GetAtlasCoordsForGlyph( glyph, textAtlas->GetWidth(), textAtlas->GetHeight(), blurTexSize );

            verTex0AttrChannel->AddAttributes( uvs );

            connComp->AddAttributeChannel( model::AttributeChannelPtr( verTex0AttrChannel ) );

            // CC CENTER

            auto desc2 = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_CUSTOM, ChannelRole::CR_PROCESSOR );

            auto ccCenterAttrChannel = std::make_shared< model::Float2AttributeChannel >( desc2, "ccCenter", true );

            auto bottomLeft = ( quadBottomLeft + translate + bearing + newLineTranslate );
            auto topRight = ( quadTopRight + translate + bearing + newLineTranslate );

            float centerX = ( bottomLeft.x + topRight.x ) / 2.f;
            float centerY = ( bottomLeft.y + topRight.y ) / 2.f;

            ccCenterAttrChannel->AddAttribute( glm::vec2( centerX, centerY ) );
            ccCenterAttrChannel->AddAttribute( glm::vec2( centerX, centerY ) );
            ccCenterAttrChannel->AddAttribute( glm::vec2( centerX, centerY ) );
            ccCenterAttrChannel->AddAttribute( glm::vec2( centerX, centerY ) );

            connComp->AddAttributeChannel( model::AttributeChannelPtr( ccCenterAttrChannel ) );

			// CC ID

			auto desc3 = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_CUSTOM, ChannelRole::CR_PASSTHROUGH );

			auto ccIdAttrChannel = std::make_shared< model::Float2AttributeChannel >( desc3, "cc_num", true );

			auto k = Float32( vertexAttributeChannel->GetComponents().size() );

			ccIdAttrChannel->AddAttribute( glm::vec2( k, 0.f ) );
			ccIdAttrChannel->AddAttribute( glm::vec2( k, 0.f ) );
			ccIdAttrChannel->AddAttribute( glm::vec2( k, 0.f ) );
			ccIdAttrChannel->AddAttribute( glm::vec2( k, 0.f ) );

			connComp->AddAttributeChannel( model::AttributeChannelPtr( ccIdAttrChannel ) );

			vertexAttributeChannel->AddConnectedComponent( connComp );

        }
    }

	auto ccNum = Float32( vertexAttributeChannel->GetComponents().size() );

	for( auto & cc : vertexAttributeChannel->GetComponents() )
	{
		auto typedAC = std::static_pointer_cast< model::Float2AttributeChannel >( cc->GetAttributeChannels()[ 3 ] );
		for( int i = 0; i < 4; ++i )
		{
			typedAC->GetVertices()[ i ].y = ccNum;
		}
	}

    auto components = vertexAttributeChannel->GetComponents();
    if( components.empty() ) // FIXME: We add one empty CC because of bug #72174842
    {
        vertexAttributeChannel->AddConnectedComponent( CreateEmptyCC() );
    }

    if( arranger )
    {
        vertexAttributeChannel = arranger->Arange( vertexAttributeChannel );
    }

    // FIXME: Is this necessary ??? What for ?
    if( textLayout.size() != 0 )
        return textLayout.back().x; // FIXME: This does not work for multiline text
    else
        return 0.0f;
}



// ***********************
//
float               TextHelper::ComputeAlignement( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot )
{
    auto alignmentTranslation = 0.f;
    switch( tat )
    {
        case TextAlignmentType::Center:
            alignmentTranslation = -translate.x / 2.f;
            break;
        case TextAlignmentType::Right:
            alignmentTranslation = -translate.x;
            break;
        case TextAlignmentType::Character:
            if( translateDot.x > 0.0f )
            {
                alignmentTranslation = -translateDot.x;
            }
            else
            {
                // No special character in current line. Align to right.
                alignmentTranslation = -translate.x;
            }
            break;
    }

    return alignmentTranslation;
}

// ***********************
//
std::vector< glm::vec3 >        TextHelper::LayoutLetters       ( const std::wstring & text, TextRepresentationConstPtr textRepr, TextLayoutInfo & layout, bool useBox )
{
    std::vector< glm::vec3 > resultLayout;
    resultLayout.reserve( text.length() );

    if( useBox )
        // Note: This should be equal to max advance (with outline if exists) + interspace + max kerning. To much computing. For now
        // I take doubled advance and hope it's ok. If something hang, you know what to do.
        layout.MaxLength = std::max( layout.MaxLength, 2 * (float)textRepr->ComputeMaxAdvanceX() / layout.AspectRatio + layout.Interspace );

    glm::vec3 translate( 0.0, 0.0, 0.0 );
    glm::vec3 newLineTranslation( 0.0, 0.0, 0.0 );
    glm::vec3 translateDot( 0.f );

    unsigned int lastSpace = 0;
    glm::vec3 lastSpaceTranslate( 0.0, 0.0, 0.0 );
    glm::vec3 lastGlyphTranslate( 0.0, 0.0, 0.0 );

    unsigned int lineBeginIdx = 0;
    unsigned int lineEndIdx = 0;
    unsigned int i = 0;

    while( i < text.size() )
    {
        lineBeginIdx = i;

        for( ; i < text.size(); ++i )
        {
            auto wch = text[ i ];
            lineEndIdx = i + 1;         // All letters are potencial end of line.

            if( wch == L' ' )
            {
                lastSpace = i;
                lastSpaceTranslate = translate;

                translate += glm::vec3( layout.SpaceSize, 0.f, 0.f ) + glm::vec3( layout.Interspace, 0.0, 0.0 );
                resultLayout.push_back( translate );
                continue;
            }

            if( wch == L'\n' || wch == L'\r' )
            {
                lastSpace = i;

                newLineTranslation += glm::vec3( 0.f, layout.NewLineSize, 0.f );
                resultLayout.push_back( translate );
                break;
            }

            if( auto glyph = textRepr->GetGlyph( wch, layout.UseOutline ) )
            {
                auto kerningShift = glm::vec3( 0.f, 0.f, 0.f );

                if( layout.UseKerning && i > 0 )
                {
                    auto kerShift = textRepr->GetKerning( text[ i - 1 ], text[ i ] );
                    kerningShift.x = kerShift / layout.AspectRatio;
                    translate += kerningShift;
                }

                if( wch == layout.AlignChar && layout.TextAlign == TextAlignmentType::Character )
                {
                    translateDot = translate;
                }

                resultLayout.push_back( translate + newLineTranslation );
                lastGlyphTranslate = translate;

                translate += glm::vec3( ( glyph->advanceX ) / layout.AspectRatio, 0.f, 0.f ) + glm::vec3( layout.Interspace, 0.0, 0.0 );
            }
            else
            {
                resultLayout.push_back( translate + newLineTranslation );
                translate += glm::vec3( layout.SpaceSize, 0.f, 0.f ) + glm::vec3( layout.Interspace, 0.0, 0.0 );
            }

            if( useBox && translate.x > layout.MaxLength )
            {
                newLineTranslation += glm::vec3( 0.f, layout.NewLineSize, 0.f );

                // Note: If last space is enter, we break word and begin next line.
                if( !( lineBeginIdx - 1 == lastSpace ) )
                {
                    i = lastSpace;  // This will ommit space. We have new line instead.
                    translate = lastSpaceTranslate;
                    lineEndIdx = lastSpace;

                    resultLayout.erase( resultLayout.begin() + ( lastSpace + 1 ), resultLayout.end() );
                }
                else
                {
                    resultLayout.pop_back();

                    translate = lastGlyphTranslate;
                    lineEndIdx = i;
                    i--;
                    lastSpace = i;
                }

                break;
            }
        }

        TextHelper::ApplyAlignement( layout.TextAlign, translate, translateDot, text, resultLayout, layout.MaxLength, lineBeginIdx, lineEndIdx );

        // End of line (or text) reached.
        translateDot = glm::vec3( 0.f );
        translate = glm::vec3( 0.f );
        ++i;
    }


    return resultLayout;
}

// ***********************
//
void                TextHelper::ApplyAlignementPC   ( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot, std::vector< model::IConnectedComponentPtr > & components )
{
    auto alignmentTranslation = ComputeAlignement( tat, translate, translateDot );

    if( tat != TextAlignmentType::Left )
    {
        for( auto cc : components )
        {
            auto & verts = std::static_pointer_cast< model::Float3AttributeChannel >( cc->GetAttributeChannels()[ 0 ] )->GetVertices();

            for( auto & v : verts )
            {
                v.x += alignmentTranslation;
            }

            auto & centers = std::static_pointer_cast< model::Float2AttributeChannel >( cc->GetAttributeChannels()[ 2 ] )->GetVertices();

            for( auto & c : centers )
            {
                c.x += alignmentTranslation;
            }
        }

    }
}

// ***********************
//
void                TextHelper::ApplyAlignementP    ( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot, std::vector< model::ConnectedComponentPtr > & components )
{
    auto alignmentTranslation = ComputeAlignement( tat, translate, translateDot );

    if( tat != TextAlignmentType::Left )
    {
        for( auto cc : components )
        {
            auto & verts = std::static_pointer_cast< model::Float3AttributeChannel >( cc->GetAttributeChannels()[ 0 ] )->GetVertices();

            for( auto & v : verts )
            {
                v.x += alignmentTranslation;
            }
        }
    }
}

// ***********************
//
void                TextHelper::ApplyAlignement     ( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot, const std::wstring& text, std::vector< glm::vec3 > & layout, float boxWidth, int beginIdx, int endIdx )
{
    if( tat == TextAlignmentType::Justification )
    {
        auto widthUnderflow = boxWidth - translate.x;
        auto iterBegin = text.begin() + beginIdx;
        auto iterEnd = text.begin() + endIdx;

        // Find first non-space character.
        iterBegin = std::find_if_not( iterBegin, iterEnd, []( wchar_t c ) { return c == ' ' ? true : false; } );
        // Find all spaces which will be extended.
        auto numSpaces = std::count_if( iterBegin, iterEnd, []( wchar_t c ) { return c == ' ' ? true : false; } );

        float additionalWidth = widthUnderflow / numSpaces;
        float currentAdd = 0.0f;
        for( ; iterBegin != iterEnd; iterBegin++ )
        {
            if( *iterBegin == ' ' )
            {
                currentAdd += additionalWidth;
            }

            auto layoutIdx = std::distance( text.begin(), iterBegin );
            layout[ layoutIdx ].x += currentAdd;
        }

    }
    else if( tat != TextAlignmentType::Left )
    {
        auto alignmentTranslation = ComputeAlignement( tat, translate, translateDot );

        for( auto iter = layout.begin() + beginIdx; iter != layout.begin() + endIdx; iter++ )
        {
            iter->x += alignmentTranslation;
        }
    }
}


// ***********************
//
bool                TextHelper::IsWhitespace        ( wchar_t character )
{
    if( character == L' ' || character == L'\n' || character == L'\r' )
        return true;
    return false;
}

// ***********************
//
bool                TextHelper::IsEnter             ( wchar_t character )
{
    if( character == L'\n' || character == L'\r' )
        return true;
    return false;
}

} // bv
