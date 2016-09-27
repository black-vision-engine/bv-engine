#include "stdafx.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


#include "Text3dUtils.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include <FreeType/ftglyph.h>


#include "Assets/Font/TextAtlas.h"

#include "Assets/Assets.h"
#include "Assets/Font/Glyph.h"


#include "Memory/MemoryLeaks.h"


namespace bv { namespace model
{

// *********************************
//
namespace
{



} // anonymous

//// *********************************
////
//Float3AttributeChannelConstPtr      Text3DUtils::CreateAttrChannel( const wchar_t & wch, const std::string & fontPath, SizeType size )
//{
//    {
//        wch;
//        fontPath;
//        size;
//    }
//    
//    FTGLPolygonFont font( fontPath.c_str() );
//
//    if( font.Error() )
//        return nullptr;
//
//    font.FaceSize( size );
//    
//    //FTPolygonGlyph glyph;
//
//    return nullptr;
//}

// *********************************
//
VertexAttributesChannelPtr   Text3DUtils::CreateEmptyVACForText3D()
{
    VertexAttributesChannelDescriptor vacDesc;

    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    vacDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR );

    return std::make_shared< VertexAttributesChannel>( PrimitiveType::PT_TRIANGLES, vacDesc );
}

// ***********************
//
std::vector< ConnectedComponentPtr >     Text3DUtils::CreateText                  ( const std::wstring& text, TextConstPtr textAsset, TextLayout layout )
{
    std::vector< ConnectedComponentPtr > letters;

    for( int l = 0; l < text.size(); ++l )
    {
        if( text[ l ] == L' ' ||
            text[ l ] == L'\n' ||
            text[ l ] == L'\r' )
            continue;

        ConnectedComponentPtr component = nullptr;

        //// Find previous use of this letter.
        //for( int i = 0; i < l; ++i )
        //{
        //    if( text[ i ] == text[ l ] )
        //    {
        //        component = letters[ i ];
        //        break;
        //    }
        //}

        // Component not found in previously used letters. We must crate it.
        if( component == nullptr )
        {
            component = CreateLetter( text[ l ], textAsset, layout );
            assert( component );
        }

        letters.push_back( component );
    }

    ArrangeText( text, letters, layout );

    return letters;
}

// ***********************
//
ConnectedComponentPtr                    Text3DUtils::CreateLetter                ( const wchar_t character, TextConstPtr& textAsset, TextLayout layout )
{
    ConnectedComponentPtr       component = ConnectedComponent::Create();

    model::AttributeChannelDescriptorPtr posDesc = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    model::AttributeChannelDescriptorPtr normDesc = std::make_shared< model::AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR );

    Float3AttributeChannelPtr   positions = std::make_shared< Float3AttributeChannel >( posDesc, "vertexPosition", false );
    Float3AttributeChannelPtr   normals = std::make_shared< Float3AttributeChannel >( normDesc, "vertexNormal", false );

    component->AddAttributeChannel( positions );
    component->AddAttributeChannel( normals );

    auto positionsVec = textAsset->CreateCharacter3D( character, layout.Size );
    auto numVerticies = positionsVec.size();
    positions->ReplaceAttributes( std::move( positionsVec ) );

    // Generate normals along z axis.
    auto & normalsVec = normals->GetVertices();
    normalsVec.resize( numVerticies, glm::vec3( 0.0f, 0.0f, 1.0f ) );

    return component;
}


void                                     Text3DUtils::ArrangeText                 ( const std::wstring& text, std::vector< ConnectedComponentPtr > & components, TextLayout layout )
{
    auto textAtlas = layout.TextAsset->GetAtlas();
    auto spacing = layout.Spacing;
    auto outlineSize = layout.OutlineSize;
    auto viewWidth = layout.ViewWidth;
    auto viewHeight = layout.ViewHeight;
    //auto blurSize = layout.BlurSize;
    auto useKerning = layout.UseKerning;

    assert( textAtlas );

    glm::vec3 translate(0.f);
    glm::vec3 translateDot(0.f);
    glm::vec3 interspace( spacing, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

    bool outline = false;

    if( outlineSize != 0 )
        outline = true;

    // Note: Atlas was created with specific font size. We must tak it into consideration while rescaling geometry.
    float fontRatio = layout.Size / layout.TextAsset->GetFontSize();
    float aspectRatio = float( std::min( viewWidth, viewHeight ) ) / 2.f;
    float scaleRatio = aspectRatio / fontRatio;

    //float blurTexSize = float( blurSize );
    //float blurLenghtX = float( blurSize ) / aspectRatio;
    //float blurLenghtY = float( blurSize ) / aspectRatio;


    float ccPaddingX = 1.f / aspectRatio;
    float ccPaddingY = 1.f / aspectRatio;

    // Space width should be get form : https://www.mail-archive.com/freetype@nongnu.org/msg01384.html
    auto spaceGlyphWidth    = (float)textAtlas->GetGlyph( L'0', outline )->width / scaleRatio + spacing;
    auto newLineShift       = -(float) 1.5f * textAtlas->GetGlyph( L'0', outline )->height / scaleRatio;

    unsigned int componentIdx = 0;
    for( unsigned int i = 0; i < text.size(); ++i )
    {
        auto wch = text[ i ];

        if( wch == L' ' )
        {
            translate += glm::vec3( spaceGlyphWidth, 0.f, 0.f ) + interspace;
            continue;
        }

        if( wch == L'\n' || wch == L'\r' )
        {
            translate = glm::vec3( 0.f );
            newLineTranslation += glm::vec3( 0.f, newLineShift , 0.f );
            continue;
        }


        if( auto glyph = textAtlas->GetGlyph( wch, outline ) )
        {
            auto kerningShift = glm::vec3( 0.f, 0.f, 0.f );

            if( useKerning && i > 0 )
            {
                auto kerShift = textAtlas->GetKerning( text[ i - 1 ], text[ i ] );
                kerningShift.x = kerShift / scaleRatio;
                translate += kerningShift;
            }

            glm::vec3 letterTranslate = translate + newLineTranslation - glm::vec3( ccPaddingX, ccPaddingY, 0.0 );

            auto attributeChannel = components[ componentIdx ]->GetAttributeChannels();
            auto posChannel = std::static_pointer_cast< Float3AttributeChannel >( attributeChannel[ 0 ] );
            assert( posChannel->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION );

            // Note: FreeType unit is 1/64th of a pixel.
            glm::vec3 scaleFactor( 1.0f / ( scaleRatio * 64.0f ), 1.0f / ( scaleRatio * 64.0f ), 1.0f );

            for( auto & pos : posChannel->GetVertices() )
            {
                pos = scaleFactor * pos + letterTranslate;
            }

            if( wch==L'.' && layout.Tat == TextAlignmentType::Dot )
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


}

} // model
} // bv
