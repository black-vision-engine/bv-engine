#include "BVGLPlugin.h"

#include <glad/glad.h>

namespace bv {

// *****************************
// FIXME: this initialization should check more caps and return an error code instead of a simple bool
bool        BVGLPlugin::InitializeBVGL						()
{
	if( !gladLoadGL() )
	{
        return false;
    }
    
    return true;
}

// *****************************
//
void        BVGLPlugin::Viewport						( GLint x, GLint y, GLsizei width, GLsizei height )
{
    glViewport( x, y, width, height );
}

// *****************************
//
void        BVGLPlugin::ClearColor						( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    glClearColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGLPlugin::ClearDepth						( GLclampd depth )
{
    glClearDepth( depth );
}

// *****************************
//
void        BVGLPlugin::Clear							( GLbitfield mask )
{
    glClear( mask );
}

// *****************************
//
void        BVGLPlugin::DepthRange						( GLclampd zNear, GLclampd zFar )
{
    glDepthRange( zNear, zFar );
}

// *****************************
//
void        BVGLPlugin::LinkProgram						( GLuint program )
{
    glLinkProgram( program );
}

// *****************************
//
void        BVGLPlugin::UseProgram						( GLuint program )
{
    glUseProgram( program );
}

// *****************************
//
void        BVGLPlugin::ValidateProgram					( GLuint program )
{
    glValidateProgram( program );
}

// *****************************
//
void        BVGLPlugin::Uniform1i						( GLint location, GLint v )
{
    glUniform1i( location, v );
}

// *****************************
//
void        BVGLPlugin::Uniform1f						( GLint location, GLfloat v )
{
    glUniform1f( location, v );
}

// *****************************
//
void        BVGLPlugin::Uniform2f						( GLint location, GLfloat v, GLfloat v1 )
{
    glUniform2f( location, v, v1 );
}

// *****************************
//
void        BVGLPlugin::Uniform3f						( GLint location, GLfloat v, GLfloat v1, GLfloat v2 )
{
    glUniform3f( location, v, v1, v2 );
}

// *****************************
//
void        BVGLPlugin::Uniform4f						( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 )
{
    glUniform4f( location, v, v1, v2, v3 );
}

// *****************************
//
void        BVGLPlugin::UniformMatrix2fv				( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    glUniformMatrix2fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLPlugin::UniformMatrix3fv				( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    glUniformMatrix3fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLPlugin::UniformMatrix4fv				( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    glUniformMatrix4fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLPlugin::ShaderSource					( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length )
{
    glShaderSource( shader, count, strings, length ); 
}

// *****************************
//
void        BVGLPlugin::CompileShader					( GLuint shader )
{
    glCompileShader( shader );
}

// *****************************
//
void        BVGLPlugin::AttachShader					( GLuint program, GLuint shader )
{
    glAttachShader( program, shader );
}

// *****************************
//
void        BVGLPlugin::GetProgramiv					( GLuint program, GLenum pname, GLint * param )
{
    glGetProgramiv( program, pname, param );
}

// *****************************
//
void        BVGLPlugin::GetProgramInfoLog				( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    glGetProgramInfoLog( program, bufSize, length, infoLog );
}

// *****************************
//
void        BVGLPlugin::GetActiveUniform				( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    glGetActiveUniform( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGLPlugin::GetUniformLocation				( GLuint program, const GLchar * name )
{
    return glGetUniformLocation( program, name );
}

// *****************************
//
void        BVGLPlugin::GetActiveAttrib					( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    glGetActiveAttrib( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGLPlugin::GetAttribLocation				( GLuint program, const GLchar * name )
{
    return glGetAttribLocation( program, name );
}

// *****************************
//
void        BVGLPlugin::GetShaderiv						( GLuint shader, GLenum pname, GLint * param )
{
    glGetShaderiv( shader, pname, param );
}

// *****************************
//
void        BVGLPlugin::GetShaderInfoLog				( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    glGetShaderInfoLog( shader, bufSize, length, infoLog );
}

// *****************************
//
void        BVGLPlugin::GetIntegerv						( GLenum pname, GLint * params )
{
    glGetIntegerv( pname, params );
}

// *****************************
//
void        BVGLPlugin::GetDoublev						( GLenum pname, GLdouble * params )
{
    glGetDoublev( pname, params );
}

// *****************************
//
void        BVGLPlugin::BindAttribLocation				( GLuint program, GLuint index, const GLchar * name )
{
    glBindAttribLocation( program, index, name );
}

// *****************************
//
void        BVGLPlugin::BindFragDataLocation			( GLuint program, GLuint colorNumber, const GLchar * name )
{
    glBindFragDataLocation( program, colorNumber, name );
}

// *****************************
//
GLuint      BVGLPlugin::CreateProgram					()
{
    return glCreateProgram();
}

// *****************************
//
void        BVGLPlugin::DeleteProgram					( GLuint program )
{
    glDeleteProgram( program );
}

// *****************************
//
GLuint      BVGLPlugin::CreateShader					( GLenum type )
{
    return glCreateShader( type );
}

// *****************************
//
void        BVGLPlugin::DeleteShader					( GLuint shader )
{
    glDeleteShader( shader );
}

// *****************************
//
void        BVGLPlugin::GenBuffers						( GLsizei n, GLuint * buffers )
{
    glGenBuffers( n, buffers );
}

// *****************************
//
void        BVGLPlugin::DeleteBuffers					( GLsizei n, const GLuint * buffers )
{
    glDeleteBuffers( n, buffers );
}

// *****************************
//
void        BVGLPlugin::BindBuffer						( GLenum target, GLuint buffer )
{
    glBindBuffer( target, buffer );
}

// *****************************
//
void        BVGLPlugin::BufferData						( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    glBufferData( target, size, data, usage );
}

// *****************************
//
void        BVGLPlugin::BufferSubData					( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data )
{
	glBufferSubData( target, offset, size, data );
}


// *****************************
//
GLvoid *    BVGLPlugin::MapBuffer						( GLenum target, GLenum access )
{
    return glMapBuffer( target, access );
}

// *****************************
//
GLboolean    BVGLPlugin::UnmapBuffer					( GLenum target )
{
    return glUnmapBuffer( target );
}

// *****************************
//
void         BVGLPlugin::GenTextures					( GLsizei n, GLuint * textures )
{
    glGenTextures( n, textures );
}

// *****************************
//
void         BVGLPlugin::DeleteTextures					( GLsizei n, const GLuint * textures )
{
    glDeleteTextures( n, textures );
}

// *****************************
//
void         BVGLPlugin::TexImage2D						( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels )
{
    glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

// *****************************
//
void         BVGLPlugin::ActiveTexture					( GLenum texture )
{
    glActiveTexture( texture );
}

// *****************************
//
void         BVGLPlugin::TexSubImage2D					( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels )
{
    glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

// *****************************
//
void         BVGLPlugin::BindTexture					( GLenum target, GLuint texture )
{
    glBindTexture( target, texture );
}

// *****************************
//
void         BVGLPlugin::TexParameteri					( GLenum target, GLenum pname, GLint param )
{
    glTexParameteri( target, pname, param );
}

// *****************************
//
void            BVGLPlugin::TexParameterfv				( GLenum target, GLenum pname, const GLfloat * params )
{
    glTexParameterfv( target, pname, params );
}

// *****************************
//
void         BVGLPlugin::ReadBuffer						( GLenum mode )
{
    glReadBuffer( mode );
}

// *****************************
//
void         BVGLPlugin::ReadPixels						( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels )
{
    glReadPixels( x, y, width, height, format, type, pixels );
}

// *****************************
//
void         BVGLPlugin::GenFramebuffers				( GLsizei n, GLuint * framebuffers )
{
    glGenFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGLPlugin::DeleteFramebuffers				( GLsizei n, const GLuint * framebuffers )
{
    glDeleteFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGLPlugin::GenRenderbuffers				( GLsizei n, GLuint * renderbuffers )
{
    glGenRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGLPlugin::DeleteRenderbuffers			( GLsizei n, const GLuint * renderbuffers )
{
    glDeleteRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGLPlugin::BindRenderbuffer				( GLenum target, GLuint renderbuffer )
{
    glBindRenderbuffer( target, renderbuffer );
}

// *****************************
//
void         BVGLPlugin::RenderbufferStorage			( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
{
    glRenderbufferStorage( target, internalformat, width, height );
}

// *****************************
//
void         BVGLPlugin::BindFramebuffer				( GLenum target, GLuint framebuffer )
{
    glBindFramebuffer( target, framebuffer );
}

// *****************************
//
void         BVGLPlugin::FramebufferTexture2D			( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
    glFramebufferTexture2D( target, attachment, textarget, texture, level );
}

// *****************************
//
void         BVGLPlugin::FramebufferRenderbuffer		( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
    glFramebufferRenderbuffer( target, attachment, renderbuffertarget, renderbuffer );
}

// *****************************
//
void         BVGLPlugin::DrawBuffers					( GLsizei n, const GLenum * bufs )
{
    glDrawBuffers( n, bufs );
}

// *****************************
//
void        BVGLPlugin::DrawArrays						( GLenum mode, GLint first, GLsizei count )
{
    glDrawArrays( mode, first, count );
}

// *****************************
//
void        BVGLPlugin::DrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
    glDrawElements( mode, count, type, indices );
}

// *****************************
//
void        BVGLPlugin::DrawArraysInstanced							( GLenum mode, GLint first, GLsizei count, GLsizei primcount )
{
    glDrawArraysInstanced( mode, first, count, primcount );
}

// *****************************
//
void        BVGLPlugin::DrawElementsInstanced						( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount )
{
    glDrawElementsInstanced( mode, count, type, indices, primcount );
}

// *****************************
//
void        BVGLPlugin::DrawElementsBaseVertex						( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex )
{
    glDrawElementsBaseVertex( mode, count, type, indices, basevertex );
}

// *****************************
//
void        BVGLPlugin::DrawArraysInstancedBaseInstance				( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance )
{
    glDrawArraysInstancedBaseInstance( mode, first, count, primcount, baseinstance );
}

// *****************************
//
void        BVGLPlugin::DrawElementsInstancedBaseVertexBaseInstance	( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance )
{
    glDrawElementsInstancedBaseVertexBaseInstance( mode, count, type, indices, primcount, basevertex, baseinstance );
}

// *****************************
//
void        BVGLPlugin::DrawElementsInstancedBaseVertex				( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex )
{
    glDrawElementsInstancedBaseVertex( mode, count, type, indices, primcount, basevertex );
}

// *****************************
//
void        BVGLPlugin::DrawArraysIndirect							( GLenum mode, const void *indirect )
{
    glDrawArraysIndirect( mode, indirect );
}

// *****************************
//
void        BVGLPlugin::DrawElementsIndirect							( GLenum mode, GLenum type, const void *indirect )
{
    glDrawElementsIndirect( mode, type, indirect );
}

// *****************************
//
void        BVGLPlugin::MultiDrawArraysIndirect						( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    glMultiDrawArraysIndirect( mode, indirect, drawcount, stride );
}

// *****************************
//
void        BVGLPlugin::MultiDrawElementsIndirect					( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    glMultiDrawElementsIndirect( mode, type, indirect, drawcount, stride );
}

// *****************************
//
GLenum      BVGLPlugin::CheckFramebufferStatus			( GLenum target )
{
    return glCheckFramebufferStatus( target );
}

// *****************************
//
void        BVGLPlugin::Enable							( GLenum cap )
{
    glEnable( cap );
}

// *****************************
//
void        BVGLPlugin::Disable							( GLenum cap )
{
    glDisable( cap );
}

// *****************************
//
void        BVGLPlugin::DepthFunc						( GLenum func )
{
    glDepthFunc( func );
}

// *****************************
//
void        BVGLPlugin::DepthMask						( GLboolean flag )
{
    glDepthMask( flag );
}

// *****************************
//
void        BVGLPlugin::BlendFunc						( GLenum sfactor, GLenum dfactor )
{
    glBlendFunc( sfactor, dfactor );
}

// *****************************
//
void        BVGLPlugin::BlendColor						( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    glBlendColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGLPlugin::FrontFace						( GLenum mode )
{
    glFrontFace( mode );
}

// *****************************
//
void        BVGLPlugin::CullFace						( GLenum mode )
{
    glCullFace( mode );
}

// *****************************
//
void        BVGLPlugin::PolygonMode						( GLenum face, GLenum mode )
{
    glPolygonMode( face, mode );
}

// *****************************
//
void        BVGLPlugin::PolygonOffset					( GLfloat factor, GLfloat units )
{
    glPolygonOffset( factor, units );
}

// *****************************
//
GLenum      BVGLPlugin::GetError						()
{
    return glGetError();
}

// *****************************
//
const GLubyte * BVGLPlugin::GetString					( GLenum name )
{
    return glGetString( name );
}

// *****************************
//
const GLubyte * BVGLPlugin::GetStringi					( GLenum name, GLuint index )
{
    return glGetStringi( name, index );
}

// *****************************
//
void            BVGLPlugin::GenVertexArrays				( GLsizei n, GLuint * arrays )
{
    glGenVertexArrays( n, arrays );
}

// *****************************
//
void            BVGLPlugin::DeleteVertexArrays			( GLsizei n, const GLuint * arrays )
{
    glDeleteVertexArrays( n, arrays );
}

// *****************************
//
void            BVGLPlugin::EnableVertexAttribArray		( GLuint index )
{
    glEnableVertexAttribArray( index );
}

// *****************************
//
void            BVGLPlugin::DisableVertexAttribArray	( GLuint index )
{
    glDisableVertexAttribArray( index );
}

// *****************************
//
void            BVGLPlugin::BindVertexArray				( GLuint array )
{
    glBindVertexArray( array );
}

// *****************************
//
void            BVGLPlugin::VertexAttribPointer			( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    glVertexAttribPointer( index, size, type, normalized, stride, pointer );
}

// *****************************
//
void			BVGLPlugin::VertexAttribDivisor			( GLuint index,  GLuint divisor )
{
    glVertexAttribDivisor( index, divisor );
}

// *****************************
//
void			BVGLPlugin::PrintShortSummary           ( const std::string & )
{
}

// *****************************
//
void			BVGLPlugin::PrintCompleteSummary        ( const std::string & )
{
}

#ifdef BV_GL_VERSION_4_4
// Images and textures
void BVGLPlugin::BindImageTexture			( GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format )
{
	glBindImageTexture( unit, texture, level, layered, layer, access, format );
}


void BVGLPlugin::BindImageTextures			( GLuint first, GLsizei count, const GLuint * textures )
{
	glBindImageTextures( first, count, textures );
}

void BVGLPlugin::BindTextures				( GLuint first, GLsizei count, const GLuint* textures )
{
	glBindTextures( first, count, textures );
}

#endif

#ifdef BV_GL_VERSION_4_5

void BVGLPlugin::BindTextureUnit				( GLuint unit, GLuint texture )
{
	glBindTextureUnit( unit, texture );
}

#endif

void BVGLPlugin::CompressedTexImage1D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTexImage1D( target, level, internalFormat, width, border, imageSize, data );
}

void BVGLPlugin::CompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTexImage2D( target, level, internalFormat, width, height, border, imageSize, data );
}

void BVGLPlugin::CompressedTexImage3D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTexImage3D( target, level, internalFormat, width, height, depth, border, imageSize, data );
}

void BVGLPlugin::CompressedTexSubImage1D		( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTexSubImage1D( target, level, xoffset, width, format, imageSize, data );
}

void BVGLPlugin::CompressedTexSubImage2D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTexSubImage2D( target, level, xoffset, yoffset, width, height, format, imageSize, data );
}

void BVGLPlugin::CompressedTexSubImage3D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data );
}

#ifdef BV_GL_VERSION_4_5

void BVGLPlugin::CompressedTextureSubImage1D	( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTextureSubImage1D( texture, level, xoffset, width, format, imageSize, data );
}

void BVGLPlugin::CompressedTextureSubImage2D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTextureSubImage2D( texture, level, xoffset, yoffset, width, height, format, imageSize, data );
}

void BVGLPlugin::CompressedTextureSubImage3D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	glCompressedTextureSubImage3D( texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data );
}

#endif

void BVGLPlugin::TexImage1D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	glTexImage1D( target, level, internalFormat, width, border, format, type, data );
}

void BVGLPlugin::TexImage3D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	glTexImage3D( target, level, internalFormat, width, height, depth, border, format, type, data );
}

/**GL4.3*/
void BVGLPlugin::TexImage2DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	glTexImage2DMultisample( target, samples, internalFormat, width, height, fixedSampleLocations );
}
/**GL4.3*/
void BVGLPlugin::TexImage3DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	glTexImage3DMultisample( target, samples, internalFormat, width, height, depth, fixedSampleLocations );
}

/**GL4.2*/
void BVGLPlugin::TexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width )
{
	glTexStorage1D( target, levels, internalFormat, width );
}

void BVGLPlugin::TexStorage2D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height )
{
	glTexStorage2D( target, levels, internalFormat, width, height );
}

/**GL4.2*/
void BVGLPlugin::TexStorage3D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth )
{
	glTexStorage3D( target, levels, internalFormat, width, height, depth );
}

/**GL4.3*/
void BVGLPlugin::TexStorage2DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	glTexStorage2DMultisample( target, samples, internalFormat, width, height, fixedSampleLocations );
}

