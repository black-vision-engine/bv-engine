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
std::vector< ConnectedComponentPtr >     Text3DUtils::CreateText                  ( const std::wstring & text, FontAsset3DConstPtr textAsset, TextLayout layout )
{
    std::vector< ConnectedComponentPtr > letters;

    for( int l = 0; l < text.size(); ++l )
    {
        if( text[ l ] == L' ' ||
            text[ l ] == L'\n' ||
            text[ l ] == L'\r' )
            continue;

        ConnectedComponentPtr component = CreateLetter( text[ l ], textAsset, layout );
        assert( component );

        letters.push_back( component );
    }

    ArrangeText( text, letters, layout );

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
    auto spacing = layout.Spacing;
    auto viewWidth = layout.ViewWidth;
    auto viewHeight = layout.ViewHeight;
    auto alignement = layout.Tat;
    bool useKerning = layout.UseKerning;

    assert( textObj );

    glm::vec3 translate(0.f);
    glm::vec3 translateDot(0.f);
    glm::vec3 interspace( spacing, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );


    // Note: Atlas was created with specific font size. We must tak it into consideration while rescaling geometry.
    float fontRatio = layout.Size / textObj->GetFontSize();
    float aspectRatio = float( std::min( viewWidth, viewHeight ) ) / 2.f;
    float scaleRatio = aspectRatio / fontRatio;


    //float ccPaddingX = 1.f / aspectRatio;
    //float ccPaddingY = 1.f / aspectRatio;

    // Space width should be get form : https://www.mail-archive.com/freetype@nongnu.org/msg01384.html
    auto spaceGlyphWidth    = (float)textRepresentation->GetGlyph( L'0' )->width / scaleRatio + spacing;
    auto newLineShift       = -(float) 1.5f * textRepresentation->GetGlyph( L'0' )->height / scaleRatio;

    unsigned int componentIdx = 0;
    unsigned int lineBeginComponentIdx = 0;
    unsigned int i = 0;

    while( i < text.size() )
    {
        lineBeginComponentIdx = componentIdx;

        for( ; i < text.size(); ++i )
        {
            auto wch = text[ i ];

            if( wch == L' ' )
            {
                translate += glm::vec3( spaceGlyphWidth, 0.f, 0.f ) + interspace;
                continue;
            }

            if( wch == L'\n' || wch == L'\r' )
            {
                newLineTranslation += glm::vec3( 0.f, newLineShift, 0.f );
                break;
            }


            if( auto glyph = textRepresentation->GetGlyph( wch ) )
            {
                auto kerningShift = glm::vec3( 0.f, 0.f, 0.f );

                if( useKerning && i > 0 )
                {
                    auto kerShift = textRepresentation->GetKerning( text[ i - 1 ], text[ i ] );
                    kerningShift.x = kerShift / scaleRatio;
                    translate += kerningShift;
                }

                glm::vec3 letterTranslate = translate + newLineTranslation;// -glm::vec3( ccPaddingX, ccPaddingY, 0.0 );

                auto attributeChannel = components[ componentIdx ]->GetAttributeChannels();
                auto posChannel = std::static_pointer_cast< Float3AttributeChannel >( attributeChannel[ 0 ] );
                assert( posChannel->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION );

                // Note: FreeType unit is 1/64th of a pixel.
                glm::vec3 scaleFactor( 1.0f / ( scaleRatio * 64.0f ), 1.0f / ( scaleRatio * 64.0f ), 1.0f );

                for( auto & pos : posChannel->GetVertices() )
                {
                    pos = scaleFactor * pos + letterTranslate;
                }

                if( wch == L'.' && layout.Tat == TextAlignmentType::Dot )
                {
                    translateDot = translate;
                }

                {
                    translate += glm::vec3( ( glyph->advanceX ) / scaleRatio, 0.f, 0.f ) + interspace;
                }

                componentIdx++;
            }
            else
            {
                translate += glm::vec3( spaceGlyphWidth, 0.f, 0.f ) + interspace;
                //assert( !( "Cannot find glyph for char " + wch) );
            }
        }

        // Apply alignement to current line and go to next.
        std::vector< model::ConnectedComponentPtr > lineComponents( components.begin() + lineBeginComponentIdx, components.begin() + componentIdx );

        TextHelper::ApplyAlignementP( alignement, translate, translateDot, lineComponents );

        // End of line (or text) reached.
        translate = glm::vec3( 0.f );
        ++i;
    }


}

} // model
} // bv
