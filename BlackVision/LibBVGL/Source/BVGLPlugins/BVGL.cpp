#include "BVGL.h"

#include <gl/glew.h>

// #include <cstdio>


namespace bv {

// *****************************
// FIXME: this initialization should check much more caps and return an error code instead of a simple bool
bool        BVGL::InitializeBVGL                ()
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
void        BVGL::bvglViewport                  ( GLint x, GLint y, GLsizei width, GLsizei height )
{
    ::glViewport( x, y, width, height );
}

// *****************************
//
void        BVGL::bvglClearColor                ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    ::glClearColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGL::bvglClearDepth                ( GLclampd depth )
{
    ::glClearDepth( depth );
}

// *****************************
//
void        BVGL::bvglClear                     ( GLbitfield mask )
{
    ::glClear( mask );
}

// *****************************
//
void        BVGL::bvglDepthRange                ( GLclampd zNear, GLclampd zFar )
{
    ::glDepthRange( zNear, zFar );
}

// *****************************
//
void        BVGL::bvglLinkProgram               ( GLuint program )
{
    ::glLinkProgram( program );
}

// *****************************
//
void        BVGL::bvglUseProgram                ( GLuint program )
{
    ::glUseProgram( program );
}

// *****************************
//
void        BVGL::bvglValidateProgram           ( GLuint program )
{
    ::glValidateProgram( program );
}

// *****************************
//
void        BVGL::bvglUniform1i                 ( GLint location, GLint v )
{
    ::glUniform1i( location, v );
}

// *****************************
//
void        BVGL::bvglUniform1f                 ( GLint location, GLfloat v )
{
    ::glUniform1f( location, v );
}

// *****************************
//
void        BVGL::bvglUniform2f                 ( GLint location, GLfloat v, GLfloat v1 )
{
    ::glUniform2f( location, v, v1 );
}

// *****************************
//
void        BVGL::bvglUniform3f                 ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 )
{
    ::glUniform3f( location, v, v1, v2 );
}

// *****************************
//
void        BVGL::bvglUniform4f                 ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 )
{
    ::glUniform4f( location, v, v1, v2, v3 );
}

// *****************************
//
void        BVGL::bvglUniformMatrix2fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix2fv( location, count, transpose, value );
}

// *****************************
//
void        BVGL::bvglUniformMatrix3fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix3fv( location, count, transpose, value );
}

// *****************************
//
void        BVGL::bvglUniformMatrix4fv          ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix4fv( location, count, transpose, value );
}

// *****************************
//
void        BVGL::bvglShaderSource              ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length )
{
    ::glShaderSource( shader, count, strings, length ); 
}

// *****************************
//
void        BVGL::bvglCompileShader             ( GLuint shader )
{
    ::glCompileShader( shader );
}

// *****************************
//
void        BVGL::bvglAttachShader              ( GLuint program, GLuint shader )
{
    ::glAttachShader( program, shader );
}

// *****************************
//
void        BVGL::bvglGetProgramiv              ( GLuint program, GLenum pname, GLint * param )
{
    ::glGetProgramiv( program, pname, param );
}

// *****************************
//
void        BVGL::bvglGetProgramInfoLog         ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    ::glGetProgramInfoLog( program, bufSize, length, infoLog );
}

