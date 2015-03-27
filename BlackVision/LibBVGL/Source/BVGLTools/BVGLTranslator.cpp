#include "BVGLTranslator.h"

#include <cassert>


namespace bv {

// Hack - force map initialization
std::hash_map< GLenum, std::string > BVGLTranslator::ms_bufferUsage;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_bufferTarget;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_textureTarget;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_textureFormat;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_attribPointerType;

BVGLTranslator BVGLTranslator::ms_instance;

// ****************************
//
BVGLTranslator::BVGLTranslator                                      ()
{
    ms_bufferUsage[ (GLenum) GL_STATIC_DRAW ]	= "GL_STATIC_DRAW";
    ms_bufferUsage[ (GLenum) GL_DYNAMIC_DRAW ]	= "GL_DYNAMIC_DRAW";
    ms_bufferUsage[ (GLenum) GL_STREAM_READ  ]	= "GL_STREAM_READ";
    ms_bufferUsage[ (GLenum) GL_STREAM_DRAW  ]	= "GL_STREAM_DRAW";

    ms_bufferTarget[ (GLenum) GL_ARRAY_BUFFER ]         = "GL_ARRAY_BUFFER";
    ms_bufferTarget[ (GLenum) GL_PIXEL_PACK_BUFFER ]    = "GL_PIXEL_PACK_BUFFER";
    ms_bufferTarget[ (GLenum) GL_PIXEL_UNPACK_BUFFER ]	= "GL_PIXEL_UNPACK_BUFFER";

    ms_textureTarget[ (GLenum) GL_TEXTURE_2D ]  = "GL_TEXTURE_2D";

	ms_textureFormat[ (GLenum) GL_ALPHA ]		= "GL_ALPHA"; 
	ms_textureFormat[ (GLenum) GL_RGBA ]		= "GL_RGBA"; 
	ms_textureFormat[ (GLenum) GL_BGR ]			= "GL_BGR"; 
	ms_textureFormat[ (GLenum) GL_BGRA ]		= "GL_BGRA"; 
	ms_textureFormat[ (GLenum) GL_LUMINANCE ]	= "GL_LUMINANCE";
	ms_textureFormat[ (GLenum) GL_R32F ]		= "GL_R32F";

    ms_attribPointerType[ (GLenum) GL_FLOAT ]           = "GL_FLOAT";
    ms_attribPointerType[ (GLenum) GL_UNSIGNED_BYTE ]   = "GL_UNSIGNED_BYTE";
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
std::string     BVGLTranslator::TranslateFramebufferTarget          ( GLenum target )
{
    assert( target == GL_FRAMEBUFFER );

    return "GL_FRAMEBUFER";
}

// ****************************
//
std::string     BVGLTranslator::TranslateRenderbufferTarget         ( GLenum target )
{
    assert( target == GL_RENDERBUFFER );

    return "GL_RENDERBUFFER";
}

// ****************************
//
std::string     BVGLTranslator::TranslateRenderbufferInternalFormat ( GLenum format )
{
    //FIXME: add other formats
    assert( format == GL_DEPTH_COMPONENT );

    return "GL_DEPTH_COMPONENT";
}

// ****************************
//
std::string     BVGLTranslator::TranslateAttribPointerType          ( GLenum type )
{
    assert( ms_attribPointerType.find( type ) != ms_attribPointerType.end() );

    return ms_attribPointerType[ type ];
}

} //bv
