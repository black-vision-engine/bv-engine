#include "BVGL.h"
#include "BVGLPlugins\BVGLPluginFactory.h"


namespace bv {

namespace {
	
BVGLPlugin &	bvgl = BVGLPluginFactory::BVGL	();

} // anonymous


// *****************************
// FIXME: this initialization should check much more caps and return an error code instead of a simple bool
bool        BVGL::InitializeBVGL                ()
{
	return bvgl.InitializeBVGL();
}

// *****************************
//
void        BVGL::PrintShortSummary	            ( const char * message )
{
    bvgl.PrintShortSummary( message );
}

// *****************************
//
void        BVGL::PrintCompleteSummary	        ( const char * message )
{
    bvgl.PrintCompleteSummary( message );
}

// *****************************
//
void        BVGL::bvglViewport                  ( GLint x, GLint y, GLsizei width, GLsizei height )
{
	bvgl.Viewport( x, y, width, height );
}

// *****************************
//
void        BVGL::bvglClearColor                ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
	bvgl.ClearColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGL::bvglClearDepth                ( GLclampd depth )
{
	bvgl.ClearDepth( depth );
}

// *****************************
//
void        BVGL::bvglClear                     ( GLbitfield mask )
{
	bvgl.Clear( mask );
}

// *****************************
//
void        BVGL::bvglDepthRange                ( GLclampd zNear, GLclampd zFar )
{
	bvgl.DepthRange( zNear, zFar );
}

// *****************************
//
void        BVGL::bvglLinkProgram               ( GLuint program )
{
	bvgl.LinkProgram( program );
}

// *****************************
//
void        BVGL::bvglUseProgram                ( GLuint program )
{
	bvgl.UseProgram( program );
}

// *****************************
//
void        BVGL::bvglValidateProgram           ( GLuint program )
{
	bvgl.ValidateProgram( program );
}

// *****************************
//
void        BVGL::bvglUniform1i                 ( GLint location, GLint v )
{
	bvgl.Uniform1i( location, v );
}

// *****************************
//
void        BVGL::bvglUniform1f                 ( GLint location, GLfloat v )
{
	bvgl.Uniform1f( location, v );
}

// *****************************
//
void        BVGL::bvglUniform2f                 ( GLint location, GLfloat v, GLfloat v1 )
{
	bvgl.Uniform2f( location, v, v1 );
}

// *****************************
//
void        BVGL::bvglUniform3f                 ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 )
{
	bvgl.Uniform3f( location, v, v1, v2 );
}

// *****************************
//
void        BVGL::bvglUniform4f                 ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 )
{
	bvgl.Uniform4f( location, v, v1, v2, v3 );
}

// *****************************
//
void        BVGL::bvglUniformMatrix2fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
	bvgl.UniformMatrix2fv( location, count, transpose, value );
}

// *****************************
//
void        BVGL::bvglUniformMatrix3fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
	bvgl.UniformMatrix3fv( location, count, transpose, value );
}

// *****************************
//
void        BVGL::bvglUniformMatrix4fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
	bvgl.UniformMatrix4fv( location, count, transpose, value );
}

// *****************************
//
void        BVGL::bvglShaderSource              ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length )
{
	bvgl.ShaderSource( shader, count, strings, length ); 
}

// *****************************
//
void        BVGL::bvglCompileShader             ( GLuint shader )
{
	bvgl.CompileShader( shader );
}

// *****************************
//
void        BVGL::bvglAttachShader              ( GLuint program, GLuint shader )
{
	bvgl.AttachShader( program, shader );
}

// *****************************
//
void        BVGL::bvglGetProgramiv              ( GLuint program, GLenum pname, GLint * param )
{
	bvgl.GetProgramiv( program, pname, param );
}

// *****************************
//
void        BVGL::bvglGetProgramInfoLog         ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
	bvgl.GetProgramInfoLog( program, bufSize, length, infoLog );
}

