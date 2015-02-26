#pragma once

#include <cstddef>

// FIXME: remove later
// FIXME: Some temporary defines for the time of transition - to make sure that there is no explicit glew.h dependency
#include "BVGLDefs.h"

namespace bv {

class BVGL
{
public:

    static void         bvglViewport                ( GLint x, GLint y, GLsizei width, GLsizei height );

    static void         bvglDepthRange              ( GLclampd zNear, GLclampd zFar );

    static void         bvglLinkProgram             ( GLuint program );
    static void         bvglUseProgram              ( GLuint program );
    static void         bvglValidateProgram         ( GLuint program );
                        
    static void         bvglUniform1i               ( GLint location, GLint v );
                        
    static void         bvglUniform1f               ( GLint location, GLfloat v );
    static void         bvglUniform2f               ( GLint location, GLfloat v, GLfloat v1 );
    static void         bvglUniform3f               ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 );
    static void         bvglUniform4f               ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 );
                        
    static void         bvglUniformMatrix2fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void         bvglUniformMatrix3fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void         bvglUniformMatrix4fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
                        
    static void         bvglShaderSource            ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length );
    static void         bvglCompileShader           ( GLuint shader );
    static void         bvglAttachShader            ( GLuint program, GLuint shader );
                        
    static void         bvglGetProgramiv            ( GLuint program, GLenum pname, GLint * param );
    static void         bvglGetProgramInfoLog       ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
                        
    static void         bvglGetActiveUniform        ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint        bvglGetUniformLocation      ( GLuint program, const GLchar * name );
                        
    static void         bvglGetActiveAttrib         ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint        bvglGetAttribLocation       ( GLuint program, const GLchar * name );
                        
    static void         bvglGetShaderiv             ( GLuint shader, GLenum pname, GLint * param );
    static void         bvglGetShaderInfoLog        ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
                        
    static void         bvglGetIntegerv             ( GLenum pname, GLint * params );
    static void         bvglGetDoublev              ( GLenum pname, GLdouble * params );

    static void         bvglBindAttribLocation      ( GLuint program, GLuint index, const GLchar * name );
    static void         bvglBindFragDataLocation    ( GLuint program, GLuint colorNumber, const GLchar * name );
                        
    static GLuint       bvglCreateProgram           ();
    static void         bvglDeleteProgram           ( GLuint program );
                        
    static GLuint       bvglCreateShader            ( GLenum type );
    static void         bvglDeleteShader            ( GLuint shader );

    static void         bvglGenBuffers              ( GLsizei n, GLuint * buffers );
    static void         bvglDeleteBuffers           ( GLsizei n, const GLuint * buffers );

    static void         bvglBindBuffer              ( GLenum target, GLuint buffer );
    static void         bvglBufferData              ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );

    static GLvoid *     bvglMapBuffer               ( GLenum target, GLenum access );
    static GLboolean    bvglUnmapBuffer             ( GLenum target );

    static void         bvglGenTextures             ( GLsizei n, GLuint * textures );
    static void         bvglDeleteTextures          ( GLsizei n, const GLuint * textures );

    static void         bvglTexImage2D              ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels );
    static void         bvglActiveTexture           ( GLenum texture );

    static void         bvglTexSubImage2D           ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels );
    static void         bvglBindTexture             ( GLenum target, GLuint texture );
    static void         bvglTexParameteri           ( GLenum target, GLenum pname, GLint param );

    static void         bvglReadBuffer              ( GLenum mode );
    static void         bvglReadPixels              ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );

    static void         bvglGenFramebuffers         ( GLsizei n, GLuint * framebuffers );
    static void         bvglDeleteFramebuffers      ( GLsizei n, const GLuint * framebuffers );
    
    static void         bvglGenRenderbuffers        ( GLsizei n, GLuint * renderbuffers );
    static void         bvglDeleteRenderbuffers     ( GLsizei n, const GLuint * renderbuffers );

    static void         bvglBindRenderbuffer        ( GLenum target, GLuint renderbuffer );
    static void         bvglRenderbufferStorage     ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );

    static void         bvglBindFramebuffer         ( GLenum target, GLuint framebuffer );
    static void         bvglFramebufferTexture2D    ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level ) ;
    static void         bvglFramebufferRenderbuffer ( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

    static void         bvglDrawBuffers             ( GLsizei n, const GLenum * bufs );

    static GLenum       bvglCheckFramebufferStatus  ( GLenum target );

};

} //bv
