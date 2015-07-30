#pragma once

#include <string>
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
	virtual void				BufferSubData				( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data );

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
    virtual void                FramebufferTexture2D		( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
    virtual void                FramebufferRenderbuffer		( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

    virtual void                DrawBuffers										( GLsizei n, const GLenum * bufs );
    virtual void                DrawArrays										( GLenum mode, GLint first, GLsizei count );
	virtual void                DrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
	virtual void                DrawArraysInstanced								( GLenum mode, GLint first, GLsizei count, GLsizei primcount );
	virtual void                DrawElementsInstanced							( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount );
	virtual void                DrawElementsBaseVertex							( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex );
	virtual void                DrawArraysInstancedBaseInstance					( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance );
	virtual void                DrawElementsInstancedBaseVertexBaseInstance		( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance );
	virtual void                DrawElementsInstancedBaseVertex					( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex );
	virtual void                DrawArraysIndirect								( GLenum mode, const void *indirect );
	virtual void                DrawElementsIndirect							( GLenum mode, GLenum type, const void *indirect );
	virtual void                MultiDrawArraysIndirect							( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride );
	virtual void                MultiDrawElementsIndirect						( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride );
	
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

	virtual void				PrintShortSummary	        ( const std::string & message );
	virtual void				PrintCompleteSummary	    ( const std::string & message );

	virtual void				VertexAttribDivisor								( GLuint index,  GLuint divisor );
	
	// new
// Images and textures
	virtual void				BindImageTexture			( GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format );
	virtual void				BindImageTextures			( GLuint first, GLsizei count, const GLuint textures );

	virtual void				BindTextures				( GLuint first, GLsizei count, const GLuint* textures );
	virtual void				BindTextureUnit				( GLuint unit, GLuint texture );

	virtual void				CompressedTexImage1D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid* data );
	virtual void				CompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data );
	virtual void				CompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data );
	virtual void				CompressedTexSubImage1D		( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei ImageSize, const GLvoid* data );
	virtual void				CompressedTexSubImage2D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei ImageSize, const GLvoid* data );
	virtual void				CompressedTexSubImage3D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei ImageSize, const GLvoid* data );

	virtual void				CompressedTextureSubImage1D	( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei ImageSize, const GLvoid* data );
	virtual void				CompressedTextureSubImage2D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei ImageSize, const GLvoid* data );
	virtual void				CompressedTextureSubImage3D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei ImageSize, const GLvoid* data );

	virtual void				TexImage1D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data );
	virtual void				TexImage3D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data );
	virtual void				TexImage2DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations );
	virtual void				TexImage3DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations );

	virtual void				TexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width );
	virtual void				TexStorage3D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width );
	virtual void				TexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width );
	virtual void				TexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width );

// Sampler
	virtual void				BindSampler					( GLuint unit, GLuint sampler );
	virtual void				BindSamplers				( GLuint first, GLsizei count, const GLuint* samplers );

// Transform Feedback
	virtual void				GenTransformFeedbacks		( GLsizei n, GLuint* ids );
	virtual void				BindTransformFeedback		( GLenum target, GLuint id );
	virtual void				BeginTransformFeedback		( GLenum primitiveMode );
	virtual void				EndTransformFeedback		();
	virtual void				DrawTransformFeedback		( GLenum mode, GLuint id );

// Buffer
	virtual void				NamedBufferData				( GLuint buffer, GLsizei size, const void* data, GLenum usage );
	virtual void				NamedBufferStorage			( GLuint buffer, GLsizei size, const void* data, GLbitfield flags );
	virtual void				NamedBufferSubData			( GLuint bufferm, GLintptr offset, GLsizei size, const void* data );
	virtual void				BufferStorage				( GLenum target, GLsizeiptr size, const GLvoid* data, GLbitfield flags );

	virtual void				BindBufferBase				( GLenum target, GLuint index, GLuint buffer );
	virtual void				BindBuffersBase				( GLenum target, GLuint first, GLsizei count, const GLuint* buffer );

	virtual void				BindBufferRange				( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size );
	virtual void				BindBuffersRange			( GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offset, const GLsizeiptr* sizes );

	virtual void				BindVertexBuffer			( GLuint bindingIndex, GLuint buffer, GLintptr offset, GLintptr stride );
	virtual void				BindVertexBuffers			( GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLintptr* strides );

	virtual void				BindVertexArrayVertexBuffer				( GLuint vaobj, GLuint bindingIndex, GLuint buffer, GLintptr offset, GLintptr stride );
	virtual void				BindVertexArrayVertexBuffers			( GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLintptr* strides );

// Blending
	virtual void				BlendEquation				( GLenum mode );
	virtual void				BlendEquationi				( GLuint buf, GLenum mode );
	virtual void				BlendEquationSparate		( GLenum modeRGB, GLenum modeAlpha );
	virtual void				BlendEquationSparatei		( GLuint buf, GLenum modeRGB, GLenum modeAlpha );
	virtual void				BlendFunci					( GLuint buf, GLenum sfactor, GLenum dfactor );
	virtual void				BlendFuncSeparate			( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );
	virtual void				BlendFuncSeparatei			( GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );

// Framebuffer
	virtual void				BlitFramebuffer				( GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter );
	virtual void				BlitNamedFramebuffer		( GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter );
	/// Wystarczy zestaw FramebufferTexture, FramebufferTextureLayer oraz FramebufferTexture2D ewentualnie ich wersje z DSA
	virtual void				FramebufferTexture			( GLenum target, GLenum attachment, GLuint texture, GLint level );
	virtual void				NamedFramebufferTexture		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level );
	virtual void				NamedFramebufferTextureLayer		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer );
	virtual void				NamedFramebufferRenderbuffer		( GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

	virtual GLenum				CheckNamedFramebufferStatus	( GLuint framebuffer, GLenum target );
	virtual void				ClearBufferiv				( GLenum buffer, GLint drawbuffer, const GLint* value );
	virtual void				ClearBufferuiv				( GLenum buffer, GLint drawbuffer, const GLuint* value );
	virtual void				ClearBufferfv				( GLenum buffer, GLint drawbuffer, const GLfloat* value );
	virtual void				ClearBufferfi				( GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil );
	virtual void				ClearNamedFramebufferiv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value );
	virtual void				ClearNamedFramebufferuiv	( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value );
	virtual void				ClearNamedFramebufferfv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value );
	virtual void				ClearvNamedFramebufferfi	( GLuint framebuffer, GLenum buffer, GLfloat depth, GLint stencil );

	virtual void				NamedFramebufferDrawBuffer	( GLuint framebuffer, GLenum buf );
	virtual void				NamedFramebufferDrawBuffers	( GLuint framebuffer, GLsizei n, const GLenum* bufs );
	virtual void				NamedFramebufferReadBuffer	( GLuint framebuffer, GLenum mode );
	virtual void				NamedRenderbufferStorage	( GLuint renderbuffer, GLenum internalFormat, GLsizei width, GLsizei height );
	virtual void				NamedRenderbufferStorageMultisample	( GLuint renderbuffer, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height );
	virtual void				RenderbufferStorageMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height );




// Stencil
	virtual void				ClearStencil				( GLint s );


};

} //bv
