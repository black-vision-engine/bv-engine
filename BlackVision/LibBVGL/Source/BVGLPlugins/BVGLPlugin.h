#pragma once

#include <cstddef>

// FIXME: remove later. Some temporary defines for the time of transition - to make sure that there is no explicit glew.h dependency
#include "BVGLDefs.h"


namespace bv {

class BVGLPlugin
{
public:

    virtual bool				InitializeBVGL				();

    virtual void                Viewport					( GLint x, GLint y, GLsizei width, GLsizei height );
    
    virtual void                ClearColor					( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
    virtual void                ClearDepth					( GLclampd depth );
    virtual void                Clear						( GLbitfield mask );
    
    virtual void                DepthRange					( GLclampd zNear, GLclampd zFar );

    virtual void                LinkProgram					( GLuint program );
    virtual void                UseProgram					( GLuint program );
    virtual void                ValidateProgram				( GLuint program );
                                
    virtual void                Uniform1i					( GLint location, GLint v );
                                
    virtual void                Uniform1f					( GLint location, GLfloat v );
    virtual void                Uniform2f					( GLint location, GLfloat v, GLfloat v1 );
    virtual void                Uniform3f					( GLint location, GLfloat v, GLfloat v1, GLfloat v2 );
    virtual void                Uniform4f					( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 );
                                
    virtual void                UniformMatrix2fv			( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    virtual void                UniformMatrix3fv			( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    virtual void                UniformMatrix4fv			( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
                                
    virtual void                ShaderSource				( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length );
    virtual void                CompileShader				( GLuint shader );
    virtual void                AttachShader				( GLuint program, GLuint shader );
                                
    virtual void                GetProgramiv				( GLuint program, GLenum pname, GLint * param );
    virtual void                GetProgramInfoLog			( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
                                
    virtual void                GetActiveUniform			( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    virtual GLint               GetUniformLocation			( GLuint program, const GLchar * name );
                                
    virtual void                GetActiveAttrib				( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    virtual GLint               GetAttribLocation			( GLuint program, const GLchar * name );
                                
    virtual void                GetShaderiv					( GLuint shader, GLenum pname, GLint * param );
    virtual void                GetShaderInfoLog			( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
                                
    virtual void                GetIntegerv					( GLenum pname, GLint * params );
    virtual void                GetDoublev					( GLenum pname, GLdouble * params );

    virtual void                BindAttribLocation			( GLuint program, GLuint index, const GLchar * name );
    virtual void                BindFragDataLocation		( GLuint program, GLuint colorNumber, const GLchar * name );
                                
    virtual GLuint              CreateProgram				();
    virtual void                DeleteProgram				( GLuint program );
                                
    virtual GLuint              CreateShader				( GLenum type );
    virtual void                DeleteShader				( GLuint shader );

    virtual void                GenBuffers					( GLsizei n, GLuint * buffers );
    virtual void                DeleteBuffers				( GLsizei n, const GLuint * buffers );

    virtual void                BindBuffer					( GLenum target, GLuint buffer );
    virtual void                BufferData					( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );

    virtual GLvoid *            MapBuffer					( GLenum target, GLenum access );
    virtual GLboolean           UnmapBuffer					( GLenum target );

    virtual void                GenTextures					( GLsizei n, GLuint * textures );
    virtual void                DeleteTextures				( GLsizei n, const GLuint * textures );

    virtual void                TexImage2D					( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels );
    virtual void                ActiveTexture				( GLenum texture );

    virtual void                TexSubImage2D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels );
    virtual void                BindTexture					( GLenum target, GLuint texture );
    virtual void                TexParameteri				( GLenum target, GLenum pname, GLint param );
    virtual void                TexParameterfv				( GLenum target, GLenum pname, const GLfloat * params );

    virtual void                ReadBuffer					( GLenum mode );
    virtual void                ReadPixels					( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );

    virtual void                GenFramebuffers				( GLsizei n, GLuint * framebuffers );
    virtual void                DeleteFramebuffers			( GLsizei n, const GLuint * framebuffers );
    
    virtual void                GenRenderbuffers			( GLsizei n, GLuint * renderbuffers );
    virtual void                DeleteRenderbuffers			( GLsizei n, const GLuint * renderbuffers );

    virtual void                BindRenderbuffer			( GLenum target, GLuint renderbuffer );
    virtual void                RenderbufferStorage			( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );

    virtual void                BindFramebuffer				( GLenum target, GLuint framebuffer );
    virtual void                FramebufferTexture2D		( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level ) ;
    virtual void                FramebufferRenderbuffer		( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

    virtual void                DrawBuffers					( GLsizei n, const GLenum * bufs );
    virtual void                DrawArrays					( GLenum mode, GLint first, GLsizei count );

    virtual GLenum              CheckFramebufferStatus		( GLenum target );

    virtual void                Enable						( GLenum cap );
    virtual void                Disable						( GLenum cap );

    virtual void                DepthFunc					( GLenum func );
    virtual void                DepthMask					( GLboolean flag );
                
    virtual void                BlendFunc					( GLenum sfactor, GLenum dfactor );
    virtual void                BlendColor					( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );

    virtual void                FrontFace					( GLenum mode );
    virtual void                CullFace					( GLenum mode );
    
    virtual void                PolygonMode					( GLenum face, GLenum mode );
    virtual void                PolygonOffset				( GLfloat factor, GLfloat units );

    virtual GLenum              GetError					();

    virtual const GLubyte *     GetString					( GLenum name );
    virtual const GLubyte *     GetStringi					( GLenum name, GLuint index );

    virtual void                GenVertexArrays				( GLsizei n, GLuint * arrays );
    virtual void                DeleteVertexArrays			( GLsizei n, const GLuint * arrays );

    virtual void                EnableVertexAttribArray		( GLuint index );
    virtual void                DisableVertexAttribArray	( GLuint index );

    virtual void                BindVertexArray				( GLuint array );

    virtual void                VertexAttribPointer			( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

	virtual void				PrintStats					();

};

} //bv
