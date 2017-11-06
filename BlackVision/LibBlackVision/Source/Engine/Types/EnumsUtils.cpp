#include "stdafx.h"

#include "EnumsUtils.h"
#include "Serialization/SerializationHelper.inl"


#include "Memory/MemoryLeaks.h"



namespace bv { 

namespace 
{

TextureFormat TextureFormatBV[] =
{
	TextureFormat::F_A32FR32FG32FB32F	,	// IF_A32FR32FG32FB32F
	TextureFormat::F_R32FG32FB32F		,	// IF_R32FG32FB32F
	TextureFormat::F_A32F				,	// IF_A32F
	TextureFormat::F_A8R8G8B8			,	// IF_A8R8G8B8
	TextureFormat::F_R8G8B8				,	// IF_R8G8B8
	TextureFormat::F_A8					,	// IF_A8
	TextureFormat::F_L8					,	// IF_L8
};

} // anonymous

TextureFormat	EnumsUtils::Convert	( image::ImageFormat ifmt )
{
	return TextureFormatBV[ ( int )ifmt ];
}



namespace Convert
{


std::pair< bv::PrimitiveType, const char* > PrimitiveTypeMapping[] = {
    std::make_pair( bv::PrimitiveType::PT_LINES, "Lines" ),
    std::make_pair( bv::PrimitiveType::PT_LINE_LOOP, "LineLoop" ),
    std::make_pair( bv::PrimitiveType::PT_LINE_STRIP, "LineStrip" ),
    std::make_pair( bv::PrimitiveType::PT_POINTS, "Points" ),
    std::make_pair( bv::PrimitiveType::PT_POLYGON, "Polygons" ),
    std::make_pair( bv::PrimitiveType::PT_QUADS, "Quads" ),
    std::make_pair( bv::PrimitiveType::PT_QUAD_MESH, "QuadMesh" ),
    std::make_pair( bv::PrimitiveType::PT_QUAD_STRIP, "QuadStrip" ),
    std::make_pair( bv::PrimitiveType::PT_TRIANGLES, "Triangles" ),
    std::make_pair( bv::PrimitiveType::PT_TRIANGLE_FAN, "TriangleFan" ),
    std::make_pair( bv::PrimitiveType::PT_TRIANGLE_MESH, "TriangleMesh" ),
    std::make_pair( bv::PrimitiveType::PT_TRIANGLE_STRIP, "TriangleStrip" ),
    std::make_pair( bv::PrimitiveType::PT_TOTAL, "" ),
};

IMPLEMENT_ENUM_SERIALIZATION( PrimitiveType, PrimitiveTypeMapping );


}   // Convert
}   // bv