// *****************************
//
void        BVGL::bvglGetActiveUniform          ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    ::glGetActiveUniform( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGL::bvglGetUniformLocation        ( GLuint program, const GLchar * name )
{
    return ::glGetUniformLocation( program, name );
}

// *****************************
//
void        BVGL::bvglGetActiveAttrib           ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    ::glGetActiveAttrib( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGL::bvglGetAttribLocation         ( GLuint program, const GLchar * name )
{
    return ::glGetAttribLocation( program, name );
}

// *****************************
//
void        BVGL::bvglGetShaderiv               ( GLuint shader, GLenum pname, GLint * param )
{
    ::glGetShaderiv( shader, pname, param );
}

// *****************************
//
void        BVGL::bvglGetShaderInfoLog          ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    ::glGetShaderInfoLog( shader, bufSize, length, infoLog );
}

// *****************************
//
void        BVGL::bvglGetIntegerv               ( GLenum pname, GLint * params )
{
    ::glGetIntegerv( pname, params );
}

// *****************************
//
void        BVGL::bvglGetDoublev                ( GLenum pname, GLdouble * params )
{
    glGetDoublev( pname, params );
}

// *****************************
//
void        BVGL::bvglBindAttribLocation        ( GLuint program, GLuint index, const GLchar * name )
{
    ::glBindAttribLocation( program, index, name );
}

// *****************************
//
void        BVGL::bvglBindFragDataLocation      ( GLuint program, GLuint colorNumber, const GLchar * name )
{
    ::glBindFragDataLocation( program, colorNumber, name );
}

// *****************************
//
GLuint      BVGL::bvglCreateProgram             ()
{
    return ::glCreateProgram();
}

// *****************************
//
void        BVGL::bvglDeleteProgram             ( GLuint program )
{
    ::glDeleteProgram( program );
}

// *****************************
//
GLuint      BVGL::bvglCreateShader              ( GLenum type )
{
    return ::glCreateShader( type );
}

// *****************************
//
void        BVGL::bvglDeleteShader              ( GLuint shader )
{
    ::glDeleteShader( shader );
}

// *****************************
//
void        BVGL::bvglGenBuffers                ( GLsizei n, GLuint * buffers )
{
    ::glGenBuffers( n, buffers );
}

// *****************************
//
void        BVGL::bvglDeleteBuffers             ( GLsizei n, const GLuint * buffers )
{
    ::glDeleteBuffers( n, buffers );
}

// *****************************
//
void        BVGL::bvglBindBuffer                ( GLenum target, GLuint buffer )
{
    ::glBindBuffer( target, buffer );
}

// *****************************
//
void        BVGL::bvglBufferData                ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    ::glBufferData( target, size, data, usage );
}

// *****************************
//
GLvoid *    BVGL::bvglMapBuffer                 ( GLenum target, GLenum access )
{
    return ::glMapBuffer( target, access );
}

// *****************************
//
GLboolean    BVGL::bvglUnmapBuffer              ( GLenum target )
{
    return ::glUnmapBuffer( target );
}

// *****************************
//
void         BVGL::bvglGenTextures              ( GLsizei n, GLuint * textures )
{
    ::glGenTextures( n, textures );
}

// *****************************
//
void         BVGL::bvglDeleteTextures           ( GLsizei n, const GLuint * textures )
{
    ::glDeleteTextures( n, textures );
}

// *****************************
//
void         BVGL::bvglTexImage2D               ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels )
{
    ::glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

// *****************************
//
void         BVGL::bvglActiveTexture            ( GLenum texture )
{
    ::glActiveTexture( texture );
}

// *****************************
//
void         BVGL::bvglTexSubImage2D            ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels )
{
    ::glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

// *****************************
//
void         BVGL::bvglBindTexture              ( GLenum target, GLuint texture )
{
    ::glBindTexture( target, texture );
}

// *****************************
//
void         BVGL::bvglTexParameteri            ( GLenum target, GLenum pname, GLint param )
{
    ::glTexParameteri( target, pname, param );
}

// *****************************
//
void            BVGL::bvglTexParameterfv        ( GLenum target, GLenum pname, const GLfloat * params )
{
    ::glTexParameterfv( target, pname, params );
}

// *****************************
//
void         BVGL::bvglReadBuffer               ( GLenum mode )
{
    ::glReadBuffer( mode );
}

// *****************************
//
void         BVGL::bvglReadPixels               ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels )
{
    ::glReadPixels( x, y, width, height, format, type, pixels );
}

