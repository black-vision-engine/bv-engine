#include "BVGLResourceTrackingPlugin.h"

#include "BVGLTools/BVGLTranslator.h"

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
    //FIXME: implement tracking
}

// *****************************
//
void    BVGLResourceTrackingPlugin::PrintShortSummary           ( const std::string & message )
{
    printf( "%s\n", message.c_str() );
}

void    BVGLResourceTrackingPlugin::PrintCompleteSummary        ( const std::string & message )
{
    printf( "%s\n", message.c_str() );

    PrintBuffersStats();
    printf("\n");
	PrintTextureStats();
    printf("\n");
    PrintRenderbuffersStats();
    printf("\n");
    PrintFramebufersStats();
}

// *****************************
//
void    BVGLResourceTrackingPlugin::PrintBuffersStats           ()
{
    printf( "BUFFERS:\n");

    //for( auto buf : m_allocatedBuffers )
    //{
    //    auto bufId      = buf.first;
    //    auto bufDesc    = buf.second;

    //    GLenum target   = 0;
    //
    //    for( auto bb : m_boundBuffers )
    //    {
    //        if( bb.second == bufId )
    //        {
    //            target = bb.first;
    //            break;
    //        }
    //    }

    //    auto siz = FormatSize( (GLuint) bufDesc.size );

    //    printf( "Buf: %2d, Size: %7s, Usage: %15s", bufId, siz.c_str(), BVGLTranslator::TranslateBufferUsage( bufDesc.usage ).c_str() );

    //    if( target != 0 )
    //    {
    //        printf( " BND: %s", BVGLTranslator::TranslateBufferTarget( target ).c_str() );
    //    }

    //    printf( "\n" );
    //}
}

// *****************************
//
void        BVGLResourceTrackingPlugin::PrintTextureStats           ()
{
    printf( "TEXTURES:\n");

    //for( auto tex : m_allocatedTextures )
    //{
    //    auto texId      = tex.first;
    //    auto texDesc    = tex.second;

    //    GLenum target   = 0;
    //
    //    for( auto bt : m_boundTextures )
    //    {
    //        if( bt.second == (GLint) texId )
    //        {
    //            target = bt.first;
    //            break;
    //        }
    //    }

    //    auto siz = FormatSize( (GLuint) texDesc.DataSize() );
    //    printf( "Tex: %2d (%4d, %4d), Size: %7s, Format: %10s", texId, texDesc.width, texDesc.height, siz.c_str(), BVGLTranslator::TranslateTextureFormat( texDesc.format ).c_str() );

    //    if( target != 0 )
    //    {
    //        printf( " BND: %s", BVGLTranslator::TranslateTextureTarget( target ).c_str() );
    //    }

    //    printf( "\n" );
    //}
}

// *****************************
//
void                BVGLResourceTrackingPlugin::PrintRenderbuffersStats     ()
{
    printf( "RENDERBUFFERS:\n");
    
    //for( auto buf : m_allocatedFramebuffers )
    //{
    //    auto bufId      = buf.first;
    //    auto bufDesc    = buf.second;

    //    GLenum target   = 0;
    //
    //    for( auto bb : m_boundBuffers )
    //    {
    //        if( bb.second == bufId )
    //        {
    //            target = bb.first;
    //            break;
    //        }
    //    }

    //    auto siz = FormatSize( (GLuint) bufDesc.size );

    //    printf( "Buf: %2d, Size: %7s, Usage: %15s", bufId, siz.c_str(), BVGLTranslator::TranslateBufferUsage( bufDesc.usage ).c_str() );

    //    if( target != 0 )
    //    {
    //        printf( " BND: %s", BVGLTranslator::TranslateBufferTarget( target ).c_str() );
    //    }

    //    printf( "\n" );
    //}
}

// *****************************
//
void                BVGLResourceTrackingPlugin::PrintFramebufersStats       ()
{
    printf( "FRAMEBUFFERS:\n");
}

// *****************************
//
std::string         BVGLResourceTrackingPlugin::FormatSize          ( GLuint numBytes )
{
    auto siz = numBytes;
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
    
    if( siz > 1024 * 256 )
    {
        siz /= 1024;
        suffix = "GB";
    }

    std::stringstream ss;
    
    ss << siz << " " << suffix;

    return ss.str();
}


} //bv
