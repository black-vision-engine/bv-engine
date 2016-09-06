#include "stdafx.h"

#include "Text3dUtils.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include <FreeType/ftglyph.h>
#include "FTGL/ftgl.h"
#include "Memory/MemoryLeaks.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"



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
std::vector< ConnectedComponentPtr >     Text3DUtils::CreateText                  ( const std::wstring& text, TextConstPtr textAsset )
{
    std::vector< ConnectedComponentPtr > letters;

    for( int l = 0; l < text.size(); ++l )
    {
        ConnectedComponentPtr component = nullptr;

        // Find previous use of this letter.
        for( int i = 0; i < l; ++i )
        {
            if( text[ i ] == text[ l ] )
            {
                component = letters[ i ];
                break;
            }
        }

        // Component not found in previously used letters. We must crate it.
        if( component == nullptr )
        {
            component = CreateLetter( text[ l ], textAsset );
            assert( component );
        }

        letters.push_back( component );
    }

    return letters;
}

// ***********************
//
ConnectedComponentPtr                    Text3DUtils::CreateLetter                ( const wchar_t character, TextConstPtr& textAsset )
{
    ConnectedComponentPtr       component = ConnectedComponent::Create();

    model::AttributeChannelDescriptor * posDesc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    model::AttributeChannelDescriptor * normDesc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR );

    Float3AttributeChannelPtr   positions = std::make_shared< Float3AttributeChannel >( posDesc, "vertexPosition", false );
    Float3AttributeChannelPtr   normals = std::make_shared< Float3AttributeChannel >( normDesc, "vertexNormal", false );

    component->AddAttributeChannel( positions );
    component->AddAttributeChannel( normals );

    auto positionsVec = textAsset->CreateCharacter3D( character );
    positions->ReplaceAttributes( std::move( positionsVec ) );

    // Generate normals along z axis.
    auto & normalsVec = normals->GetVertices();
    normalsVec.resize( positionsVec.size(), glm::vec3( 0.0f, 0.0f, -1.0f ) );

    return component;
}

} // model
} // bv