// *****************************
//
void        BVGL::bvglGetActiveUniform          ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
	bvgl.GetActiveUniform( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGL::bvglGetUniformLocation        ( GLuint program, const GLchar * name )
{
	return bvgl.GetUniformLocation( program, name );
}

// *****************************
//
void        BVGL::bvglGetActiveAttrib           ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
	bvgl.GetActiveAttrib( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGL::bvglGetAttribLocation         ( GLuint program, const GLchar * name )
{
	return bvgl.GetAttribLocation( program, name );
}

// *****************************
//
void        BVGL::bvglGetShaderiv               ( GLuint shader, GLenum pname, GLint * param )
{
	bvgl.GetShaderiv( shader, pname, param );
}

// *****************************
//
void        BVGL::bvglGetShaderInfoLog          ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
	bvgl.GetShaderInfoLog( shader, bufSize, length, infoLog );
}

// *****************************
//
void        BVGL::bvglGetIntegerv               ( GLenum pname, GLint * params )
{
	bvgl.GetIntegerv( pname, params );
}

// *****************************
//
void        BVGL::bvglGetDoublev                ( GLenum pname, GLdouble * params )
{
	bvgl.GetDoublev( pname, params );
}

// *****************************
//
void        BVGL::bvglBindAttribLocation        ( GLuint program, GLuint index, const GLchar * name )
{
	bvgl.BindAttribLocation( program, index, name );
}

// *****************************
//
void        BVGL::bvglBindFragDataLocation      ( GLuint program, GLuint colorNumber, const GLchar * name )
{
	bvgl.BindFragDataLocation( program, colorNumber, name );
}

// *****************************
//
GLuint      BVGL::bvglCreateProgram             ()
{
	return bvgl.CreateProgram();
}

// *****************************
//
void        BVGL::bvglDeleteProgram             ( GLuint program )
{
	bvgl.DeleteProgram( program );
}

// *****************************
//
GLuint      BVGL::bvglCreateShader              ( GLenum type )
{
	return bvgl.CreateShader( type );
}

// *****************************
//
void        BVGL::bvglDeleteShader              ( GLuint shader )
{
	bvgl.DeleteShader( shader );
}

// *****************************
//
void        BVGL::bvglGenBuffers                ( GLsizei n, GLuint * buffers )
{
	bvgl.GenBuffers( n, buffers );
}

// *****************************
//
void        BVGL::bvglDeleteBuffers             ( GLsizei n, const GLuint * buffers )
{
	bvgl.DeleteBuffers( n, buffers );
}

// *****************************
//
void        BVGL::bvglBindBuffer                ( GLenum target, GLuint buffer )
{
	bvgl.BindBuffer( target, buffer );
}

// *****************************
//
void        BVGL::bvglBufferData                ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
	bvgl.BufferData( target, size, data, usage );
}

// *****************************
//
void		BVGL::bvglBufferSubData				( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data )
{
	bvgl.BufferSubData( target, offset, size, data );
}

// *****************************
//
GLvoid *    BVGL::bvglMapBuffer                 ( GLenum target, GLenum access )
{
	return bvgl.MapBuffer( target, access );
}

// *****************************
//
GLboolean    BVGL::bvglUnmapBuffer              ( GLenum target )
{
	return bvgl.UnmapBuffer( target );
}

// *****************************
//
void         BVGL::bvglGenTextures              ( GLsizei n, GLuint * textures )
{
	bvgl.GenTextures( n, textures );
}

// *****************************
//
void         BVGL::bvglDeleteTextures           ( GLsizei n, const GLuint * textures )
{
	bvgl.DeleteTextures( n, textures );
}

// *****************************
//
void         BVGL::bvglTexImage2D               ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels )
{
	bvgl.TexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

// *****************************
//
void         BVGL::bvglActiveTexture            ( GLenum texture )
{
	bvgl.ActiveTexture( texture );
}

// *****************************
//
void         BVGL::bvglTexSubImage2D            ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels )
{
	bvgl.TexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

// *****************************
//
void         BVGL::bvglBindTexture              ( GLenum target, GLuint texture )
{
	bvgl.BindTexture( target, texture );
}

// *****************************
//
void         BVGL::bvglTexParameteri            ( GLenum target, GLenum pname, GLint param )
{
	bvgl.TexParameteri( target, pname, param );
}

// *****************************
//
void            BVGL::bvglTexParameterfv        ( GLenum target, GLenum pname, const GLfloat * params )
{
	bvgl.TexParameterfv( target, pname, params );
}

// *****************************
//
void         BVGL::bvglReadBuffer               ( GLenum mode )
{
	bvgl.ReadBuffer( mode );
}

// *****************************
//
void         BVGL::bvglReadPixels               ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels )
{
	bvgl.ReadPixels( x, y, width, height, format, type, pixels );
}

