#include "BVGLTranslator.h"

#include <cassert>


namespace bv {

// Hack - force map initialization
BVGLTranslator BVGLTranslator::ms_instance;

std::hash_map< GLenum, std::string > BVGLTranslator::ms_bufferUsage;

// ****************************
//
BVGLTranslator::BVGLTranslator  ()
{

    // Initialize USAGE translation data
    std::string val = "GL_STATIC_DRAW";
    ms_bufferUsage.insert( std::pair< GLenum, std::string >( GL_STATIC_DRAW, val ) );
    ms_bufferUsage[ (GLenum) GL_DYNAMIC_DRAW ] = "GL_DYNAMIC_DRAW";
    ms_bufferUsage[ (GLenum) GL_STREAM_READ  ] = "GL_STREAM_READ";
    ms_bufferUsage[ (GLenum) GL_STREAM_DRAW  ] = "GL_STREAM_DRAW";
}

// ****************************
//
std::string    BVGLTranslator::TranslateBufferUsage( GLenum usage )
{
    assert( ms_bufferUsage.find( usage ) != ms_bufferUsage.end() );

    return ms_bufferUsage[ usage ];
}

} //bv