/**GL4.3*/
void BVGLPlugin::TexStorage3DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	glTexStorage3DMultisample( target, samples, internalFormat, width, height, depth, fixedSampleLocations );
}


void BVGLPlugin::TexSubImage1D				( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels )
{
	glTexSubImage1D( target, level, xoffset, width, format, type, pixels );
}

void BVGLPlugin::TexSubImage3D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels )
{
	glTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels );
}

#ifdef BV_GL_VERSION_4_5

void BVGLPlugin::TextureStorage1D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width )
{
	glTextureStorage1D( texture, levels, internalFormat, width );
}

void BVGLPlugin::TextureStorage2D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height )
{
	glTextureStorage2D( texture, levels, internalFormat, width, height );
}

void BVGLPlugin::TextureStorage3D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth )
{
	glTextureStorage3D( texture, levels , internalFormat, width, height, depth );
}

void BVGLPlugin::TextureStorage2DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	glTextureStorage2DMultisample( texture, samples, internalFormat, width, height, fixedSampleLocations );
}

void BVGLPlugin::TextureStorage3DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	glTextureStorage3DMultisample( texture, samples, internalFormat, width, height, depth, fixedSampleLocations );
}


void BVGLPlugin::TextureSubImage1D			( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels )
{
	glTextureSubImage1D( texture, level, xoffset, width, format, type, pixels );
}

