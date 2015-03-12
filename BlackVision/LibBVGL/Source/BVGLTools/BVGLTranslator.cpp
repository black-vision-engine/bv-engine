#include "BVGLTranslator.h"

#include <cassert>


namespace bv {

// Hack - force map initialization
std::hash_map< GLenum, std::string > BVGLTranslator::ms_bufferUsage;
std::hash_map< GLenum, std::string > BVGLTranslator::ms_bufferTarget;

BVGLTranslator BVGLTranslator::ms_instance;

// ****************************
//
BVGLTranslator::BVGLTranslator  ()
{
    ms_bufferUsage[ (GLenum) GL_STATIC_DRAW ] = "GL_STATIC_DRAW";
    ms_bufferUsage[ (GLenum) GL_DYNAMIC_DRAW ] = "GL_DYNAMIC_DRAW";
    ms_bufferUsage[ (GLenum) GL_STREAM_READ  ] = "GL_STREAM_READ";
    ms_bufferUsage[ (GLenum) GL_STREAM_DRAW  ] = "GL_STREAM_DRAW";

    ms_bufferTarget[ (GLenum) GL_ARRAY_BUFFER ]         = "GL_ARRAY_BUFFER";
    ms_bufferTarget[ (GLenum) GL_PIXEL_PACK_BUFFER ]    = "GL_PIXEL_PACK_BUFFER";
    ms_bufferTarget[ (GLenum) GL_PIXEL_UNPACK_BUFFER  ] = "GL_PIXEL_UNPACK_BUFFER";

    ms_textureTarget[ (GLenum) GL_TEXTURE_2D ]  = "GL_TEXTURE_2D";
}

// ****************************
//
std::string    BVGLTranslator::TranslateBufferUsage     ( GLenum usage )
{
    assert( ms_bufferUsage.find( usage ) != ms_bufferUsage.end() );

    return ms_bufferUsage[ usage ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateBufferTarget   ( GLenum target )
{
    assert( ms_bufferTarget.find( target ) != ms_bufferTarget.end() );

    return ms_bufferTarget[ target ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateTextureTarget  ( GLenum target )
{
    assert( ms_textureTarget.find( target ) != ms_textureTarget.end() );

    return ms_textureTarget[ target ];
}

// ****************************
//
std::string     BVGLTranslator::TranslateTextureFormat  ( GLenum format )
{
    { format; }
    return "GL_BGRA";
}

} //bv
