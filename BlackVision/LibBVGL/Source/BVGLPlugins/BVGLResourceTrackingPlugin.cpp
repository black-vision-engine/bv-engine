#include "BVGLResourceTrackingPlugin.h"

#include "BVGLTools/BVGLTranslator.h"

#include <cassert>
#include <cstdio>


namespace bv {

// *****************************
//
BVGLResourceTrackingPlugin::BVGLResourceTrackingPlugin  ()
{
}

// *****************************
//
BVGLResourceTrackingPlugin::~BVGLResourceTrackingPlugin ()
{
}
 
// *****************************
//
void    BVGLResourceTrackingPlugin::GenBuffers                  ( GLsizei n, GLuint * buffers )
{
    Parent::GenBuffers( n, buffers );

    for( auto i = 0; i < n; ++i )
    {
        auto buffer = buffers[ i ];

        m_allocatedBuffers[ buffer ] = BufferDesc();
    }
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteBuffers               ( GLsizei n, const GLuint * buffers )
{
    Parent::DeleteBuffers( n, buffers );

    for( auto i = 0; i < n; ++i )
    {
        auto buffer = buffers[ i ];

        m_allocatedBuffers.erase( buffer );
    }    
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BindBuffer                  ( GLenum target, GLuint buffer )
{
    Parent::BindBuffer( target, buffer );

    m_boundBuffers[ target ] = buffer;
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BufferData                  ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    Parent::BufferData( target, size, data, usage );

    assert( m_boundBuffers.find( target ) != m_boundBuffers.end() );

    auto buffer = m_boundBuffers[ target ];

    m_allocatedBuffers[ buffer ].size = size;
    m_allocatedBuffers[ buffer ].usage = usage;
    m_allocatedBuffers[ buffer ].data = data;

    PrintStats();
}

// *****************************
//
void    BVGLResourceTrackingPlugin::GenTextures                 ( GLsizei n, GLuint * textures )
{
    Parent::GenTextures( n, textures );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteTextures              ( GLsizei n, const GLuint * textures )
{
    Parent::DeleteTextures( n, textures );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::TexImage2D					( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels )
{
    Parent::TexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::ActiveTexture				( GLenum texture )
{
    Parent::ActiveTexture( texture );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::TexSubImage2D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels )
{
    Parent::TexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BindTexture					( GLenum target, GLuint texture )
{
    Parent::BindTexture( target, texture );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::GenRenderbuffers            ( GLsizei n, GLuint * renderbuffers )
{
    Parent::GenRenderbuffers( n, renderbuffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteRenderbuffers         ( GLsizei n, const GLuint * renderbuffers )
{
    Parent::DeleteRenderbuffers( n, renderbuffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::PrintStats                  ()
{
    PrintBuffersStats();    
}

// *****************************
//
void    BVGLResourceTrackingPlugin::PrintBuffersStats           ()
{
    printf( "BUFFERS:\n");

    for( auto buf : m_allocatedBuffers )
    {
        auto bufId      = buf.first;
        auto bufDesc    = buf.second;

        GLenum target   = 0;
    
        for( auto bb : m_boundBuffers )
        {
            if( bb.second == bufId )
            {
                target = bb.first;
                break;
            }
        }

        auto siz = bufDesc.size;
        std::string suffix = "B";

        if( siz > 1024 * 256 )
        {
            siz /= 1024;
            suffix = "kB";
        }

        if( siz > 1024 * 256 )
        {
            siz /= 1024;
            suffix = "MB";
        }

        printf( "Buf: %2d, Size: %4d %2s, Usage: %15s", bufId, siz, suffix.c_str(), BVGLTranslator::TranslateBufferUsage( bufDesc.usage ).c_str() );

        if( target != 0 )
        {
            printf( " BND: %s", BVGLTranslator::TranslateBufferTarget( target ).c_str() );
        }

        printf( "\n" );
    }
}

} //bv
