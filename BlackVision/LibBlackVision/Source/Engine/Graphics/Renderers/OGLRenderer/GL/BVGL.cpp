#include "BVGL.h"

// #include <cstdio>


namespace bv {

// *****************************
//
void    BVGL::glLinkProgram             ( GLuint program )
{
    ::glLinkProgram( program );
}

// *****************************
//
void    BVGL::glUseProgram              ( GLuint program )
{
    ::glUseProgram( program );
}

// *****************************
//
void    BVGL::glValidateProgram         ( GLuint program )
{
    ::glValidateProgram( program );
}

// *****************************
//
void    BVGL::glUniform1i             ( GLint location, GLint v )
{
    ::glUniform1i( location, v );
}

// *****************************
//
void    BVGL::glUniform1f             ( GLint location, GLfloat v )
{
    ::glUniform1f( location, v );
}

// *****************************
//
void    BVGL::glUniform2f             ( GLint location, GLfloat v, GLfloat v1 )
{
    ::glUniform2f( location, v, v1 );
}

// *****************************
//
void    BVGL::glUniform3f             ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 )
{
    ::glUniform3f( location, v, v1, v2 );
}

// *****************************
//
void    BVGL::glUniform4f             ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 )
{
    ::glUniform4f( location, v, v1, v2, v3 );
}

// *****************************
//
void    BVGL::glUniformMatrix2fv      ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix2fv( location, count, transpose, value );
}

// *****************************
//
void    BVGL::glUniformMatrix3fv      ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix3fv( location, count, transpose, value );
}

// *****************************
//
void    BVGL::glUniformMatrix4fv      ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    ::glUniformMatrix4fv( location, count, transpose, value );
}

// *****************************
//
void    BVGL::glShaderSource            ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length )
{
    ::glShaderSource( shader, count, strings, length ); 
}

// *****************************
//
void    BVGL::glCompileShader           ( GLuint shader )
{
    ::glCompileShader( shader );
}

// *****************************
//
void    BVGL::glAttachShader            ( GLuint program, GLuint shader )
{
    ::glAttachShader( program, shader );
}

// *****************************
//
void    BVGL::glGetProgramiv            ( GLuint program, GLenum pname, GLint * param )
{
    ::glGetProgramiv( program, pname, param );
}

// *****************************
//
void    BVGL::glGetProgramInfoLog       ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    ::glGetProgramInfoLog( program, bufSize, length, infoLog );
}

// *****************************
//
void    BVGL::glGetActiveUniform        ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    ::glGetActiveUniform( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint   BVGL::glGetUniformLocation      ( GLuint program, const GLchar * name )
{
    return ::glGetUniformLocation( program, name );
}

// *****************************
//
void    BVGL::glGetActiveAttrib       ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    ::glGetActiveAttrib( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint   BVGL::glGetAttribLocation     ( GLuint program, const GLchar * name )
{
    return ::glGetAttribLocation( program, name );
}

// *****************************
//
void    BVGL::glGetShaderiv             ( GLuint shader, GLenum pname, GLint * param )
{
    ::glGetShaderiv( shader, pname, param );
}

// *****************************
//
void    BVGL::glGetShaderInfoLog        ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    ::glGetShaderInfoLog( shader, bufSize, length, infoLog );
}

// *****************************
//
void    BVGL::glBindAttribLocation      ( GLuint program, GLuint index, const GLchar * name )
{
    ::glBindAttribLocation( program, index, name );
}

// *****************************
//
void    BVGL::glBindFragDataLocation    ( GLuint program, GLuint colorNumber, const GLchar * name )
{
    ::glBindFragDataLocation( program, colorNumber, name );
}

// *****************************
//
GLuint  BVGL::glCreateProgram ()
{
    return ::glCreateProgram();
}

// *****************************
//
void    BVGL::glDeleteProgram           ( GLuint program )
{
    ::glDeleteProgram( program );
}

// *****************************
//
GLuint  BVGL::glCreateShader            ( GLenum type )
{
    return ::glCreateShader( type );
}

// *****************************
//
void    BVGL::glDeleteShader            ( GLuint shader )
{
    ::glDeleteShader( shader );
}

} //bv
