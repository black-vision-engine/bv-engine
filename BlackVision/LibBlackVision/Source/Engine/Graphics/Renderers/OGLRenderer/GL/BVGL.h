#pragma once

#include <cstddef>


namespace bv {

// FIXME: remove later
// FIXME: Some temporary defines for the time of transition - to make sure that there is no explicit glew.h dependency
#define GL_FALSE 0

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_LINK_STATUS 0x8B82
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_VALIDATE_STATUS 0x8B83
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef char GLchar;

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
// FIXME: remove later

class BVGL
{
public:

    static void         bvglLinkProgram         ( GLuint program );
    static void         bvglUseProgram          ( GLuint program );
    static void         bvglValidateProgram     ( GLuint program );
                        
    static void         bvglUniform1i           ( GLint location, GLint v );
                        
    static void         bvglUniform1f           ( GLint location, GLfloat v );
    static void         bvglUniform2f           ( GLint location, GLfloat v, GLfloat v1 );
    static void         bvglUniform3f           ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 );
    static void         bvglUniform4f           ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 );
                        
    static void         bvglUniformMatrix2fv    ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void         bvglUniformMatrix3fv    ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void         bvglUniformMatrix4fv    ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
                        
    static void         bvglShaderSource        ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length );
    static void         bvglCompileShader       ( GLuint shader );
    static void         bvglAttachShader        ( GLuint program, GLuint shader );
                        
    static void         bvglGetProgramiv        ( GLuint program, GLenum pname, GLint * param );
    static void         bvglGetProgramInfoLog   ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
                        
    static void         bvglGetActiveUniform    ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint        bvglGetUniformLocation  ( GLuint program, const GLchar * name );
                        
    static void         bvglGetActiveAttrib     ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint        bvglGetAttribLocation   ( GLuint program, const GLchar * name );
                        
    static void         bvglGetShaderiv         ( GLuint shader, GLenum pname, GLint * param );
    static void         bvglGetShaderInfoLog    ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
                        
    static void         bvglGetIntegerv         ( GLenum pname, GLint * params );

    static void         bvglBindAttribLocation  ( GLuint program, GLuint index, const GLchar * name );
    static void         bvglBindFragDataLocation( GLuint program, GLuint colorNumber, const GLchar * name );
                        
    static GLuint       bvglCreateProgram       ();
    static void         bvglDeleteProgram       ( GLuint program );
                        
    static GLuint       bvglCreateShader        ( GLenum type );
    static void         bvglDeleteShader        ( GLuint shader );

    static void         bvglGenBuffers          ( GLsizei n, GLuint * buffers );
    static void         bvglDeleteBuffers       ( GLsizei n, const GLuint * buffers );

    static void         bvglBindBuffer          ( GLenum target, GLuint buffer );
    static void         bvglBufferData          ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );

    static GLvoid *     bvglMapBuffer           ( GLenum target, GLenum access );
    static GLboolean    bvglUnmapBuffer         ( GLenum target );

    static void         bvglTexSubImage2D       ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels );
    static void         bvglBindTexture         ( GLenum target, GLuint texture );

    static void         bvglReadBuffer          ( GLenum mode );
    static void         bvglReadPixels          ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );

};

} //bv