// *****************************
//
void         BVGL::bvglGenFramebuffers          ( GLsizei n, GLuint * framebuffers )
{
	bvgl.GenFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGL::bvglDeleteFramebuffers       ( GLsizei n, const GLuint * framebuffers )
{
	bvgl.DeleteFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGL::bvglGenRenderbuffers         ( GLsizei n, GLuint * renderbuffers )
{
	bvgl.GenRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGL::bvglDeleteRenderbuffers      ( GLsizei n, const GLuint * renderbuffers )
{
	bvgl.DeleteRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGL::bvglBindRenderbuffer         ( GLenum target, GLuint renderbuffer )
{
	bvgl.BindRenderbuffer( target, renderbuffer );
}

// *****************************
//
void         BVGL::bvglRenderbufferStorage      ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
{
	bvgl.RenderbufferStorage( target, internalformat, width, height );
}

// *****************************
//
void         BVGL::bvglBindFramebuffer          ( GLenum target, GLuint framebuffer )
{
	bvgl.BindFramebuffer( target, framebuffer );
}

// *****************************
//
void         BVGL::bvglFramebufferTexture2D     ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
	bvgl.FramebufferTexture2D( target, attachment, textarget, texture, level );
}

// *****************************
//
void         BVGL::bvglFramebufferRenderbuffer  ( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
	bvgl.FramebufferRenderbuffer( target, attachment, renderbuffertarget, renderbuffer );
}

// *****************************
//
void         BVGL::bvglDrawBuffers              ( GLsizei n, const GLenum * bufs )
{
	bvgl.DrawBuffers( n, bufs );
}

// *****************************
//
void        BVGL::bvglDrawArrays                ( GLenum mode, GLint first, GLsizei count )
{
	bvgl.DrawArrays( mode, first, count );
}

// *****************************
//
void        BVGL::bvglDrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
    bvgl.DrawElements( mode, count, type, indices );
}

// *****************************
//
void        BVGL::bvglDrawElementsBaseVertex						( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex )
{
    bvgl.DrawElementsBaseVertex( mode, count, type, indices, basevertex );
}

// *****************************
//
void        BVGL::bvglDrawArraysInstanced							( GLenum mode, GLint first, GLsizei count, GLsizei primcount )
{
    bvgl.DrawArraysInstanced( mode, first, count, primcount );
}

// *****************************
//
void        BVGL::bvglDrawElementsInstanced							( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount )
{
    bvgl.DrawElementsInstanced( mode, count, type, indices, primcount );
}

// *****************************
//
void        BVGL::bvglDrawArraysInstancedBaseInstance				( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance )
{
    bvgl.DrawArraysInstancedBaseInstance( mode, first, count, primcount, baseinstance );
}

// *****************************
//
void        BVGL::bvglDrawElementsInstancedBaseVertexBaseInstance	( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance )
{
    bvgl.DrawElementsInstancedBaseVertexBaseInstance( mode, count, type, indices, primcount, basevertex, baseinstance );
}

// *****************************
//
void        BVGL::bvglDrawElementsInstancedBaseVertex				( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex )
{
    bvgl.DrawElementsInstancedBaseVertex( mode, count, type, indices, primcount, basevertex );
}

// *****************************
//
void        BVGL::bvglDrawArraysIndirect		( GLenum mode, const void *indirect )
{
    bvgl.DrawArraysIndirect( mode, indirect );
}

// *****************************
//
void        BVGL::bvglDrawElementsIndirect	( GLenum mode, GLenum type, const void *indirect )
{
    bvgl.DrawElementsIndirect( mode, type, indirect );
}

// *****************************
//
void        BVGL::bvglMultiDrawArraysIndirect	( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    bvgl.MultiDrawArraysIndirect( mode, indirect, drawcount, stride );
}

// *****************************
//
void        BVGL::bvglMultiDrawElementsIndirect	( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    bvgl.MultiDrawElementsIndirect( mode, type, indirect, drawcount, stride );
}

// *****************************
//
GLenum      BVGL::bvglCheckFramebufferStatus    ( GLenum target )
{
	return bvgl.CheckFramebufferStatus( target );
}

// *****************************
//
void        BVGL::bvglEnable                    ( GLenum cap )
{
	bvgl.Enable( cap );
}

// *****************************
//
void        BVGL::bvglDisable                   ( GLenum cap )
{
	bvgl.Disable( cap );
}

// *****************************
//
void        BVGL::bvglDepthFunc                 ( GLenum func )
{
	bvgl.DepthFunc( func );
}

// *****************************
//
void        BVGL::bvglDepthMask                 ( GLboolean flag )
{
	bvgl.DepthMask( flag );
}

// *****************************
//
void        BVGL::bvglBlendFunc                 ( GLenum sfactor, GLenum dfactor )
{
	bvgl.BlendFunc( sfactor, dfactor );
}

// *****************************
//
void        BVGL::bvglBlendColor                ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
	bvgl.BlendColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGL::bvglFrontFace                 ( GLenum mode )
{
	bvgl.FrontFace( mode );
}

// *****************************
//
void        BVGL::bvglCullFace                  ( GLenum mode )
{
	bvgl.CullFace( mode );
}

// *****************************
//
void        BVGL::bvglPolygonMode               ( GLenum face, GLenum mode )
{
	bvgl.PolygonMode( face, mode );
}

// *****************************
//
void        BVGL::bvglPolygonOffset             ( GLfloat factor, GLfloat units )
{
	bvgl.PolygonOffset( factor, units );
}

// *****************************
//
GLenum      BVGL::bvglGetError                  ()
{
	return bvgl.GetError();
}

// *****************************
//
const GLubyte * BVGL::bvglGetString             ( GLenum name )
{
	return bvgl.GetString( name );
}

// *****************************
//
const GLubyte * BVGL::bvglGetStringi            ( GLenum name, GLuint index )
{
	return bvgl.GetStringi( name, index );
}

// *****************************
//
void            BVGL::bvglGenVertexArrays           ( GLsizei n, GLuint * arrays )
{
	bvgl.GenVertexArrays( n, arrays );
}

// *****************************
//
void            BVGL::bvglDeleteVertexArrays        ( GLsizei n, const GLuint * arrays )
{
	bvgl.DeleteVertexArrays( n, arrays );
}

// *****************************
//
void            BVGL::bvglEnableVertexAttribArray   ( GLuint index )
{
	bvgl.EnableVertexAttribArray( index );
}

// *****************************
//
void            BVGL::bvglDisableVertexAttribArray  ( GLuint index )
{
	bvgl.DisableVertexAttribArray( index );
}

// *****************************
//
void            BVGL::bvglBindVertexArray           ( GLuint array )
{
	bvgl.BindVertexArray( array );
}

// *****************************
//
void            BVGL::bvglVertexAttribPointer       ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
	bvgl.VertexAttribPointer( index, size, type, normalized, stride, pointer );
}


//pablito::

// ******************************
//
void			BVGL::bvglBindFramebufferEXT		(GLenum target, GLuint framebuffer)
{
	bvgl.BindFramebufferEXT(target,framebuffer);
}

// ******************************
//
void			BVGL::bvglBindRenderbufferEXT		(GLenum target, GLuint renderbuffer)
{
	bvgl.BindRenderbufferEXT(target,renderbuffer);
}

// ******************************
//
void			BVGL::bvglDeleteFramebuffersEXT	(GLsizei n, const GLuint *framebuffers)
{
	bvgl.DeleteFramebufferEXT(n,framebuffers);
}

// ******************************
//
void			BVGL::bvglDeleteRenderbuffersEXT	(GLsizei n, const GLuint *renderbuffers)
{
	bvgl.DeleteRenderbufferEXT(n,renderbuffers);
}

// ******************************
//
void			BVGL::bvglFlush	()
{
	bvgl.Flush();
}

// ******************************
//
void			BVGL::bvglFinish	()
{
	bvgl.Finish();
}

// ******************************
//
/*GLuint			BVGL::bvglGetError	()
{
	return bvgl.GetError();
}*/



// *****************************
//
void			BVGL::bvglVertexAttribDivisor		( GLuint index,  GLuint divisor )
{
	bvgl.VertexAttribDivisor( index, divisor );
}

//
// Images and textures
#ifdef BV_GL_VERSION_4_4

void BVGL::bvglBindImageTexture			( GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format )
{
	bvgl.BindImageTexture( unit, texture, level, layered, layer, access, format );
}


void BVGL::bvglBindImageTextures			( GLuint first, GLsizei count, const GLuint * textures )
{
	bvgl.BindImageTextures( first, count, textures );
}



void BVGL::bvglBindTextures				( GLuint first, GLsizei count, const GLuint* textures )
{
	bvgl.BindTextures( first, count, textures );
}

#endif

#ifdef BV_GL_VERSION_4_5

void BVGL::bvglBindTextureUnit				( GLuint unit, GLuint texture )
{
	bvgl.BindTextureUnit( unit, texture );
}

#endif

void BVGL::bvglCompressedTexImage1D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTexImage1D( target, level, internalFormat, width, border, imageSize, data );
}

