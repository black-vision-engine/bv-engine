#include "BVGLResourceTrackingPlugin.h"

#include "BVGLTools/BVGLTranslator.h"

#include <cassert>
#include <cstdio>


namespace bv {

BVGLResourceTrackingPlugin  BVGLResourceTrackingPlugin::bvgl;

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
void    BVGLResourceTrackingPlugin::bvglGenBuffers              ( GLsizei n, GLuint * buffers )
{
    bvgl.GenBuffers( n, buffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::bvglDeleteBuffers           ( GLsizei n, const GLuint * buffers )
{
    bvgl.DeleteBuffers( n, buffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::bvglBindBuffer              ( GLenum target, GLuint buffer )
{
    bvgl.BindBuffer( target, buffer );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::bvglBufferData              ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    bvgl.BufferData( target, size, data, usage );
}
 
// *****************************
//
void    BVGLResourceTrackingPlugin::GenBuffers                  ( GLsizei n, GLuint * buffers )
{
    Parent::bvglGenBuffers( n, buffers );

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
    Parent::bvglDeleteBuffers( n, buffers );

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
    Parent::bvglBindBuffer( target, buffer );

    m_boundBuffers[ target ] = buffer;
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BufferData                  ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    Parent::bvglBufferData( target, size, data, usage );

    assert( m_boundBuffers.find( target ) != m_boundBuffers.end() );

    auto buffer = m_boundBuffers[ target ];

    m_allocatedBuffers[ buffer ].size = size;
    m_allocatedBuffers[ buffer ].usage = usage;
    m_allocatedBuffers[ buffer ].data = data;

    PrintStats();
}

// *****************************
//
void    BVGLResourceTrackingPlugin::PrintStats                  ()
{
    bvgl.PrintBuffersStats();    
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

        printf( "Buf: %2d, Size: %4d %2s, Usage: %d, DataPTR: %08X", bufId, siz, suffix.c_str(), BVGLTranslator::TranslateBufferUsage( bufDesc.usage ), bufDesc.data );

        if( target != 0 )
        {
            printf( " BOUND to: %5d", target );
        }

        printf( "\n" );
    }
}

} //bv