void BVGLPlugin::TextureSubImage2D			( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels )
{
	glTextureSubImage2D( texture, level, xoffset, yoffset, width, height, format, type, pixels );
}

void BVGLPlugin::TextureSubImage3D			( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels )
{
	glTextureSubImage3D( texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels );
}

void BVGLPlugin::TextureBuffer				( GLuint texture, GLenum internalFormat, GLuint buffer )
{
	glTextureBuffer( texture, internalFormat, buffer );
}

void BVGLPlugin::TextureBufferRange			( GLuint texture, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	glTextureBufferRange( texture, internalFormat, buffer, offset, size );
}

#endif

void BVGLPlugin::TexBuffer					( GLenum target, GLenum internalFormat, GLuint buffer )
{
	glTexBuffer( target, internalFormat, buffer );
}

/**GL4.3*/
void BVGLPlugin::TexBufferRange				( GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	glTexBufferRange( target, internalFormat, buffer, offset, size );
}

/**GL4.3*/
void BVGLPlugin::TextureView					( GLuint texture, GLenum target, GLuint origTexture, GLenum internalFormat, GLuint minLevel, GLuint numLevels, GLuint minLayer, GLuint numLayers )
{
	glTextureView( texture, target, origTexture, internalFormat, minLevel, numLevels, minLayer, numLayers );
}

