#pragma once

#include <gl/glew.h>


namespace bv {

class BVGL
{
public:

    static void     glLinkProgram           ( GLuint program );
    static void     glUseProgram            ( GLuint program );
    static void     glValidateProgram       ( GLuint program );

    static void     glUniform1i             ( GLint location, GLint v );

    static void     glUniform1f             ( GLint location, GLfloat v );
    static void     glUniform2f             ( GLint location, GLfloat v, GLfloat v1 );
    static void     glUniform3f             ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 );
    static void     glUniform4f             ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 );

    static void     glUniformMatrix2fv      ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void     glUniformMatrix3fv      ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void     glUniformMatrix4fv      ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );

    static void     glShaderSource          ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length );
    static void     glCompileShader         ( GLuint shader );
    static void     glAttachShader          ( GLuint program, GLuint shader );

    static void     glGetProgramiv          ( GLuint program, GLenum pname, GLint * param );
    static void     glGetProgramInfoLog     ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );

    static void     glGetActiveUniform      ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint    glGetUniformLocation    ( GLuint program, const GLchar * name );

    static void     glGetActiveAttrib       ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint    glGetAttribLocation     ( GLuint program, const GLchar * name );

    static void     glGetShaderiv           ( GLuint shader, GLenum pname, GLint * param );
    static void     glGetShaderInfoLog      ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog );

    static void     glBindAttribLocation    ( GLuint program, GLuint index, const GLchar * name );
    static void     glBindFragDataLocation  ( GLuint program, GLuint colorNumber, const GLchar * name );

    static GLuint   glCreateProgram         ();
    static void     glDeleteProgram         ( GLuint program );

    static GLuint   glCreateShader          ( GLenum type );
    static void     glDeleteShader          ( GLuint shader );

};

} //bv