// *****************************
//
void         BVGL::bvglGenFramebuffers          ( GLsizei n, GLuint * framebuffers )
{
    ::glGenFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGL::bvglDeleteFramebuffers       ( GLsizei n, const GLuint * framebuffers )
{
    ::glDeleteFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGL::bvglGenRenderbuffers         ( GLsizei n, GLuint * renderbuffers )
{
    ::glGenRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGL::bvglDeleteRenderbuffers      ( GLsizei n, const GLuint * renderbuffers )
{
    ::glDeleteRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGL::bvglBindRenderbuffer         ( GLenum target, GLuint renderbuffer )
{
    ::glBindRenderbuffer( target, renderbuffer );
}

// *****************************
//
void         BVGL::bvglRenderbufferStorage      ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
{
    ::glRenderbufferStorage( target, internalformat, width, height );
}

// *****************************
//
void         BVGL::bvglBindFramebuffer          ( GLenum target, GLuint framebuffer )
{
    ::glBindFramebuffer( target, framebuffer );
}

// *****************************
//
void         BVGL::bvglFramebufferTexture2D     ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
    ::glFramebufferTexture2D( target, attachment, textarget, texture, level );
}

// *****************************
//
void         BVGL::bvglFramebufferRenderbuffer  ( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
    ::glFramebufferRenderbuffer( target, attachment, renderbuffertarget, renderbuffer );
}

// *****************************
//
void         BVGL::bvglDrawBuffers              ( GLsizei n, const GLenum * bufs )
{
    ::glDrawBuffers( n, bufs );
}

// *****************************
//
void        BVGL::bvglDrawArrays                ( GLenum mode, GLint first, GLsizei count )
{
    ::glDrawArrays( mode, first, count );
}

// *****************************
//
GLenum      BVGL::bvglCheckFramebufferStatus    ( GLenum target )
{
    return ::glCheckFramebufferStatus( target );
}

// *****************************
//
void        BVGL::bvglEnable                    ( GLenum cap )
{
    ::glEnable( cap );
}

// *****************************
//
void        BVGL::bvglDisable                   ( GLenum cap )
{
    ::glDisable( cap );
}

// *****************************
//
void        BVGL::bvglDepthFunc                 ( GLenum func )
{
    ::glDepthFunc( func );
}

// *****************************
//
void        BVGL::bvglDepthMask                 ( GLboolean flag )
{
    ::glDepthMask( flag );
}

// *****************************
//
void        BVGL::bvglBlendFunc                 ( GLenum sfactor, GLenum dfactor )
{
    ::glBlendFunc( sfactor, dfactor );
}

// *****************************
//
void        BVGL::bvglBlendColor                ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    ::glBlendColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGL::bvglFrontFace                 ( GLenum mode )
{
    ::glFrontFace( mode );
}

// *****************************
//
void        BVGL::bvglCullFace                  ( GLenum mode )
{
    ::glCullFace( mode );
}

// *****************************
//
void        BVGL::bvglPolygonMode               ( GLenum face, GLenum mode )
{
    ::glPolygonMode( face, mode );
}

// *****************************
//
void        BVGL::bvglPolygonOffset             ( GLfloat factor, GLfloat units )
{
    ::glPolygonOffset( factor, units );
}

// *****************************
//
GLenum      BVGL::bvglGetError                  ()
{
    return ::glGetError();
}

// *****************************
//
const GLubyte * BVGL::bvglGetString             ( GLenum name )
{
    return ::glGetString( name );
}

// *****************************
//
const GLubyte * BVGL::bvglGetStringi            ( GLenum name, GLuint index )
{
    return ::glGetStringi( name, index );
}

// *****************************
//
void            BVGL::bvglGenVertexArrays           ( GLsizei n, GLuint * arrays )
{
    ::glGenVertexArrays( n, arrays );
}

// *****************************
//
void            BVGL::bvglDeleteVertexArrays        ( GLsizei n, const GLuint * arrays )
{
    ::glDeleteVertexArrays( n, arrays );
}

// *****************************
//
void            BVGL::bvglEnableVertexAttribArray   ( GLuint index )
{
    ::glEnableVertexAttribArray( index );
}

// *****************************
//
void            BVGL::bvglDisableVertexAttribArray  ( GLuint index )
{
    ::glDisableVertexAttribArray( index );
}

// *****************************
//
void            BVGL::bvglBindVertexArray           ( GLuint array )
{
    ::glBindVertexArray( array );
}

// *****************************
//
void            BVGL::bvglVertexAttribPointer       ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    ::glVertexAttribPointer( index, size, type, normalized, stride, pointer );
}

} //bv