#ifdef BV_GL_VERSION_4_5
void BVGLPlugin::TextureParameteri			( GLuint texture, GLenum pname, GLint param )
{
	glTextureParameteri( texture, pname, param );
}

void BVGLPlugin::TextureParameterfv			( GLuint texture, GLenum pname, const GLfloat * params )
{
	glTextureParameterfv( texture, pname, params );
}
#endif


void BVGLPlugin::CopyTexSubImage1D			( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width )
{
	glCopyTexSubImage1D( target, level, xoffset, x, y, width );
}

void BVGLPlugin::CopyTexSubImage2D			( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	glCopyTexSubImage2D( target, level, xoffset, yoffset, x, y, width, height );
}

void BVGLPlugin::CopyTexSubImage3D			( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	glCopyTexSubImage3D( target, level, xoffset, yoffset, zoffset, x, y, width, height );
}

#ifdef BV_GL_VERSION_4_5
void BVGLPlugin::CopyTextureSubImage1D		( GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width )
{
	glCopyTextureSubImage1D( texture, level, xoffset, x, y, width );
}

void BVGLPlugin::CopyTextureSubImage2D		( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	glCopyTextureSubImage2D( texture, level, xoffset, yoffset, x, y, width, height );
}

void BVGLPlugin::CopyTextureSubImage3D		( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	glCopyTextureSubImage3D( texture, level, xoffset, yoffset, zoffset, x, y, width, height );
}
#endif