void BVGL::bvglCompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTexImage2D( target, level, internalFormat, width, height, border, imageSize, data );
}

void BVGL::bvglCompressedTexImage3D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTexImage3D( target, level, internalFormat, width, height, depth, border, imageSize, data );
}

void BVGL::bvglCompressedTexSubImage1D		( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTexSubImage1D( target, level, xoffset, width, format, imageSize, data );
}

void BVGL::bvglCompressedTexSubImage2D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTexSubImage2D( target, level, xoffset, yoffset, width, height, format, imageSize, data );
}

void BVGL::bvglCompressedTexSubImage3D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data );
}

#ifdef BV_GL_VERSION_4_5

void BVGL::bvglCompressedTextureSubImage1D	( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTextureSubImage1D( texture, level, xoffset, width, format, imageSize, data );
}

void BVGL::bvglCompressedTextureSubImage2D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTextureSubImage2D( texture, level, xoffset, yoffset, width, height, format, imageSize, data );
}

void BVGL::bvglCompressedTextureSubImage3D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data )
{
	bvgl.CompressedTextureSubImage3D( texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data );
}

#endif

void BVGL::bvglTexImage1D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	bvgl.TexImage1D( target, level, internalFormat, width, border, format, type, data );
}

void BVGL::bvglTexImage3D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data )
{
	bvgl.TexImage3D( target, level, internalFormat, width, height, depth, border, format, type, data );
}

