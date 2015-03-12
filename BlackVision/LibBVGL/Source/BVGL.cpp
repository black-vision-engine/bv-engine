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

} // bv
