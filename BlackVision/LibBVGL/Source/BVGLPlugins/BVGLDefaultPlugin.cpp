#include "BVGLDefaultPlugin.h"

#include <gl/glew.h>

// #include <cstdio>


namespace bv {

// *****************************
// FIXME: this initialization should check much more caps and return an error code instead of a simple bool
bool        BVGLDefaultPlugin::InitializeBVGL                ()
{
    glewInit();

    if ( !GLEW_ARB_vertex_program || !glewGetExtension( "GL_ARB_fragment_program" ) ) 
    {		
        return false;
    }

    return true;
}

// *****************************
//
void        BVGLDefaultPlugin::bvglViewport                  ( GLint x, GLint y, GLsizei width, GLsizei height )
{
    ::glViewport( x, y, width, height );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglClearColor                ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    ::glClearColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglClearDepth                ( GLclampd depth )
{
    ::glClearDepth( depth );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglClear                     ( GLbitfield mask )
{
    ::glClear( mask );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDepthRange                ( GLclampd zNear, GLclampd zFar )
{
    ::glDepthRange( zNear, zFar );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglLinkProgram               ( GLuint program )
{
    ::glLinkProgram( program );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUseProgram                ( GLuint program )
{
    ::glUseProgram( program );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglValidateProgram           ( GLuint program )
{
    ::glValidateProgram( program );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniform1i                 ( GLint location, GLint v )
{
    ::glUniform1i( location, v );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniform1f                 ( GLint location, GLfloat v )
{
    ::glUniform1f( location, v );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniform2f                 ( GLint location, GLfloat v, GLfloat v1 )
{
    ::glUniform2f( location, v, v1 );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniform3f                 ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 )
{
    ::glUniform3f( location, v, v1, v2 );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniform4f                 ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 )
{
    ::glUniform4f( location, v, v1, v2, v3 );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniformMatrix2fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix2fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniformMatrix3fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix3fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglUniformMatrix4fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix4fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglShaderSource              ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length )
{
    ::glShaderSource( shader, count, strings, length ); 
}

// *****************************
//
void        BVGLDefaultPlugin::bvglCompileShader             ( GLuint shader )
{
    ::glCompileShader( shader );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglAttachShader              ( GLuint program, GLuint shader )
{
    ::glAttachShader( program, shader );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetProgramiv              ( GLuint program, GLenum pname, GLint * param )
{
    ::glGetProgramiv( program, pname, param );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetProgramInfoLog         ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    ::glGetProgramInfoLog( program, bufSize, length, infoLog );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetActiveUniform          ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    ::glGetActiveUniform( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGLDefaultPlugin::bvglGetUniformLocation        ( GLuint program, const GLchar * name )
{
    return ::glGetUniformLocation( program, name );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetActiveAttrib           ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    ::glGetActiveAttrib( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGLDefaultPlugin::bvglGetAttribLocation         ( GLuint program, const GLchar * name )
{
    return ::glGetAttribLocation( program, name );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetShaderiv               ( GLuint shader, GLenum pname, GLint * param )
{
    ::glGetShaderiv( shader, pname, param );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetShaderInfoLog          ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    ::glGetShaderInfoLog( shader, bufSize, length, infoLog );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetIntegerv               ( GLenum pname, GLint * params )
{
    ::glGetIntegerv( pname, params );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGetDoublev                ( GLenum pname, GLdouble * params )
{
    glGetDoublev( pname, params );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglBindAttribLocation        ( GLuint program, GLuint index, const GLchar * name )
{
    ::glBindAttribLocation( program, index, name );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglBindFragDataLocation      ( GLuint program, GLuint colorNumber, const GLchar * name )
{
    ::glBindFragDataLocation( program, colorNumber, name );
}

// *****************************
//
GLuint      BVGLDefaultPlugin::bvglCreateProgram             ()
{
    return ::glCreateProgram();
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDeleteProgram             ( GLuint program )
{
    ::glDeleteProgram( program );
}

// *****************************
//
GLuint      BVGLDefaultPlugin::bvglCreateShader              ( GLenum type )
{
    return ::glCreateShader( type );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDeleteShader              ( GLuint shader )
{
    ::glDeleteShader( shader );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglGenBuffers                ( GLsizei n, GLuint * buffers )
{
    ::glGenBuffers( n, buffers );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDeleteBuffers             ( GLsizei n, const GLuint * buffers )
{
    ::glDeleteBuffers( n, buffers );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglBindBuffer                ( GLenum target, GLuint buffer )
{
    ::glBindBuffer( target, buffer );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglBufferData                ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    ::glBufferData( target, size, data, usage );
}

// *****************************
//
GLvoid *    BVGLDefaultPlugin::bvglMapBuffer                 ( GLenum target, GLenum access )
{
    return ::glMapBuffer( target, access );
}

// *****************************
//
GLboolean    BVGLDefaultPlugin::bvglUnmapBuffer              ( GLenum target )
{
    return ::glUnmapBuffer( target );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglGenTextures              ( GLsizei n, GLuint * textures )
{
    ::glGenTextures( n, textures );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglDeleteTextures           ( GLsizei n, const GLuint * textures )
{
    ::glDeleteTextures( n, textures );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglTexImage2D               ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels )
{
    ::glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglActiveTexture            ( GLenum texture )
{
    ::glActiveTexture( texture );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglTexSubImage2D            ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels )
{
    ::glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglBindTexture              ( GLenum target, GLuint texture )
{
    ::glBindTexture( target, texture );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglTexParameteri            ( GLenum target, GLenum pname, GLint param )
{
    ::glTexParameteri( target, pname, param );
}

// *****************************
//
void            BVGLDefaultPlugin::bvglTexParameterfv        ( GLenum target, GLenum pname, const GLfloat * params )
{
    ::glTexParameterfv( target, pname, params );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglReadBuffer               ( GLenum mode )
{
    ::glReadBuffer( mode );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglReadPixels               ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels )
{
    ::glReadPixels( x, y, width, height, format, type, pixels );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglGenFramebuffers          ( GLsizei n, GLuint * framebuffers )
{
    ::glGenFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglDeleteFramebuffers       ( GLsizei n, const GLuint * framebuffers )
{
    ::glDeleteFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglGenRenderbuffers         ( GLsizei n, GLuint * renderbuffers )
{
    ::glGenRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglDeleteRenderbuffers      ( GLsizei n, const GLuint * renderbuffers )
{
    ::glDeleteRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglBindRenderbuffer         ( GLenum target, GLuint renderbuffer )
{
    ::glBindRenderbuffer( target, renderbuffer );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglRenderbufferStorage      ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
{
    ::glRenderbufferStorage( target, internalformat, width, height );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglBindFramebuffer          ( GLenum target, GLuint framebuffer )
{
    ::glBindFramebuffer( target, framebuffer );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglFramebufferTexture2D     ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
    ::glFramebufferTexture2D( target, attachment, textarget, texture, level );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglFramebufferRenderbuffer  ( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
    ::glFramebufferRenderbuffer( target, attachment, renderbuffertarget, renderbuffer );
}

// *****************************
//
void         BVGLDefaultPlugin::bvglDrawBuffers              ( GLsizei n, const GLenum * bufs )
{
    ::glDrawBuffers( n, bufs );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDrawArrays                ( GLenum mode, GLint first, GLsizei count )
{
    ::glDrawArrays( mode, first, count );
}

// *****************************
//
GLenum      BVGLDefaultPlugin::bvglCheckFramebufferStatus    ( GLenum target )
{
    return ::glCheckFramebufferStatus( target );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglEnable                    ( GLenum cap )
{
    ::glEnable( cap );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDisable                   ( GLenum cap )
{
    ::glDisable( cap );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDepthFunc                 ( GLenum func )
{
    ::glDepthFunc( func );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglDepthMask                 ( GLboolean flag )
{
    ::glDepthMask( flag );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglBlendFunc                 ( GLenum sfactor, GLenum dfactor )
{
    ::glBlendFunc( sfactor, dfactor );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglBlendColor                ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    ::glBlendColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglFrontFace                 ( GLenum mode )
{
    ::glFrontFace( mode );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglCullFace                  ( GLenum mode )
{
    ::glCullFace( mode );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglPolygonMode               ( GLenum face, GLenum mode )
{
    ::glPolygonMode( face, mode );
}

// *****************************
//
void        BVGLDefaultPlugin::bvglPolygonOffset             ( GLfloat factor, GLfloat units )
{
    ::glPolygonOffset( factor, units );
}

// *****************************
//
GLenum      BVGLDefaultPlugin::bvglGetError                  ()
{
    return ::glGetError();
}

// *****************************
//
const GLubyte * BVGLDefaultPlugin::bvglGetString             ( GLenum name )
{
    return ::glGetString( name );
}

// *****************************
//
const GLubyte * BVGLDefaultPlugin::bvglGetStringi            ( GLenum name, GLuint index )
{
    return ::glGetStringi( name, index );
}

// *****************************
//
void            BVGLDefaultPlugin::bvglGenVertexArrays           ( GLsizei n, GLuint * arrays )
{
    ::glGenVertexArrays( n, arrays );
}

// *****************************
//
void            BVGLDefaultPlugin::bvglDeleteVertexArrays        ( GLsizei n, const GLuint * arrays )
{
    ::glDeleteVertexArrays( n, arrays );
}

// *****************************
//
void            BVGLDefaultPlugin::bvglEnableVertexAttribArray   ( GLuint index )
{
    ::glEnableVertexAttribArray( index );
}

// *****************************
//
void            BVGLDefaultPlugin::bvglDisableVertexAttribArray  ( GLuint index )
{
    ::glDisableVertexAttribArray( index );
}

// *****************************
//
void            BVGLDefaultPlugin::bvglBindVertexArray           ( GLuint array )
{
    ::glBindVertexArray( array );
}

// *****************************
//
void            BVGLDefaultPlugin::bvglVertexAttribPointer       ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    ::glVertexAttribPointer( index, size, type, normalized, stride, pointer );
}

} //bv