/**GL4.3*/
void BVGL::bvglTexImage2DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	bvgl.TexImage2DMultisample( target, samples, internalFormat, width, height, fixedSampleLocations );
}
/**GL4.3*/
void BVGL::bvglTexImage3DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	bvgl.TexImage3DMultisample( target, samples, internalFormat, width, height, depth, fixedSampleLocations );
}

/**GL4.2*/
void BVGL::bvglTexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width )
{
	bvgl.TexStorage1D( target, levels, internalFormat, width );
}

void BVGL::bvglTexStorage2D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height )
{
	bvgl.TexStorage2D( target, levels, internalFormat, width, height );
}

/**GL4.2*/
void BVGL::bvglTexStorage3D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth )
{
	bvgl.TexStorage3D( target, levels, internalFormat, width, height, depth );
}

/**GL4.3*/
void BVGL::bvglTexStorage2DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSasmpleLocations )
{
	bvgl.TexStorage2DMultisample( target, samples, internalFormat, width, height, fixedSasmpleLocations );
}

/**GL4.3*/
void BVGL::bvglTexStorage3DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSasmpleLocations )
{
	bvgl.TexStorage3DMultisample( target, samples, internalFormat, width, height, depth, fixedSasmpleLocations );
}


void BVGL::bvglTexSubImage1D				( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels )
{
	bvgl.TexSubImage1D( target, level, xoffset, width, format, type, pixels );
}

void BVGL::bvglTexSubImage3D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels )
{
	bvgl.TexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels );
}

#ifdef BV_GL_VERSION_4_5

void BVGL::bvglTextureStorage1D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width )
{
	bvgl.TextureStorage1D( texture, levels, internalFormat, width );
}

void BVGL::bvglTextureStorage2D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height )
{
	bvgl.TextureStorage2D( texture, levels, internalFormat, width, height );
}

void BVGL::bvglTextureStorage3D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth )
{
	bvgl.TextureStorage3D( texture, levels , internalFormat, width, height, depth );
}

void BVGL::bvglTextureStorage2DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations )
{
	bvgl.TextureStorage2DMultisample( texture, samples, internalFormat, width, height, fixedSampleLocations );
}

void BVGL::bvglTextureStorage3DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations )
{
	bvgl.TextureStorage3DMultisample( texture, samples, internalFormat, width, height, depth, fixedSampleLocations );
}


void BVGL::bvglTextureSubImage1D			( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels )
{
	bvgl.TextureSubImage1D( texture, level, xoffset, width, format, type, pixels );
}

void BVGL::bvglTextureSubImage2D			( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels )
{
	bvgl.TextureSubImage2D( texture, level, xoffset, yoffset, width, height, format, type, pixels );
}

void BVGL::bvglTextureSubImage3D			( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels )
{
	bvgl.TextureSubImage3D( texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels );
}

void BVGL::bvglTextureBuffer				( GLuint texture, GLenum internalFormat, GLuint buffer )
{
	bvgl.TextureBuffer( texture, internalFormat, buffer );
}

void BVGL::bvglTextureBufferRange			( GLuint texture, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	bvgl.TextureBufferRange( texture, internalFormat, buffer, offset, size );
}

#endif

void BVGL::bvglTexBuffer					( GLenum target, GLenum internalFormat, GLuint buffer )
{
	bvgl.TexBuffer( target, internalFormat, buffer );
}

/**GL4.3*/
void BVGL::bvglTexBufferRange				( GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	bvgl.TexBufferRange( target, internalFormat, buffer, offset, size );
}

/**GL4.3*/
void BVGL::bvglTextureView					( GLuint texture, GLenum target, GLuint origTexture, GLenum internalFormat, GLuint minLevel, GLuint numLevels, GLuint minLayer, GLuint numLayers )
{
	bvgl.TextureView( texture, target, origTexture, internalFormat, minLevel, numLevels, minLayer, numLayers );
}

#ifdef BV_GL_VERSION_4_5
void BVGL::bvglTextureParameteri			( GLuint texture, GLenum pname, GLint param )
{
	bvgl.TextureParameteri( texture, pname, param );
}

void BVGL::bvglTextureParameterfv			( GLuint texture, GLenum pname, const GLfloat * params )
{
	bvgl.TextureParameterfv( texture, pname, params );
}
#endif


void BVGL::bvglCopyTexSubImage1D			( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width )
{
	bvgl.CopyTexSubImage1D( target, level, xoffset, x, y, width );
}

void BVGL::bvglCopyTexSubImage2D			( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	bvgl.CopyTexSubImage2D( target, level, xoffset, yoffset, x, y, width, height );
}

void BVGL::bvglCopyTexSubImage3D			( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	bvgl.CopyTexSubImage3D( target, level, xoffset, yoffset, zoffset, x, y, width, height );
}

