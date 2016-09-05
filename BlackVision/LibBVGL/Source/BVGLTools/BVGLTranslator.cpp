#include "BVGLTranslator.h"

#include <cassert>


namespace bv {

// Hack - force map initialization
std::hash_map< GLenum, std::string > BVGLTranslator::ms_bufferUsage;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_bufferTarget;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_textureTarget;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_textureFormat;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_textureInternalFormat;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_attribPointerType;

std::hash_map< GLenum, unsigned int > BVGLTranslator::ms_pixelSize;

BVGLTranslator BVGLTranslator::ms_instance;

// ****************************
//
BVGLTranslator::BVGLTranslator                                      ()
{
    ms_bufferUsage[ (GLenum) GL_STATIC_DRAW ]	= "GL_STATIC_DRAW";
    ms_bufferUsage[ (GLenum) GL_STATIC_READ ]	= "GL_STATIC_READ";
    ms_bufferUsage[ (GLenum) GL_DYNAMIC_DRAW ]	= "GL_DYNAMIC_DRAW";
    ms_bufferUsage[ (GLenum) GL_STREAM_READ  ]	= "GL_STREAM_READ";
    ms_bufferUsage[ (GLenum) GL_STREAM_DRAW  ]	= "GL_STREAM_DRAW";

    ms_bufferTarget[ (GLenum) GL_ARRAY_BUFFER ]						= "GL_ARRAY_BUFFER";
    ms_bufferTarget[ (GLenum) GL_ELEMENT_ARRAY_BUFFER ]				= "GL_ELEMENT_ARRAY_BUFFER";
    ms_bufferTarget[ (GLenum) GL_PIXEL_PACK_BUFFER ]				= "GL_PIXEL_PACK_BUFFER";
    ms_bufferTarget[ (GLenum) GL_PIXEL_UNPACK_BUFFER ]				= "GL_PIXEL_UNPACK_BUFFER";
    ms_bufferTarget[ (GLenum) GL_DRAW_INDIRECT_BUFFER ]				= "GL_DRAW_INDIRECT_BUFFER";
	ms_bufferTarget[ (GLenum) GL_UNIFORM_BUFFER ]					= "GL_UNIFORM_BUFFER";
	ms_bufferTarget[ (GLenum) GL_TRANSFORM_FEEDBACK_BUFFER ]		= "GL_TRANSFORM_FEEDBACK_BUFFER";
	ms_bufferTarget[ (GLenum) GL_SHADER_STORAGE_BUFFER ]			= "GL_SHADER_STORAGE_BUFFER";
	ms_bufferTarget[ (GLenum) GL_TEXTURE_BUFFER ]					= "GL_TEXTURE_BUFFER";
	ms_bufferTarget[ (GLenum) GL_COPY_READ_BUFFER ]					= "GL_COPY_READ_BUFFER";
	ms_bufferTarget[ (GLenum) GL_COPY_WRITE_BUFFER ]				= "GL_COPY_WRITE_BUFFER";
	ms_bufferTarget[ (GLenum) GL_ATOMIC_COUNTER_BUFFER ]			= "GL_ATOMIC_COUNTER_BUFFER";

    ms_textureTarget[ (GLenum) GL_TEXTURE_2D ]						= "GL_TEXTURE_2D";
	ms_textureTarget[ (GLenum) GL_TEXTURE_1D ]						= "GL_TEXTURE_1D";
	ms_textureTarget[ (GLenum) GL_TEXTURE_2D ]						= "GL_TEXTURE_2D";
	ms_textureTarget[ (GLenum) GL_TEXTURE_3D ]						= "GL_TEXTURE_3D";
	ms_textureTarget[ (GLenum) GL_TEXTURE_1D_ARRAY ]				= "GL_TEXTURE_1D_ARRAY";
	ms_textureTarget[ (GLenum) GL_TEXTURE_2D_ARRAY ]				= "GL_TEXTURE_2D_ARRAY";
	ms_textureTarget[ (GLenum) GL_TEXTURE_CUBE_MAP ]				= "GL_TEXTURE_CUBE_MAP";
	ms_textureTarget[ (GLenum) GL_TEXTURE_2D_MULTISAMPLE ]			= "GL_TEXTURE_2D_MULTISAMPLE";
	ms_textureTarget[ (GLenum) GL_TEXTURE_2D_MULTISAMPLE_ARRAY ]	= "GL_TEXTURE_2D_MULTISAMPLE_ARRAY";
	ms_textureTarget[ (GLenum) GL_TEXTURE_BUFFER ]					= "GL_TEXTURE_BUFFER";
	ms_textureTarget[ (GLenum) GL_TEXTURE_CUBE_MAP_ARRAY ]			= "GL_TEXTURE_CUBE_MAP_ARRAY";

	ms_textureFormat[ (GLenum) GL_ALPHA ]		= "GL_ALPHA"; 
	ms_textureFormat[ (GLenum) GL_RGBA ]		= "GL_RGBA"; 
	ms_textureFormat[ (GLenum) GL_BGR ]			= "GL_BGR"; 
	ms_textureFormat[ (GLenum) GL_BGRA ]		= "GL_BGRA"; 
	ms_textureFormat[ (GLenum) GL_LUMINANCE ]	= "GL_LUMINANCE";
	ms_textureFormat[ (GLenum) GL_R32F ]		= "GL_R32F";

	ms_textureInternalFormat[ (GLenum) GL_ALPHA8 ]		= "GL_ALPHA8"; 
	ms_textureInternalFormat[ (GLenum) GL_LUMINANCE8 ]	= "GL_LUMINANCE8"; 
	ms_textureInternalFormat[ (GLenum) GL_RGB8 ]		= "GL_RGB8"; 
	ms_textureInternalFormat[ (GLenum) GL_RGBA8 ]		= "GL_RGBA8"; 
	ms_textureInternalFormat[ (GLenum) GL_RGB32F ]		= "GL_RGB32F";
	ms_textureInternalFormat[ (GLenum) GL_RGBA32F ]		= "GL_RGBA32F";

    ms_attribPointerType[ (GLenum) GL_FLOAT ]           = "GL_FLOAT";
    ms_attribPointerType[ (GLenum) GL_UNSIGNED_BYTE ]   = "GL_UNSIGNED_BYTE";
    ms_attribPointerType[ (GLenum) GL_UNSIGNED_SHORT ]	= "GL_UNSIGNED_SHORT";
    ms_attribPointerType[ (GLenum) GL_UNSIGNED_INT ]	= "GL_UNSIGNED_INT";

	ms_pixelSize[ (GLenum) GL_ALPHA8 ]		=	1;
	ms_pixelSize[ (GLenum) GL_LUMINANCE8 ]	=	1;
	ms_pixelSize[ (GLenum) GL_RGB8 ]		=	3;
	ms_pixelSize[ (GLenum) GL_RGBA8 ]		=	4;
	ms_pixelSize[ (GLenum) GL_RGB32F ]		=	12;
	ms_pixelSize[ (GLenum) GL_RGBA32F ]		=	16;

}

// ****************************
//
std::string    BVGLTranslator::TranslateBufferUsage                 ( GLenum usage )
{
    assert( ms_bufferUsage.find( usage ) != ms_bufferUsage.end() );

    return ms_bufferUsage[ usage ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateBufferTarget               ( GLenum target )
{
    assert( ms_bufferTarget.find( target ) != ms_bufferTarget.end() );

    return ms_bufferTarget[ target ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateTextureTarget              ( GLenum target )
{
    assert( ms_textureTarget.find( target ) != ms_textureTarget.end() );

    return ms_textureTarget[ target ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateTextureFormat              ( GLenum format )
{
	assert( ms_textureFormat.find( format ) != ms_textureFormat.end() );

    return ms_textureFormat[ format ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateTextureInternalFormat      ( GLenum format )
{
    assert( ms_textureInternalFormat.count( format ) != 0 );

    return ms_textureInternalFormat[ format ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateFramebufferTarget          ( GLenum target )
{
    { target; }

    assert( target == GL_FRAMEBUFFER );

    return "GL_FRAMEBUFER";
}

// ****************************
//
std::string     BVGLTranslator::TranslateRenderbufferTarget         ( GLenum target )
{
    { target; }

    assert( target == GL_RENDERBUFFER );

    return "GL_RENDERBUFFER";
}

// ****************************
//
std::string     BVGLTranslator::TranslateRenderbufferInternalFormat ( GLenum format )
{
    { format; }
    //FIXME: add other formats
	assert( format == GL_DEPTH_COMPONENT || format == GL_RGBA );

	switch( format )
	{
	case GL_DEPTH_COMPONENT:
		return "GL_DEPTH_COMPONENT";
	case GL_RGBA:
		return "GL_RGBA";
	default:
		assert( false ); // should never be here
		return "";
	}
}

// ****************************
//
std::string     BVGLTranslator::TranslateAttribPointerType          ( GLenum type )
{
    assert( ms_attribPointerType.find( type ) != ms_attribPointerType.end() );

    return ms_attribPointerType[ type ];
}

// ****************************
//
unsigned int	BVGLTranslator::GetPixelSize						( GLenum format )
{
    assert( ms_pixelSize.count( format ) != 0 );

    return ms_pixelSize[ format ];
}

// ****************************
//
std::string     BVGLTranslator::RemoveGLPrefix                      ( const std::string & str )
{
    return str.substr( 3, std::string::npos );
}

} //bv