void BVGLPlugin::CopyTexImage2D				( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border )
{
	glCopyTexImage2D( target, level, internalFormat, x, y, width, height, border );
}

void BVGLPlugin::CopyTexImage1D				( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border )
{
	glCopyTexImage1D( target, level, internalFormat, x, y, width, border );
}

/**GL4.3*/
void BVGLPlugin::CopyImageSubData			( GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth )
{
	glCopyImageSubData( srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth );
}

// Sampler

#ifdef BV_GL_VERSION_4_5
void BVGLPlugin::CreateSamplers				( GLsizei n, GLuint* samplers )
{
	glCreateSamplers( n, samplers );
}
#endif

void BVGLPlugin::GenSamplers					( GLsizei n, GLuint* samplers )
{
	glGenSamplers( n, samplers );
}

void BVGLPlugin::DeleteSamplers				( GLsizei n, GLuint* samplers )
{
	glDeleteSamplers( n, samplers );
}


void BVGLPlugin::BindSampler					( GLuint unit, GLuint sampler )
{
	glBindSampler( unit, sampler );
}

#ifdef BV_GL_VERSION_4_4
void BVGLPlugin::BindSamplers				( GLuint first, GLsizei count, const GLuint* samplers )
{
	glBindSamplers( first, count, samplers );
}
#endif

