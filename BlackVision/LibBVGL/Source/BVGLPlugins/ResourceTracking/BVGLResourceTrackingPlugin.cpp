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

#ifdef BV_GL_VERSION_4_4
// *****************************
//
void BVGLResourceTrackingPlugin::BufferStorage				( GLenum target, GLsizeiptr size, const GLvoid* data, GLbitfield flags )
{
	Parent::BufferStorage( target, size, data, flags );

	// BufferStorage function sets usage = GL_DYNAMIC_DRAW
	m_buffers.GetBoundResource( target ).Set( size, GL_DYNAMIC_DRAW, data );

    PrintCompleteSummary( "BufferStorage() called" );
}
#endif

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

	auto& texture = m_textures.GetBoundResource( target );

	if( level == 0 )		// We set size only for base texture. Otherwise textures would be 1x1.
		texture.Set( width, height, 0, internalformat, pixels );
	texture.NewMipmapLevel( level );

    PrintCompleteSummary( "TexImage2D() called" );
}

// *****************************
//
void BVGLResourceTrackingPlugin::TexImage1D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	Parent::TexImage1D( target, level, internalFormat, width, border, format, type, data );
			
	auto& texture = m_textures.GetBoundResource( target );
	
	if( level == 0 )		// We set size only for base texture. Otherwise textures would be 1x1.
		texture.Set( width, 0, 0, internalFormat, data );
	texture.NewMipmapLevel( level );

    PrintCompleteSummary( "TexImage1D() called" );
}

// *****************************
//
void BVGLResourceTrackingPlugin::TexImage3D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	Parent::TexImage3D( target, level, internalFormat, width, height, depth, border, format, type, data );
				
	auto& texture = m_textures.GetBoundResource( target );

	if( level == 0 )		// We set size only for base texture. Otherwise textures would be 1x1.
		texture.Set( width, height, depth, internalFormat, data );
	texture.NewMipmapLevel( level );

    PrintCompleteSummary( "TexImage3D() called" );
}

// *****************************
//**GL4.3*/
void BVGLResourceTrackingPlugin::TexImage2DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	Parent::TexImage2DMultisample( target, samples, internalFormat, width, height, fixedSampleLocations );
					
	auto& texture = m_textures.GetBoundResource( target );
	
	// Multisample texture can't have mipmaps
	texture.Set( width, height, 0, internalFormat, nullptr );
	texture.NewMipmapLevel( 0 );
}

// *****************************
//**GL4.3*/
void BVGLResourceTrackingPlugin::TexImage3DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	Parent::TexImage3DMultisample( target, samples, internalFormat, width, height, depth, fixedSampleLocations );
						
	auto& texture = m_textures.GetBoundResource( target );
	
	// Multisample texture can't have mipmaps
	texture.Set( width, height, depth, internalFormat, nullptr );
	texture.NewMipmapLevel( 0 );
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

#ifdef BV_GL_VERSION_4_4
// *****************************
// @todo This should be implemented when binding to multiple texture units will be done.
void	BVGLResourceTrackingPlugin::BindTextures				( GLuint first, GLsizei count, const GLuint* textures )
{
	Parent::BindTextures( first, count, textures );
}
#endif
#ifdef BV_GL_VERSION_4_5
// *****************************
// @todo This should be implemented when binding to multiple texture units will be done.
void	BVGLResourceTrackingPlugin::BindTextureUnit				( GLuint unit, GLuint texture )
{
	Parent::BindTextureUnit( unit, texture );
}

// *****************************
//
void BVGLResourceTrackingPlugin::TextureStorage1D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width )
{
	Parent::TextureStorage1D( texture, levels, internalFormat, width );
		
	auto& tex = m_textures.GetResource( texture );

	tex.Set( width, 0, 0, internalFormat, nullptr );
	tex.NewMipmapLevel( levels - 1 );	// Base level is counted in.
}

// *****************************
//
void BVGLResourceTrackingPlugin::TextureStorage2D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height )
{
	Parent::TextureStorage2D( texture, levels, internalFormat, width, height );
		
	auto& tex = m_textures.GetResource( texture );

	tex.Set( width, height, 0, internalFormat, nullptr );
	tex.NewMipmapLevel( levels - 1 );	// Base level is counted in.
}

// *****************************
//
void BVGLResourceTrackingPlugin::TextureStorage3D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth )
{
	Parent::TextureStorage3D( texture, levels, internalFormat, width, height, depth );
		
	auto& tex = m_textures.GetResource( texture );

	tex.Set( width, height, depth, internalFormat, nullptr );
	tex.NewMipmapLevel( levels - 1 );	// Base level is counted in.
}

// *****************************
//
void BVGLResourceTrackingPlugin::TextureStorage2DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	Parent::TextureStorage2DMultisample( texture, samples, internalFormat, width, height, fixedSampleLocations );
		
	auto& tex = m_textures.GetResource( texture );

	tex.Set( width, height, 0, internalFormat, nullptr );
}

// *****************************
//
void BVGLResourceTrackingPlugin::TextureStorage3DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	Parent::TextureStorage3DMultisample( texture, samples, internalFormat, width, height, depth, fixedSampleLocations );
		
	auto& tex = m_textures.GetResource( texture );

	tex.Set( width, height, depth, internalFormat, nullptr );
}

#endif


void BVGLResourceTrackingPlugin::CompressedTexImage1D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexImage1D( target, level, internalFormat, width, border, imageSize, data );
	
	auto& texture = m_textures.GetBoundResource( target );

	if( level == 0 )		// We set size only for base texture. Otherwise textures would be 1x1.
		texture.Set( width, 0, 0, internalFormat, data );
	texture.NewMipmapLevel( level );

    PrintCompleteSummary( "CompressedTexImage1D() called" );
}

