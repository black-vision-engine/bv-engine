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

	m_textures.GetBoundResource( target ).Set( width, height, 0, format, pixels );

    PrintCompleteSummary( "TexImage2D() called" );
}

// *****************************
// It's texture unit not texture realy.
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
// @todo It should take into account binding to multiple texture units.
void    BVGLResourceTrackingPlugin::BindTexture					( GLenum target, GLuint texture )
{
    Parent::BindTexture( target, texture );

    m_textures.BindResource( target, texture );
	if( texture != 0 )		// Set format only for real texture
		m_textures.GetBoundResource( target ).SetTypeIfFirstBind( target );
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

#ifdef GL_VERSION_4_4
// *****************************
// @todo This should be implemented when binding to multiple texture units will be done.
void	BVGLResourceTrackingPlugin::BindTextures				( GLuint first, GLsizei count, const GLuint* textures )
{
	Parent::BindTextures( first, count, textures );
}
#endif
#ifdef GL_VERSION_4_5
// *****************************
// @todo This should be implemented when binding to multiple texture units will be done.
void	BVGLResourceTrackingPlugin::BindTextureUnit				( GLuint unit, GLuint texture )
{
	Parent::BindTextureUnit( unit, texture );
}
#endif


void BVGLResourceTrackingPlugin::CompressedTexImage1D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexImage1D( target, level, internalFormat, width, border, imageSize, data );
	
	m_textures.GetBoundResource( target ).Set( width, 0, 0, internalFormat, data );
    PrintCompleteSummary( "CompressedTexImage1D() called" );
}

void BVGLResourceTrackingPlugin::CompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexImage2D( target, level, internalFormat, width, height, border, imageSize, data );

	m_textures.GetBoundResource( target ).Set( width, height, 0, internalFormat, data );
    PrintCompleteSummary( "CompressedTexImage2D() called" );
}

void BVGLResourceTrackingPlugin::CompressedTexImage3D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexImage3D( target, level, internalFormat, width, height, depth, border, imageSize, data );

	// @todo Textures can't be 3-dimmensional in curretn implementation.
	m_textures.GetBoundResource( target ).Set( width, height, depth, internalFormat, data );
    PrintCompleteSummary( "CompressedTexImage3D() called" );
}

void BVGLResourceTrackingPlugin::CompressedTexSubImage1D		( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexSubImage1D( target, level, xoffset, width, format, imageSize, data );
	// @todo TexSubImage2D calls only parent implementation too.Maybe smth should be done here.
}

void BVGLResourceTrackingPlugin::CompressedTexSubImage2D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexSubImage2D( target, level, xoffset, yoffset, width, height, format, imageSize, data );
	// @todo TexSubImage2D calls only parent implementation too.Maybe smth should be done here.
}

void BVGLResourceTrackingPlugin::CompressedTexSubImage3D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data );
	// @todo TexSubImage2D calls only parent implementation too.Maybe smth should be done here.
}

void BVGLResourceTrackingPlugin::TexImage1D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	Parent::TexImage1D( target, level, internalFormat, width, border, format, type, data );
	
	m_textures.GetBoundResource( target ).Set( width, 0, 0, format, data );
    PrintCompleteSummary( "TexImage1D() called" );
}

void BVGLResourceTrackingPlugin::TexImage3D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	Parent::TexImage3D( target, level, internalFormat, width, height, depth, border, format, type, data );
		
	// @todo Textures can't be 3-dimmensional in curretn implementation.
	m_textures.GetBoundResource( target ).Set( width, height, 0, format, data );
    PrintCompleteSummary( "TexImage3D() called" );
}

/**GL4.3*/
void BVGLResourceTrackingPlugin::TexImage2DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	Parent::TexImage2DMultisample( target, samples, internalFormat, width, height, fixedSampleLocations );
}
/**GL4.3*/
void BVGLResourceTrackingPlugin::TexImage3DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	Parent::TexImage3DMultisample( target, samples, internalFormat, width, height, depth, fixedSampleLocations );
}

void BVGLResourceTrackingPlugin::TexSubImage1D				( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels )
{
	Parent::TexSubImage1D( target, level, xoffset, width, format, type, pixels );
	// @todo TexSubImage2D calls only parent implementation too.Maybe smth should be done here.
}

void BVGLResourceTrackingPlugin::TexSubImage3D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels )
{
	Parent::TexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels );
	// @todo TexSubImage2D calls only parent implementation too.Maybe smth should be done here.
}

void BVGLResourceTrackingPlugin::TexBuffer					( GLenum target, GLenum /*internalFormat*/, GLuint buffer )
{
	m_buffers.BindResource( target, buffer );
}

void BVGLResourceTrackingPlugin::TexBufferRange				( GLenum target, GLenum /*internalFormat*/, GLuint buffer, GLintptr /*offset*/, GLsizeiptr /*size*/ )
{
	m_buffers.BindResource( target, buffer );
}

void BVGLResourceTrackingPlugin::BindBufferBase				( GLenum target, GLuint /*index*/, GLuint buffer )
{
	m_buffers.BindResource( target, buffer );
}

void BVGLResourceTrackingPlugin::BindBufferRange				( GLenum target, GLuint /*index*/, GLuint buffer, GLintptr /*offset*/, GLsizeiptr /*size*/ )
{
	m_buffers.BindResource( target, buffer );
}

void BVGLResourceTrackingPlugin::FramebufferTexture			( GLenum target, GLenum attachment, GLuint texture, GLint level )
{
	Parent::FramebufferTexture( target, attachment, texture, level );
	m_framebuffers.GetBoundResource( target ).AttachTexture2D( attachment, target, texture, level );
}

void BVGLResourceTrackingPlugin::FramebufferTextureLayer		( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer )
{
	Parent::FramebufferTextureLayer( target, attachment, texture, level, layer );
	m_framebuffers.GetBoundResource( target ).AttachTexture2D( attachment, target, texture, level );
}

void BVGLResourceTrackingPlugin::RenderbufferStorageMultisample			( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height )
{
	Parent::RenderbufferStorageMultisample( target, samples, internalFormat, width, height );
	m_renderbuffers.GetBoundResource( target ).Set( internalFormat, width, height );
}

#ifdef GL_VERSION_4_5

void BVGLResourceTrackingPlugin::NamedFramebufferTexture		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level )
{
	Parent::NamedFramebufferTexture( framebuffer, attachment, texture, level );


}

void BVGLResourceTrackingPlugin::NamedFramebufferTextureLayer( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer )
{
	Parent::NamedFramebufferTextureLayer( framebuffer, attachment, texture, level, layer );

}

void BVGLResourceTrackingPlugin::NamedFramebufferRenderbuffer( GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
	Parent::NamedFramebufferRenderbuffer( framebuffer, attachment, renderbuffertarget, renderbuffer );

}

#endif

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
