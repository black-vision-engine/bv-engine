#include "stdafx.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


#include "Text3dUtils.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include <FreeType/ftglyph.h>



#include "Assets/Assets.h"
#include "Assets/Font/Glyph.h"


#include "Memory/MemoryLeaks.h"


namespace bv { namespace model
{


// *********************************
//
VertexAttributesChannelPtr   Text3DUtils::CreateEmptyVACForText3D()
{
    VertexAttributesChannelDescriptor vacDesc;

    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    return std::make_shared< VertexAttributesChannel>( PrimitiveType::PT_LINES, vacDesc );
}

// ***********************
//
ConnectedComponentPtr		Text3DUtils::CreateEmptyCC				()
{
	model::ConnectedComponentPtr connComp = model::ConnectedComponent::Create();

	auto desc = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

	auto posAttribChannel = std::make_shared< model::Float3AttributeChannel >( desc, "vertexPosition", true );

	posAttribChannel->AddAttribute( glm::vec3() );

	connComp->AddAttributeChannel( model::AttributeChannelPtr( posAttribChannel ) );

	return connComp;
}

// ***********************
//
std::vector< ConnectedComponentPtr >     Text3DUtils::CreateText                  ( const std::wstring & text, FontAsset3DConstPtr textAsset, TextLayout layout )
{
    std::vector< ConnectedComponentPtr > letters;

    auto textObj = layout.FontAsset->GetText();
    auto textRepresentation = layout.FontAsset->GetTextGeometry();
    auto viewWidth = layout.ViewWidth;
    auto viewHeight = layout.ViewHeight;
    float newLineSize = layout.NewLineSize;

    assert( textObj );

    // Note: Atlas was created with specific font size. We must tak it into consideration while rescaling geometry.
    float fontRatio = layout.Size / textObj->GetFontSize();
    float aspectRatio = float( std::min( viewWidth, viewHeight ) ) / 2.f;
    float scaleRatio = aspectRatio / fontRatio;


    // Space width should be get form : https://www.mail-archive.com/freetype@nongnu.org/msg01384.html
    auto spaceGlyphWidth = (float)textRepresentation->GetGlyph( L'0' )->advanceX / scaleRatio / 2 + layout.Spacing;
    auto newLineShift = -(float)newLineSize * textRepresentation->GetGlyph( L'0' )->height / scaleRatio;


    TextLayoutInfo layoutInfo;
    layoutInfo.AlignChar = layout.AlignChar;
    layoutInfo.AspectRatio = scaleRatio;
    layoutInfo.Interspace = layout.Spacing;
    layoutInfo.NewLineSize = newLineShift;
    layoutInfo.SpaceSize = spaceGlyphWidth;
    layoutInfo.TextAlign = layout.Tat;
    layoutInfo.UseKerning = layout.UseKerning;
    layoutInfo.UseOutline = false;
    layoutInfo.MaxLength = layout.Box.x;

    bool useBox = layout.UseBox;
    auto textLayout = TextHelper::LayoutLetters( text, textRepresentation, layoutInfo, useBox );

    unsigned int componentIdx = 0;
    for( unsigned int i = 0; i < text.size(); ++i )
    {
        auto wch = text[ i ];
        glm::vec3 translate = glm::vec3( textLayout[ i ].x, 0.0, 0.0 );
        glm::vec3 newLineTranslate = glm::vec3( 0.0f, textLayout[ i ].y, 0.0 );

        if( useBox && abs( newLineTranslate.y ) > layout.Box.y )
            break;

        if( TextHelper::IsWhitespace( wch ) )
            continue;

        glm::vec3 letterTranslate = translate + newLineTranslate;

        letters.push_back( CreateLetter( wch, textAsset, layout ) );
        auto attributeChannel = letters.back()->GetAttributeChannels();
        auto posChannel = std::static_pointer_cast< Float3AttributeChannel >( attributeChannel[ 0 ] );
        assert( posChannel->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION );

        // Note: FreeType unit is 1/64th of a pixel.
        glm::vec3 scaleFactor( 1.0f / ( scaleRatio * 64.0f ), 1.0f / ( scaleRatio * 64.0f ), 1.0f );

        for( auto & pos : posChannel->GetVertices() )
        {
            pos = scaleFactor * pos + letterTranslate;
        }

        componentIdx++;
    }

	if( letters.empty() )
		letters.push_back( CreateEmptyCC() );

    return letters;
}

// ***********************
//
std::vector< glm::vec3 >                Text3DUtils::CreateLinesFromContour         ( const ContoursList & contours )
{
    std::vector< glm::vec3 > result;

    SizeType numPoints = 0;
    for( auto & contour : contours )
    {
        numPoints += contour->PointCount();
    }

    result.reserve( 2 * numPoints );

    for( auto & contour : contours )
    {
        for( int i = 0; i < contour->PointCount() - 1; ++i )
        {
            result.push_back( glm::vec3( contour->Point( i ).X(), contour->Point( i ).Y(), 0.0f ) );
            result.push_back( glm::vec3( contour->Point( i + 1 ).X(), contour->Point( i + 1 ).Y(), 0.0f ) );
        }

        auto lastPointIdx = contour->PointCount() - 1;

        result.push_back( glm::vec3( contour->Point( lastPointIdx ).X(), contour->Point( lastPointIdx ).Y(), 0.0f ) );
        result.push_back( glm::vec3( contour->Point( 0 ).X(), contour->Point( 0 ).Y(), 0.0f ) );
    }

    return result;
}

// ***********************
//
ConnectedComponentPtr                    Text3DUtils::CreateLetter                  ( const wchar_t character, FontAsset3DConstPtr & textAsset, TextLayout layout )
{
    ConnectedComponentPtr       component = ConnectedComponent::Create();

    model::AttributeChannelDescriptorPtr posDesc = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    Float3AttributeChannelPtr   positions = std::make_shared< Float3AttributeChannel >( posDesc, "vertexPosition", false );

    component->AddAttributeChannel( positions );

    auto contoursVec = textAsset->GetTextGeometry()->CreateCharacter( character, layout.Size );
    auto positionsVec = CreateLinesFromContour( contoursVec );
    positions->ReplaceAttributes( std::move( positionsVec ) );

    return component;
}


void                                     Text3DUtils::ArrangeText                 ( const std::wstring& text, std::vector< ConnectedComponentPtr > & components, TextLayout layout )
{
    auto textObj = layout.FontAsset->GetText();
    auto textRepresentation = layout.FontAsset->GetTextGeometry();
    auto viewWidth = layout.ViewWidth;
    auto viewHeight = layout.ViewHeight;
    float newLineSize = layout.NewLineSize;

    assert( textObj );

    // Note: Atlas was created with specific font size. We must tak it into consideration while rescaling geometry.
    float fontRatio = layout.Size / textObj->GetFontSize();
    float aspectRatio = float( std::min( viewWidth, viewHeight ) ) / 2.f;
    float scaleRatio = aspectRatio / fontRatio;


    // Space width should be get form : https://www.mail-archive.com/freetype@nongnu.org/msg01384.html
    auto spaceGlyphWidth    = (float)textRepresentation->GetGlyph( L'0' )->advanceX / scaleRatio / 2 + layout.Spacing;
    auto newLineShift       = -(float)newLineSize * textRepresentation->GetGlyph( L'0' )->height / scaleRatio;


    TextLayoutInfo layoutInfo;
    layoutInfo.AlignChar = layout.AlignChar;
    layoutInfo.AspectRatio = scaleRatio;
    layoutInfo.Interspace = layout.Spacing;
    layoutInfo.NewLineSize = newLineShift;
    layoutInfo.SpaceSize = spaceGlyphWidth;
    layoutInfo.MaxLength = std::numeric_limits< float >::infinity();
    layoutInfo.TextAlign = layout.Tat;
    layoutInfo.UseKerning = layout.UseKerning;
    layoutInfo.UseOutline = false;
    layoutInfo.MaxLength = layout.Box.x;

    bool useBox = layout.Box.x > 0.0f ? true : false;
    auto textLayout = TextHelper::LayoutLetters( text, textRepresentation, layoutInfo, useBox );

    unsigned int componentIdx = 0;
    for( unsigned int i = 0; i < text.size(); ++i )
    {
        auto wch = text[ i ];
        glm::vec3 translate = glm::vec3( textLayout[ i ].x, 0.0, 0.0 );
        glm::vec3 newLineTranslate = glm::vec3( 0.0f, textLayout[ i ].y, 0.0 );

        if( TextHelper::IsWhitespace( wch ) )
            continue;


        glm::vec3 letterTranslate = translate + newLineTranslate;// -glm::vec3( ccPaddingX, ccPaddingY, 0.0 );


        auto attributeChannel = components[ componentIdx ]->GetAttributeChannels();
        auto posChannel = std::static_pointer_cast< Float3AttributeChannel >( attributeChannel[ 0 ] );
        assert( posChannel->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION );

        // Note: FreeType unit is 1/64th of a pixel.
        glm::vec3 scaleFactor( 1.0f / ( scaleRatio * 64.0f ), 1.0f / ( scaleRatio * 64.0f ), 1.0f );

        for( auto & pos : posChannel->GetVertices() )
        {
            pos = scaleFactor * pos + letterTranslate;
        }

        componentIdx++;
    }
}

} // model
} // bv