void BVGLPlugin::SamplerParameteri			( GLuint sampler, GLenum pname, GLint param )
{
	glSamplerParameteri( sampler, pname, param );
}

void BVGLPlugin::SamplerParameterfv			( GLuint sampler, GLenum pname, const GLfloat* params )
{
	glSamplerParameterfv( sampler, pname, params );
}

void BVGLPlugin::SamplerParameterf			( GLuint sampler, GLenum pname, GLfloat param )
{
	glSamplerParameterf( sampler, pname, param );
}

// Transform Feedback
/**GL4.0*/
void BVGLPlugin::GenTransformFeedbacks		( GLsizei n, GLuint* ids )
{
	glGenTransformFeedbacks( n, ids );
}
/**GL4.0*/
void BVGLPlugin::BindTransformFeedback		( GLenum target, GLuint id )
{
	glBindTransformFeedback( target, id );
}

void BVGLPlugin::BeginTransformFeedback		( GLenum primitiveMode )
{
	glBeginTransformFeedback( primitiveMode );
}

void BVGLPlugin::EndTransformFeedback		()
{
	glEndTransformFeedback();
}

/**GL4.0*/
void BVGLPlugin::DrawTransformFeedback				( GLenum mode, GLuint id )
{
	glDrawTransformFeedback( mode, id );
}
/**GL4.2*/
void BVGLPlugin::DrawTransformFeedbackInstanced		( GLenum mode, GLuint id, GLsizei primCount )
{
	glDrawTransformFeedbackInstanced( mode, id, primCount );
}

// Buffer
#ifdef BV_GL_VERSION_4_5
void BVGLPlugin::NamedBufferData				( GLuint buffer, GLsizei size, const void* data, GLenum usage )
{
	glNamedBufferData( buffer, size, data, usage );
}

void BVGLPlugin::NamedBufferStorage			( GLuint buffer, GLsizei size, const void* data, GLbitfield flags )
{
	glNamedBufferStorage( buffer, size, data, flags );
}

void BVGLPlugin::NamedBufferSubData			( GLuint buffer, GLintptr offset, GLsizei size, const void* data )
{
	glNamedBufferSubData( buffer, offset, size, data );
}
#endif

#ifdef BV_GL_VERSION_4_4
void BVGLPlugin::BufferStorage				( GLenum target, GLsizeiptr size, const GLvoid* data, GLbitfield flags )
{
	glBufferStorage( target, size, data, flags );
}
#endif

void BVGLPlugin::BindBufferBase				( GLenum target, GLuint index, GLuint buffer )
{
	glBindBufferBase( target, index, buffer );
}

#ifdef BV_GL_VERSION_4_4
void BVGLPlugin::BindBuffersBase				( GLenum target, GLuint first, GLsizei count, const GLuint* buffer )
{
	glBindBuffersBase( target, first, count, buffer );
}

void BVGLPlugin::BindBuffersRange			( GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offset, const GLsizeiptr* sizes )
{
	glBindBuffersRange( target, first, count, buffers, offset, sizes );
}
#endif

void BVGLPlugin::BindBufferRange				( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	glBindBufferRange( target, index, buffer, offset, size );
}

/**GL4.3*/
void BVGLPlugin::BindVertexBuffer			( GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizei stride )
{
	glBindVertexBuffer( bindingIndex, buffer, offset, stride );
}

