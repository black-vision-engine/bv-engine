#include "BVGLResourceTrackingPlugin.h"

#include <sstream>
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

    m_buffers.GenResources( n, buffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteBuffers               ( GLsizei n, const GLuint * buffers )
{
    Parent::DeleteBuffers( n, buffers );

    m_buffers.DeleteResources( n, buffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BindBuffer                  ( GLenum target, GLuint buffer )
{
    Parent::BindBuffer( target, buffer );

    m_buffers.BindResource( target, buffer );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BufferData                  ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    Parent::BufferData( target, size, data, usage );

    m_buffers.GetBoundResource( target ).Set( size, usage, data );

    PrintCompleteSummary( "BufferData() called" );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::GenTextures                 ( GLsizei n, GLuint * textures )
{
    Parent::GenTextures( n, textures );

    m_textures.GenResources( n, textures );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteTextures              ( GLsizei n, const GLuint * textures )
{
    Parent::DeleteTextures( n, textures );

    m_textures.DeleteResources( n, textures );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::TexImage2D					( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels )
{
    Parent::TexImage2D( target, level, internalformat, width, height, border, format, type, pixels );

    m_textures.GetBoundResource( target ).Set( width, height, format, pixels );

    PrintCompleteSummary( "TexImage2D() called" );
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

    m_textures.BindResource( target, texture );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::GenRenderbuffers            ( GLsizei n, GLuint * renderbuffers )
{
    Parent::GenRenderbuffers( n, renderbuffers );

    m_renderbuffers.GenResources( n, renderbuffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteRenderbuffers         ( GLsizei n, const GLuint * renderbuffers )
{
    Parent::DeleteRenderbuffers( n, renderbuffers );

    m_renderbuffers.DeleteResources( n, renderbuffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BindRenderbuffer            ( GLenum target, GLuint renderbuffer )
{
    Parent::BindRenderbuffer( target, renderbuffer );

    m_renderbuffers.BindResource( target, renderbuffer );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::RenderbufferStorage         ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
{
    Parent::RenderbufferStorage( target, internalformat, width, height );

    m_renderbuffers.GetBoundResource( target ).Set( internalformat, width, height );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::GenFramebuffers             ( GLsizei n, GLuint * framebuffers )
{
    Parent::GenFramebuffers( n, framebuffers );

    m_framebuffers.GenResources( n, framebuffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteFramebuffers          ( GLsizei n, const GLuint * framebuffers )
{
    Parent::DeleteFramebuffers( n, framebuffers );

    m_framebuffers.DeleteResources( n, framebuffers );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BindFramebuffer             ( GLenum target, GLuint framebuffer )
{
    Parent::BindFramebuffer( target, framebuffer );

    m_framebuffers.BindResource( target, framebuffer );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::FramebufferTexture2D		( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
    Parent::FramebufferTexture2D( target, attachment, textarget, texture, level );

    m_framebuffers.GetBoundResource( target ).AttachTexture2D( attachment, target, texture, level );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::FramebufferRenderbuffer		( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
    Parent::FramebufferRenderbuffer( target, attachment, renderbuffertarget, renderbuffer );

    m_framebuffers.GetBoundResource( target ).AttachRenderbuffer( attachment, renderbuffertarget, renderbuffer );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::GenVertexArrays             ( GLsizei n, GLuint * arrays )
{
    Parent::GenVertexArrays( n, arrays );

    m_vertexarrays.GenResources( n, arrays );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DeleteVertexArrays          ( GLsizei n, const GLuint * arrays )
{
    Parent::DeleteVertexArrays( n, arrays );

    m_vertexarrays.DeleteResources( n, arrays );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::VertexAttribPointer         ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    Parent::VertexAttribPointer( index, size, type, normalized, stride, pointer );

    auto bufferID = m_buffers.GetBoundResourceID( GL_ARRAY_BUFFER );

    m_vertexarrays.GetBoundResource( 1 ).SetAttrPointer( bufferID, index, size, type, normalized, stride, pointer );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::BindVertexArray				( GLuint array )
{
    Parent::BindVertexArray( array );

    m_vertexarrays.BindResource( 1, array );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::EnableVertexAttribArray		( GLuint index )
{
    Parent::EnableVertexAttribArray( index );
    
    m_vertexarrays.GetBoundResource( 1 ).Enable( index );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::DisableVertexAttribArray	( GLuint index )
{
    Parent::DisableVertexAttribArray( index );

    m_vertexarrays.GetBoundResource( 1 ).Disable( index );
}

// *****************************
//
void    BVGLResourceTrackingPlugin::PrintShortSummary           ( const std::string & message )
{
    if( message != "" )
        printf( "%s\n", message.c_str() );

    printf( "%s\n", m_buffers.ShortSummary("\nBUFFERS:").c_str() );
    printf( "%s\n", m_textures.ShortSummary("\nTEXTURES:").c_str() );
    printf( "%s\n", m_framebuffers.ShortSummary("\nFRAME BUFERS:").c_str() );
    printf( "%s\n", m_renderbuffers.ShortSummary("\nRENDER BUFFERS:").c_str() );
    printf( "%s\n", m_vertexarrays.ShortSummary("\nVERTEX ARRAYS:").c_str() );
}

void    BVGLResourceTrackingPlugin::PrintCompleteSummary        ( const std::string & message )
{
    if( message != "" )
        printf( "%s\n", message.c_str() );

    printf( "%s\n", m_buffers.CompleteSummary("\nBUFFERS:").c_str() );
    printf( "%s\n", m_textures.CompleteSummary("\nTEXTURES:").c_str() );
    printf( "%s\n", m_framebuffers.CompleteSummary("\nFRAME BUFERS:").c_str() );
    printf( "%s\n", m_renderbuffers.CompleteSummary("\nRENDER BUFFERS:").c_str() );
    printf( "%s\n", m_vertexarrays.CompleteSummary("\nVERTEX ARRAYS:").c_str() );
}

} //bv