#ifdef BV_GL_VERSION_4_5
void BVGL::bvglCopyTextureSubImage1D		( GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width )
{
	bvgl.CopyTextureSubImage1D( texture, level, xoffset, x, y, width );
}

void BVGL::bvglCopyTextureSubImage2D		( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	bvgl.CopyTextureSubImage2D( texture, level, xoffset, yoffset, x, y, width, height );
}

void BVGL::bvglCopyTextureSubImage3D		( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
	bvgl.CopyTextureSubImage3D( texture, level, xoffset, yoffset, zoffset, x, y, width, height );
}
#endif

void BVGL::bvglCopyTexImage2D				( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border )
{
	bvgl.CopyTexImage2D( target, level, internalFormat, x, y, width, height, border );
}

void BVGL::bvglCopyTexImage1D				( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border )
{
	bvgl.CopyTexImage1D( target, level, internalFormat, x, y, width, border );
}

/**GL4.3*/
void BVGL::bvglCopyImageSubData			( GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth )
{
	bvgl.CopyImageSubData( srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth );
}

// Sampler

#ifdef BV_GL_VERSION_4_5
void BVGL::bvglCreateSamplers				( GLsizei n, GLuint* samplers )
{
	bvgl.CreateSamplers( n, samplers );
}
#endif

void BVGL::bvglGenSamplers					( GLsizei n, GLuint* samplers )
{
	bvgl.GenSamplers( n, samplers );
}

void BVGL::bvglDeleteSamplers				( GLsizei n, GLuint* samplers )
{
	bvgl.DeleteSamplers( n, samplers );
}


void BVGL::bvglBindSampler					( GLuint unit, GLuint sampler )
{
	bvgl.BindSampler( unit, sampler );
}

#ifdef BV_GL_VERSION_4_4
void BVGL::bvglBindSamplers				( GLuint first, GLsizei count, const GLuint* samplers )
{
	bvgl.BindSamplers( first, count, samplers );
}
#endif

void BVGL::bvglSamplerParameteri			( GLuint sampler, GLenum pname, GLint param )
{
	bvgl.SamplerParameteri( sampler, pname, param );
}

void BVGL::bvglSamplerParameterfv			( GLuint sampler, GLenum pname, const GLfloat* params )
{
	bvgl.SamplerParameterfv( sampler, pname, params );
}

void BVGL::bvglSamplerParameterf			( GLuint sampler, GLenum pname, GLfloat param )
{
	bvgl.SamplerParameterf( sampler, pname, param );
}

// Transform Feedback
/**GL4.0*/
void BVGL::bvglGenTransformFeedbacks		( GLsizei n, GLuint* ids )
{
	bvgl.GenTransformFeedbacks( n, ids );
}
/**GL4.0*/
void BVGL::bvglBindTransformFeedback		( GLenum target, GLuint id )
{
	bvgl.BindTransformFeedback( target, id );
}

void BVGL::bvglBeginTransformFeedback		( GLenum primitiveMode )
{
	bvgl.BeginTransformFeedback( primitiveMode );
}

void BVGL::bvglEndTransformFeedback		()
{
	bvgl.EndTransformFeedback();
}

/**GL4.0*/
void BVGL::bvglDrawTransformFeedback				( GLenum mode, GLuint id )
{
	bvgl.DrawTransformFeedback( mode, id );
}
/**GL4.2*/
void BVGL::bvglDrawTransformFeedbackInstanced		( GLenum mode, GLuint id, GLsizei primCount )
{
	bvgl.DrawTransformFeedbackInstanced( mode, id, primCount );
}

// Buffer
#ifdef BV_GL_VERSION_4_5
void BVGL::bvglNamedBufferData				( GLuint buffer, GLsizei size, const void* data, GLenum usage )
{
	bvgl.NamedBufferData( buffer, size, data, usage );
}

void BVGL::bvglNamedBufferStorage			( GLuint buffer, GLsizei size, const void* data, GLbitfield flags )
{
	bvgl.NamedBufferStorage( buffer, size, data, flags );
}

void BVGL::bvglNamedBufferSubData			( GLuint buffer, GLintptr offset, GLsizei size, const void* data )
{
	bvgl.NamedBufferSubData( buffer, offset, size, data );
}
#endif

#ifdef BV_GL_VERSION_4_4
void BVGL::bvglBufferStorage				( GLenum target, GLsizeiptr size, const GLvoid* data, GLbitfield flags )
{
	bvgl.BufferStorage( target, size, data, flags );
}
#endif

void BVGL::bvglBindBufferBase				( GLenum target, GLuint index, GLuint buffer )
{
	bvgl.BindBufferBase( target, index, buffer );
}

#ifdef BV_GL_VERSION_4_4
void BVGL::bvglBindBuffersBase				( GLenum target, GLuint first, GLsizei count, const GLuint* buffer )
{
	bvgl.BindBuffersBase( target, first, count, buffer );
}