#ifdef BV_GL_VERSION_4_4
void BVGLPlugin::BindVertexBuffers			( GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides )
{
	glBindVertexBuffers( first, count, buffers, offsets, strides );
}
#endif

#ifdef BV_GL_VERSION_4_5

void BVGLPlugin::VertexArrayVertexBuffers	( GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides )
{
	glVertexArrayVertexBuffers( vaobj, first, count, buffers, offsets, strides );
}

void* BVGLPlugin::MapNamedBuffer				( GLuint buffer, GLenum access )
{
	return glMapNamedBuffer( buffer, access );
}

void* BVGLPlugin::MapNamedBufferRange			( GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access )
{
	return glMapNamedBufferRange( buffer, offset, length, access );
}

GLboolean BVGLPlugin::UnmapNamedBuffer			( GLuint buffer )
{
	return glUnmapNamedBuffer( buffer );
}
#endif

void* BVGLPlugin::MapBufferRange				( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access )
{
	return glMapBufferRange( target, offset, length, access );
}


void BVGLPlugin::CopyBufferSubData			( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size )
{
	glCopyBufferSubData( readBuffer, writeBuffer, readOffset, writeOffset, size );
}

#ifdef BV_GL_VERSION_4_5
void BVGLPlugin::CopyNamedBufferSubData		( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size )
{
	glCopyNamedBufferSubData( readBuffer, writeBuffer, readOffset, writeOffset, size );
}
#endif
// Blending
void BVGLPlugin::BlendEquation				( GLenum mode )
{
	glBlendEquation( mode );
}

/**GL4.0*/
void BVGLPlugin::BlendEquationi				( GLuint buf, GLenum mode )
{
	glBlendEquationi( buf, mode );
}

void BVGLPlugin::BlendEquationSeparate		( GLenum modeRGB, GLenum modeAlpha )
{
	glBlendEquationSeparate( modeRGB, modeAlpha );
}

/**GL4.0*/
void BVGLPlugin::BlendEquationSeparatei		( GLuint buf, GLenum modeRGB, GLenum modeAlpha )
{
	glBlendEquationSeparatei( buf, modeRGB, modeAlpha );
}

/**GL4.0*/
void BVGLPlugin::BlendFunci					( GLuint buf, GLenum sfactor, GLenum dfactor )
{
	glBlendFunci( buf, sfactor, dfactor );
}

void BVGLPlugin::BlendFuncSeparate			( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )
{
	glBlendFuncSeparate( srcRGB, dstRGB, srcAlpha, dstAlpha );
}

/**GL4.0*/
void BVGLPlugin::BlendFuncSeparatei			( GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )
{
	glBlendFuncSeparatei( buf, srcRGB, dstRGB, srcAlpha, dstAlpha );
}


// Framebuffer
void BVGLPlugin::BlitFramebuffer				( GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter )
{
	glBlitFramebuffer( srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter );
}

#ifdef BV_GL_VERSION_4_5
void BVGLPlugin::BlitNamedFramebuffer		( GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter )
{
	glBlitNamedFramebuffer( readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter );
}
#endif

	/// Wystarczy zestaw FramebufferTexture, FramebufferTextureLayer oraz FramebufferTexture2D ewentualnie ich wersje z DSA
void BVGLPlugin::FramebufferTexture			( GLenum target, GLenum attachment, GLuint texture, GLint level )
{
	glFramebufferTexture( target, attachment, texture, level );
}

void BVGLPlugin::FramebufferTextureLayer		( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer )
{
	glFramebufferTextureLayer( target, attachment, texture, level, layer );
}

#ifdef BV_GL_VERSION_4_5
void BVGLPlugin::NamedFramebufferTexture		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level )
{
	glNamedFramebufferTexture( framebuffer, attachment, texture, level );
}

void BVGLPlugin::NamedFramebufferTextureLayer			( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer )
{
	glNamedFramebufferTextureLayer( framebuffer, attachment, texture, level, layer );
}

void BVGLPlugin::NamedFramebufferRenderbuffer			( GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
	glNamedFramebufferRenderbuffer( framebuffer, attachment, renderbuffertarget, renderbuffer );
}

GLenum BVGLPlugin::CheckNamedFramebufferStatus	( GLuint framebuffer, GLenum target )
{
	return glCheckNamedFramebufferStatus( framebuffer, target );
}
#endif