void BVGLResourceTrackingPlugin::CompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexImage2D( target, level, internalFormat, width, height, border, imageSize, data );
	
	auto& texture = m_textures.GetBoundResource( target );

	if( level == 0 )		// We set size only for base texture. Otherwise textures would be 1x1.
		texture.Set( width, height, 0, internalFormat, data );
	texture.NewMipmapLevel( level );

    PrintCompleteSummary( "CompressedTexImage2D() called" );
}

void BVGLResourceTrackingPlugin::CompressedTexImage3D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexImage3D( target, level, internalFormat, width, height, depth, border, imageSize, data );
		
	auto& texture = m_textures.GetBoundResource( target );

	if( level == 0 )		// We set size only for base texture. Otherwise textures would be 1x1.
		texture.Set( width, height, depth, internalFormat, data );
	texture.NewMipmapLevel( level );

    PrintCompleteSummary( "CompressedTexImage3D() called" );
}


void BVGLResourceTrackingPlugin::CompressedTexSubImage1D		( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexSubImage1D( target, level, xoffset, width, format, imageSize, data );
}

void BVGLResourceTrackingPlugin::CompressedTexSubImage2D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexSubImage2D( target, level, xoffset, yoffset, width, height, format, imageSize, data );
}

void BVGLResourceTrackingPlugin::CompressedTexSubImage3D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	Parent::CompressedTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data );
}

void BVGLResourceTrackingPlugin::TexSubImage1D				( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels )
{
	Parent::TexSubImage1D( target, level, xoffset, width, format, type, pixels );
}

void BVGLResourceTrackingPlugin::TexSubImage3D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels )
{
	Parent::TexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels );
}

void BVGLResourceTrackingPlugin::TexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width )
{
	Parent::TexStorage1D( target, levels, internalFormat, width );
	
	auto& texture = m_textures.GetBoundResource( target );

	texture.Set( width, 0, 0, internalFormat, nullptr );
	texture.NewMipmapLevel( levels - 1 );	// Base level is counted in.
}

void BVGLResourceTrackingPlugin::TexStorage2D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height )
{
	Parent::TexStorage2D( target, levels, internalFormat, width, height );
		
	auto& texture = m_textures.GetBoundResource( target );

	texture.Set( width, height, 0, internalFormat, nullptr );
	texture.NewMipmapLevel( levels - 1 );	// Base level is counted in.
}

void BVGLResourceTrackingPlugin::TexStorage3D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth )
{
	Parent::TexStorage3D( target, levels, internalFormat, width, height, depth );
		
	auto& texture = m_textures.GetBoundResource( target );

	texture.Set( width, height, depth, internalFormat, nullptr );
	texture.NewMipmapLevel( levels - 1 );	// Base level is counted in.
}

void BVGLResourceTrackingPlugin::TexStorage2DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	Parent::TexStorage2DMultisample( target, samples, internalFormat, width, height, fixedSampleLocations );
		
	auto& texture = m_textures.GetBoundResource( target );

	texture.Set( width, height, 0, internalFormat, nullptr );
}

void BVGLResourceTrackingPlugin::TexStorage3DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	Parent::TexStorage3DMultisample( target, samples, internalFormat, width, height, depth, fixedSampleLocations );
		
	auto& texture = m_textures.GetBoundResource( target );

	texture.Set( width, height, depth, internalFormat, nullptr );
}


void BVGLResourceTrackingPlugin::TexBuffer					( GLenum target, GLenum internalFormat, GLuint buffer )
{
	Parent::TexBuffer( target, internalFormat, buffer );
	m_buffers.BindResource( target, buffer );
}

void BVGLResourceTrackingPlugin::TexBufferRange				( GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	Parent::TexBufferRange( target, internalFormat, buffer, offset, size );
	m_buffers.BindResource( target, buffer );
}

void BVGLResourceTrackingPlugin::BindBufferBase				( GLenum target, GLuint index, GLuint buffer )
{
	Parent::BindBufferBase( target, index, buffer );
	m_buffers.BindResource( target, buffer );
}

void BVGLResourceTrackingPlugin::BindBufferRange				( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	Parent::BindBufferRange( target, index, buffer, offset, size );
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

#ifdef BV_GL_VERSION_4_5

void BVGLResourceTrackingPlugin::NamedFramebufferTexture		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level )
{
	Parent::NamedFramebufferTexture( framebuffer, attachment, texture, level );

	auto& resource = m_framebuffers.GetResource( framebuffer );
	resource.AttachTexture2D( attachment, m_framebuffers.BoundTo( framebuffer ), texture, level );
}

void BVGLResourceTrackingPlugin::NamedFramebufferRenderbuffer( GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
	Parent::NamedFramebufferRenderbuffer( framebuffer, attachment, renderbuffertarget, renderbuffer );

	auto& resource = m_framebuffers.GetResource( framebuffer );

	resource.AttachRenderbuffer( attachment, renderbuffertarget, renderbuffer );
}

void BVGLResourceTrackingPlugin::NamedRenderbufferStorage				( GLuint renderbuffer, GLenum internalFormat, GLsizei width, GLsizei height )
{
	Parent::NamedRenderbufferStorage( renderbuffer, internalFormat, width, height );

	auto& resource = m_renderbuffers.GetResource( renderbuffer );
	resource.Set( internalFormat, width, height );
}

void BVGLResourceTrackingPlugin::NamedRenderbufferStorageMultisample		( GLuint renderbuffer, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height )
{
	Parent::NamedRenderbufferStorageMultisample( renderbuffer, samples, internalFormat, width, height );

	auto& resource = m_renderbuffers.GetResource( renderbuffer );
	resource.Set( internalFormat, width, height );
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