void BVGL::bvglBindBuffersRange			( GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offset, const GLsizeiptr* sizes )
{
	bvgl.BindBuffersRange( target, first, count, buffers, offset, sizes );
}
#endif

void BVGL::bvglBindBufferRange				( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size )
{
	bvgl.BindBufferRange( target, index, buffer, offset, size );
}

/**GL4.3*/
void BVGL::bvglBindVertexBuffer			( GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizei stride )
{
	bvgl.BindVertexBuffer( bindingIndex, buffer, offset, stride );
}

#ifdef BV_GL_VERSION_4_4
void BVGL::bvglBindVertexBuffers			( GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides )
{
	bvgl.BindVertexBuffers( first, count, buffers, offsets, strides );
}
#endif

#ifdef BV_GL_VERSION_4_5

void BVGL::bvglVertexArrayVertexBuffers			( GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides )
{
	bvgl.VertexArrayVertexBuffers( vaobj, first, count, buffers, offsets, strides );
}



void* BVGL::bvglMapNamedBuffer				( GLuint buffer, GLenum access )
{
	return bvgl.MapNamedBuffer( buffer, access );
}

void* BVGL::bvglMapNamedBufferRange			( GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access )
{
	return bvgl.MapNamedBufferRange( buffer, offset, length, access );
}

GLboolean BVGL::bvglUnmapNamedBuffer			( GLuint buffer )
{
	return bvgl.UnmapNamedBuffer( buffer );
}
#endif

void* BVGL::bvglMapBufferRange				( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access )
{
	return bvgl.MapBufferRange( target, offset, length, access );
}


void BVGL::bvglCopyBufferSubData			( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size )
{
	bvgl.CopyBufferSubData( readBuffer, writeBuffer, readOffset, writeOffset, size );
}

#ifdef BV_GL_VERSION_4_5
void BVGL::bvglCopyNamedBufferSubData		( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size )
{
	bvgl.CopyNamedBufferSubData( readBuffer, writeBuffer, readOffset, writeOffset, size );
}
#endif
// Blending
void BVGL::bvglBlendEquation				( GLenum mode )
{
	bvgl.BlendEquation( mode );
}

/**GL4.0*/
void BVGL::bvglBlendEquationi				( GLuint buf, GLenum mode )
{
	bvgl.BlendEquationi( buf, mode );
}

void BVGL::bvglBlendEquationSeparate		( GLenum modeRGB, GLenum modeAlpha )
{
	bvgl.BlendEquationSeparate( modeRGB, modeAlpha );
}

/**GL4.0*/
void BVGL::bvglBlendEquationSeparatei		( GLuint buf, GLenum modeRGB, GLenum modeAlpha )
{
	bvgl.BlendEquationSeparatei( buf, modeRGB, modeAlpha );
}

/**GL4.0*/
void BVGL::bvglBlendFunci					( GLuint buf, GLenum sfactor, GLenum dfactor )
{
	bvgl.BlendFunci( buf, sfactor, dfactor );
}

void BVGL::bvglBlendFuncSeparate			( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )
{
	bvgl.BlendFuncSeparate( srcRGB, dstRGB, srcAlpha, dstAlpha );
}

/**GL4.0*/
void BVGL::bvglBlendFuncSeparatei			( GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )
{
	bvgl.BlendFuncSeparatei( buf, srcRGB, dstRGB, srcAlpha, dstAlpha );
}


// Framebuffer
void BVGL::bvglBlitFramebuffer				( GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter )
{
	bvgl.BlitFramebuffer( srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter );
}

#ifdef BV_GL_VERSION_4_5
void BVGL::bvglBlitNamedFramebuffer		( GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter )
{
	bvgl.BlitNamedFramebuffer( readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter );
}
#endif

	/// Wystarczy zestaw FramebufferTexture, FramebufferTextureLayer oraz FramebufferTexture2D ewentualnie ich wersje z DSA
void BVGL::bvglFramebufferTexture			( GLenum target, GLenum attachment, GLuint texture, GLint level )
{
	bvgl.FramebufferTexture( target, attachment, texture, level );
}

void BVGL::bvglFramebufferTextureLayer		( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer )
{
	bvgl.FramebufferTextureLayer( target, attachment, texture, level, layer );
}

#ifdef BV_GL_VERSION_4_5
void BVGL::bvglNamedFramebufferTexture		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level )
{
	bvgl.NamedFramebufferTexture( framebuffer, attachment, texture, level );
}


void BVGL::bvglNamedFramebufferTextureLayer			( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer )
{
	bvgl.NamedFramebufferTextureLayer( framebuffer, attachment, texture, level, layer );
}

void BVGL::bvglNamedFramebufferRenderbuffer			( GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
	bvgl.NamedFramebufferRenderbuffer( framebuffer, attachment, renderbuffertarget, renderbuffer );
}