void BVGLPlugin::ClearBufferiv				( GLenum buffer, GLint drawbuffer, const GLint* value )
{
	glClearBufferiv( buffer, drawbuffer, value );
}

void BVGLPlugin::ClearBufferuiv				( GLenum buffer, GLint drawbuffer, const GLuint* value )
{
	glClearBufferuiv( buffer, drawbuffer, value );
}

void BVGLPlugin::ClearBufferfv				( GLenum buffer, GLint drawbuffer, const GLfloat* value )
{
	glClearBufferfv( buffer, drawbuffer, value );
}

void BVGLPlugin::ClearBufferfi				( GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil )
{
	glClearBufferfi( buffer, drawbuffer, depth, stencil );
}

#ifdef BV_GL_VERSION_4_5

void BVGLPlugin::ClearNamedFramebufferiv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value )
{
	glClearNamedFramebufferiv( framebuffer, buffer, drawbuffer, value );
}

void BVGLPlugin::ClearNamedFramebufferuiv	( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value )
{
	glClearNamedFramebufferuiv( framebuffer, buffer, drawbuffer, value );
}

void BVGLPlugin::ClearNamedFramebufferfv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value )
{
	glClearNamedFramebufferfv( framebuffer, buffer, drawbuffer, value );
}

void BVGLPlugin::ClearNamedFramebufferfi	( GLuint framebuffer, GLenum buffer, GLfloat depth, GLint stencil )
{
	glClearNamedFramebufferfi( framebuffer, buffer, depth, stencil );
}


void BVGLPlugin::NamedFramebufferDrawBuffer	( GLuint framebuffer, GLenum buf )
{
	glNamedFramebufferDrawBuffer( framebuffer, buf );
}

void BVGLPlugin::NamedFramebufferDrawBuffers	( GLuint framebuffer, GLsizei n, const GLenum* bufs )
{
	glNamedFramebufferDrawBuffers( framebuffer, n, bufs );
}

void BVGLPlugin::NamedFramebufferReadBuffer	( GLuint framebuffer, GLenum mode )
{
	glNamedFramebufferReadBuffer( framebuffer, mode );
}

void BVGLPlugin::NamedRenderbufferStorage	( GLuint renderbuffer, GLenum internalFormat, GLsizei width, GLsizei height )
{
	glNamedRenderbufferStorage( renderbuffer, internalFormat, width, height );
}

void BVGLPlugin::NamedRenderbufferStorageMultisample	( GLuint renderbuffer, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height )
{
	glNamedRenderbufferStorageMultisample( renderbuffer, samples, internalFormat, width, height );
}

#endif

void BVGLPlugin::RenderbufferStorageMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height )
{
	glRenderbufferStorageMultisample( target, samples, internalFormat, width, height );
}


// Drawing
void BVGLPlugin::MultiDrawArrays				( GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawCount )
{
	glMultiDrawArrays( mode, first, count, drawCount );
}

void BVGLPlugin::MultiDrawElements			( GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indicies, GLsizei drawCount )
{
	glMultiDrawElements( mode, count, type, indicies, drawCount );
}

void BVGLPlugin::MultiDrawElementsBaseVertex	( GLenum mode, GLsizei* count, GLenum type, const GLvoid** indicies, GLsizei drawcount, GLint* baseVertex )
{
	glMultiDrawElementsBaseVertex( mode, count, type, indicies, drawcount, baseVertex );
}


// Stencil
void BVGLPlugin::ClearStencil				( GLint s )
{
	glClearStencil( s );
}


void BVGLPlugin::StencilFunc					( GLenum func, GLint ref, GLuint mask )
{
	glStencilFunc( func, ref, mask );
}

void BVGLPlugin::StencilFuncSeparate			( GLenum face, GLenum func, GLint ref, GLuint mask )
{
	glStencilFuncSeparate( face, func, ref, mask );
}

void BVGLPlugin::StencilMask					( GLuint mask )
{
	glStencilMask( mask );
}

void BVGLPlugin::StencilMaskSeparate			( GLenum face, GLuint mask )
{
	glStencilMaskSeparate( face, mask );
}

void BVGLPlugin::StencilOp					( GLenum sfail, GLenum dpfail, GLenum dppass )
{
	glStencilOp( sfail, dpfail, dppass );
}

void BVGLPlugin::StencilOpSeparate			( GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass )
{
	glStencilOpSeparate( face, sfail, dpfail, dppass );
}

} //bv