GLenum BVGL::bvglCheckNamedFramebufferStatus	( GLuint framebuffer, GLenum target )
{
	return bvgl.CheckNamedFramebufferStatus( framebuffer, target );
}
#endif

void BVGL::bvglClearBufferiv				( GLenum buffer, GLint drawbuffer, const GLint* value )
{
	bvgl.ClearBufferiv( buffer, drawbuffer, value );
}

void BVGL::bvglClearBufferuiv				( GLenum buffer, GLint drawbuffer, const GLuint* value )
{
	bvgl.ClearBufferuiv( buffer, drawbuffer, value );
}

void BVGL::bvglClearBufferfv				( GLenum buffer, GLint drawbuffer, const GLfloat* value )
{
	bvgl.ClearBufferfv( buffer, drawbuffer, value );
}

void BVGL::bvglClearBufferfi				( GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil )
{
	bvgl.ClearBufferfi( buffer, drawbuffer, depth, stencil );
}

#ifdef BV_GL_VERSION_4_5

void BVGL::bvglClearNamedFramebufferiv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value )
{
	bvgl.ClearNamedFramebufferiv( framebuffer, buffer, drawbuffer, value );
}

void BVGL::bvglClearNamedFramebufferuiv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value )
{
	bvgl.ClearNamedFramebufferuiv( framebuffer, buffer, drawbuffer, value );
}

void BVGL::bvglClearNamedFramebufferfv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value )
{
	bvgl.ClearNamedFramebufferfv( framebuffer, buffer, drawbuffer, value );
}

void BVGL::bvglClearvNamedFramebufferfi		( GLuint framebuffer, GLenum buffer, GLfloat depth, GLint stencil )
{
	bvgl.ClearNamedFramebufferfi( framebuffer, buffer, depth, stencil );
}


void BVGL::bvglNamedFramebufferDrawBuffer	( GLuint framebuffer, GLenum buf )
{
	bvgl.NamedFramebufferDrawBuffer( framebuffer, buf );
}

void BVGL::bvglNamedFramebufferDrawBuffers	( GLuint framebuffer, GLsizei n, const GLenum* bufs )
{
	bvgl.NamedFramebufferDrawBuffers( framebuffer, n, bufs );
}

void BVGL::bvglNamedFramebufferReadBuffer	( GLuint framebuffer, GLenum mode )
{
	bvgl.NamedFramebufferReadBuffer( framebuffer, mode );
}

void BVGL::bvglNamedRenderbufferStorage		( GLuint renderbuffer, GLenum internalFormat, GLsizei width, GLsizei height )
{
	bvgl.NamedRenderbufferStorage( renderbuffer, internalFormat, width, height );
}

void BVGL::bvglNamedRenderbufferStorageMultisample	( GLuint renderbuffer, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height )
{
	bvgl.NamedRenderbufferStorageMultisample( renderbuffer, samples, internalFormat, width, height );
}

#endif

void BVGL::bvglRenderbufferStorageMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height )
{
	bvgl.RenderbufferStorageMultisample( target, samples, internalFormat, width, height );
}


// Drawing
void BVGL::bvglMultiDrawArrays						( GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawCount )
{
	bvgl.MultiDrawArrays( mode, first, count, drawCount );
}

void BVGL::bvglMultiDrawElements					( GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indicies, GLsizei drawCount )
{
	bvgl.MultiDrawElements( mode, count, type, indicies, drawCount );
}

void BVGL::bvglMultiDrawElementsBaseVertex			( GLenum mode, GLsizei* count, GLenum type, GLvoid** indicies, GLsizei drawcount, GLint* baseVertex )
{
	bvgl.MultiDrawElementsBaseVertex( mode, count, type, indicies, drawcount, baseVertex );
}


// Stencil
void BVGL::bvglClearStencil					( GLint s )
{
	bvgl.ClearStencil( s );
}


void BVGL::bvglStencilFunc					( GLenum func, GLint ref, GLuint mask )
{
	bvgl.StencilFunc( func, ref, mask );
}

void BVGL::bvglStencilFuncSeparate			( GLenum face, GLenum func, GLint ref, GLuint mask )
{
	bvgl.StencilFuncSeparate( face, func, ref, mask );
}

void BVGL::bvglStencilMask					( GLuint mask )
{
	bvgl.StencilMask( mask );
}

void BVGL::bvglStencilMaskSeparate			( GLenum face, GLuint mask )
{
	bvgl.StencilMaskSeparate( face, mask );
}

void BVGL::bvglStencilOp					( GLenum sfail, GLenum dpfail, GLenum dppass )
{
	bvgl.StencilOp( sfail, dpfail, dppass );
}

void BVGL::bvglStencilOpSeparate			( GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass )
{
	bvgl.StencilOpSeparate( face, sfail, dpfail, dppass );
}


} // bv
