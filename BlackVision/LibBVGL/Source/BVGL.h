#pragma once

#include "BVGLDefs.h"


namespace bv {

class BVGL
{
public:

    static bool                 InitializeBVGL              ();

    static void					PrintShortSummary	        ( const char * message );
    static void					PrintCompleteSummary	    ( const char * message );

    static void                 bvglViewport                ( GLint x, GLint y, GLsizei width, GLsizei height );

    static void                 bvglClearColor              ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
    static void                 bvglClearDepth              ( GLclampd depth );
    static void                 bvglClear                   ( GLbitfield mask );

    static void                 bvglLineWidth               ( GLfloat width );

    static void                 bvglDepthRange              ( GLclampd zNear, GLclampd zFar );

    static void                 bvglEnable                  ( GLenum cap );
    static void                 bvglDisable                 ( GLenum cap );

    static void                 bvglDepthFunc               ( GLenum func );
    static void                 bvglDepthMask               ( GLboolean flag );

    static void                 bvglBlendFunc               ( GLenum sfactor, GLenum dfactor );
    static void                 bvglBlendColor              ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );

    static void                 bvglFrontFace               ( GLenum mode );
    static void                 bvglCullFace                ( GLenum mode );

    static void                 bvglPolygonMode             ( GLenum face, GLenum mode );
    static void                 bvglPolygonOffset           ( GLfloat factor, GLfloat units );

    static GLenum               bvglGetError                ();
    static const GLubyte *      bvgluErrorString            ( GLenum error );

    static const GLubyte *      bvglGetString               ( GLenum name );
    static const GLubyte *      bvglGetStringi              ( GLenum name, GLuint index );
    
    static void                 bvglReadBuffer              ( GLenum mode );
    static void                 bvglReadPixels              ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );

// Shaders
    static GLuint               bvglCreateProgram           ();
    static void                 bvglDeleteProgram           ( GLuint program );

    static GLuint               bvglCreateShader            ( GLenum type );
    static void                 bvglDeleteShader            ( GLuint shader );

    static void                 bvglLinkProgram             ( GLuint program );
    static void                 bvglUseProgram              ( GLuint program );
    static void                 bvglValidateProgram         ( GLuint program );

    static void                 bvglShaderSource            ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length );
    static void                 bvglCompileShader           ( GLuint shader );
    static void                 bvglAttachShader            ( GLuint program, GLuint shader );

    static void                 bvglGetProgramiv            ( GLuint program, GLenum pname, GLint * param );
    static void                 bvglGetProgramInfoLog       ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );

    static void                 bvglGetShaderiv             ( GLuint shader, GLenum pname, GLint * param );
    static void                 bvglGetShaderInfoLog        ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
    
    static void                 bvglGetActiveAttrib         ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint                bvglGetAttribLocation       ( GLuint program, const GLchar * name );

    static void                 bvglBindAttribLocation      ( GLuint program, GLuint index, const GLchar * name );
    static void                 bvglBindFragDataLocation    ( GLuint program, GLuint colorNumber, const GLchar * name );

    static void                 bvglGetIntegerv             ( GLenum pname, GLint * params );
    static void                 bvglGetDoublev              ( GLenum pname, GLdouble * params );

// uniforms
    static void                 bvglGetActiveUniform        ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
    static GLint                bvglGetUniformLocation      ( GLuint program, const GLchar * name );

    static void                 bvglUniform1i               ( GLint location, GLint v );

    static void                 bvglUniform1f               ( GLint location, GLfloat v );
    static void                 bvglUniform2f               ( GLint location, GLfloat v, GLfloat v1 );
    static void                 bvglUniform3f               ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 );
    static void                 bvglUniform4f               ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 );

    static void                 bvglUniformMatrix2fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void                 bvglUniformMatrix3fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
    static void                 bvglUniformMatrix4fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );


// Vertex Array Object
    static void                 bvglGenVertexArrays         ( GLsizei n, GLuint * arrays );
    static void                 bvglDeleteVertexArrays      ( GLsizei n, const GLuint * arrays );

    static void                 bvglEnableVertexAttribArray ( GLuint index );
    static void                 bvglDisableVertexAttribArray( GLuint index );

    static void                 bvglBindVertexArray         ( GLuint array );

    static void                 bvglVertexAttribPointer     ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    static void					bvglVertexAttribDivisor		( GLuint index,  GLuint divisor );

// Images and textures
    static void                 bvglGenTextures             ( GLsizei n, GLuint * textures );
    static void                 bvglDeleteTextures          ( GLsizei n, const GLuint * textures );

    static void                 bvglActiveTexture           ( GLenum texture );
    
    static void                 bvglBindTexture             ( GLenum target, GLuint texture );
#ifdef BV_GL_VERSION_4_4
    static void					bvglBindImageTexture			( GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format );
    static void					bvglBindImageTextures			( GLuint first, GLsizei count, const GLuint* textures );
    static void					bvglBindTextures				( GLuint first, GLsizei count, const GLuint* textures );
#endif
#ifdef BV_GL_VERSION_4_5
    static void					bvglBindTextureUnit				( GLuint unit, GLuint texture );
#endif

    static void					bvglCompressedTexImage1D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid* data );
    static void					bvglCompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data );
    static void					bvglCompressedTexImage3D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data );
    static void					bvglCompressedTexSubImage1D		( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data );
    static void					bvglCompressedTexSubImage2D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data );
    static void					bvglCompressedTexSubImage3D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data );

    static void					bvglTexImage1D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data );
    static void                 bvglTexImage2D					( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels );
    static void					bvglTexImage3D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data );
    static void					bvglTexImage2DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations );
    static void					bvglTexImage3DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations );

    static void					bvglTexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width );
    static void					bvglTexStorage2D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height );
    static void					bvglTexStorage3D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth );
    static void					bvglTexStorage2DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSasmpleLocations );
    static void					bvglTexStorage3DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSasmpleLocations );

    static void					bvglTexSubImage1D				( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels );
    static void                 bvglTexSubImage2D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels );
    static void					bvglTexSubImage3D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels );

#ifdef BV_GL_VERSION_4_5
    static void					bvglCompressedTextureSubImage1D	( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data );
    static void					bvglCompressedTextureSubImage2D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data );
    static void					bvglCompressedTextureSubImage3D	( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data );

    static void					bvglTextureStorage1D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width );
    static void					bvglTextureStorage2D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height );
    static void					bvglTextureStorage3D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth );
    static void					bvglTextureStorage2DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations );
    static void					bvglTextureStorage3DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations );

    static void					bvglTextureSubImage1D			( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels );
    static void					bvglTextureSubImage2D			( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels );
    static void					bvglTextureSubImage3D			( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels );

    static void					bvglTextureBuffer				( GLuint texture, GLenum internalFormat, GLuint buffer );
    static void					bvglTextureBufferRange			( GLuint texture, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size );
#endif
    
    static void					bvglTexBuffer					( GLenum target, GLenum internalFormat, GLuint buffer );	
    static void					bvglTexBufferRange				( GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size );

    static void					bvglTextureView					( GLuint texture, GLenum target, GLuint origTexture, GLenum internalFormat, GLuint minLevel, GLuint numLevels, GLuint minLayer, GLuint numLayers );

#ifdef BV_GL_VERSION_4_5
    static void					bvglTextureParameteri			( GLuint texture, GLenum pname, GLint param );
    static void					bvglTextureParameterfv			( GLuint texture, GLenum pname, const GLfloat * params );
#endif
    static void                 bvglTexParameteri           ( GLenum target, GLenum pname, GLint param );
    static void                 bvglTexParameterfv          ( GLenum target, GLenum pname, const GLfloat * params );

    static void					bvglCopyTexSubImage1D			( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width );
    static void					bvglCopyTexSubImage2D			( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height );
    static void					bvglCopyTexSubImage3D			( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
#ifdef BV_GL_VERSION_4_5
    static void					bvglCopyTextureSubImage1D		( GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width );
    static void					bvglCopyTextureSubImage2D		( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height );
    static void					bvglCopyTextureSubImage3D		( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
#endif
    static void					bvglCopyTexImage2D				( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border );
    static void					bvglCopyTexImage1D				( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border );
    static void					bvglCopyImageSubData			( GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth );

    static void					bvglGetTexImage					( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * data );
    static void					bvglGenerateMipmap				( GLenum target );

// Sampler
#ifdef BV_GL_VERSION_4_5
    static void					bvglCreateSamplers				( GLsizei n, GLuint* samplers );
#endif
    static void					bvglGenSamplers					( GLsizei n, GLuint* samplers );
    static void					bvglDeleteSamplers				( GLsizei n, GLuint* samplers );

    static void					bvglBindSampler					( GLuint unit, GLuint sampler );
#ifdef BV_GL_VERSION_4_4
    static void					bvglBindSamplers				( GLuint first, GLsizei count, const GLuint* samplers );
#endif

    static void					bvglSamplerParameteri			( GLuint sampler, GLenum pname, GLint param );
    static void					bvglSamplerParameterfv			( GLuint sampler, GLenum pname, const GLfloat* params );
    static void					bvglSamplerParameterf			( GLuint sampler, GLenum pname, GLfloat param );

// Transform Feedback
    static void					bvglGenTransformFeedbacks		( GLsizei n, GLuint* ids );
    static void					bvglBindTransformFeedback		( GLenum target, GLuint id );
    static void					bvglBeginTransformFeedback		( GLenum primitiveMode );
    static void					bvglEndTransformFeedback		();

    static void					bvglDrawTransformFeedback				( GLenum mode, GLuint id );
    static void					bvglDrawTransformFeedbackInstanced		( GLenum mode, GLuint id, GLsizei primCount );

// Buffer
    static void                 bvglGenBuffers					( GLsizei n, GLuint * buffers );
    static void                 bvglDeleteBuffers				( GLsizei n, const GLuint * buffers );

    static void                 bvglBufferData					( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );
    static void					bvglBufferSubData				( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data );
#ifdef BV_GL_VERSION_4_5
    static void					bvglNamedBufferData				( GLuint buffer, GLsizei size, const void* data, GLenum usage );
    static void					bvglNamedBufferStorage			( GLuint buffer, GLsizei size, const void* data, GLbitfield flags );
    static void					bvglNamedBufferSubData			( GLuint buffer, GLintptr offset, GLsizei size, const void* data );
#endif
#ifdef BV_GL_VERSION_4_4
    static void					bvglBufferStorage				( GLenum target, GLsizeiptr size, const GLvoid* data, GLbitfield flags );


    static void					bvglBindBuffersBase				( GLenum target, GLuint first, GLsizei count, const GLuint* buffer );
    static void					bvglBindBuffersRange			( GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offset, const GLsizeiptr* sizes );
    static void					bvglBindVertexBuffers			( GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides );
#endif
    static void					bvglBindBufferBase				( GLenum target, GLuint index, GLuint buffer );
    static void					bvglBindBufferRange				( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size );
    static void					bvglBindVertexBuffer			( GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizei stride );
    static void                 bvglBindBuffer					( GLenum target, GLuint buffer );
#ifdef BV_GL_VERSION_4_5
    static void					bvglVertexArrayVertexBuffers	( GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides );


    static void*				bvglMapNamedBuffer				( GLuint buffer, GLenum access );
    static void*				bvglMapNamedBufferRange			( GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access );
    static GLboolean			bvglUnmapNamedBuffer			( GLuint buffer );
    static void					bvglCopyNamedBufferSubData		( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size );
#endif
    static GLvoid *             bvglMapBuffer               ( GLenum target, GLenum access );
    static GLboolean            bvglUnmapBuffer             ( GLenum target );
    static void*				bvglMapBufferRange				( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access );
    static void					bvglCopyBufferSubData			( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size );

// Blending
    static void					bvglBlendEquation				( GLenum mode );
    static void					bvglBlendEquationi				( GLuint buf, GLenum mode );
    static void					bvglBlendEquationSeparate		( GLenum modeRGB, GLenum modeAlpha );
    static void					bvglBlendEquationSeparatei		( GLuint buf, GLenum modeRGB, GLenum modeAlpha );
    static void					bvglBlendFunci					( GLuint buf, GLenum sfactor, GLenum dfactor );
    static void					bvglBlendFuncSeparate			( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );
    static void					bvglBlendFuncSeparatei			( GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );

// Framebuffer
    static void                 bvglGenFramebuffers         ( GLsizei n, GLuint * framebuffers );
    static void                 bvglDeleteFramebuffers      ( GLsizei n, const GLuint * framebuffers );

    static void                 bvglGenRenderbuffers        ( GLsizei n, GLuint * renderbuffers );
    static void                 bvglDeleteRenderbuffers     ( GLsizei n, const GLuint * renderbuffers );

    static void                 bvglBindRenderbuffer        ( GLenum target, GLuint renderbuffer );
    static void                 bvglBindFramebuffer         ( GLenum target, GLuint framebuffer );
    static void                 bvglFramebufferRenderbuffer ( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

    static void					bvglFramebufferTexture			( GLenum target, GLenum attachment, GLuint texture, GLint level );
    static void                 bvglFramebufferTexture2D		( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level ) ;
    static void					bvglFramebufferTextureLayer		( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer );

    static void					bvglBlitFramebuffer				( GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter );
#ifdef BV_GL_VERSION_4_5
    static void					bvglBlitNamedFramebuffer		( GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter );
    static void					bvglNamedFramebufferTexture		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level );
    static void					bvglNamedFramebufferTextureLayer( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer );
    static void					bvglNamedFramebufferRenderbuffer( GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

    static GLenum				bvglCheckNamedFramebufferStatus	( GLuint framebuffer, GLenum target );
#endif
    static GLenum               bvglCheckFramebufferStatus		( GLenum target );


    static void					bvglClearBufferiv				( GLenum buffer, GLint drawbuffer, const GLint* value );
    static void					bvglClearBufferuiv				( GLenum buffer, GLint drawbuffer, const GLuint* value );
    static void					bvglClearBufferfv				( GLenum buffer, GLint drawbuffer, const GLfloat* value );
    static void					bvglClearBufferfi				( GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil );
#ifdef BV_GL_VERSION_4_5
    static void					bvglClearNamedFramebufferiv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value );
    static void					bvglClearNamedFramebufferuiv	( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value );
    static void					bvglClearNamedFramebufferfv		( GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value );
    static void					bvglClearvNamedFramebufferfi	( GLuint framebuffer, GLenum buffer, GLfloat depth, GLint stencil );


    static void					bvglNamedFramebufferDrawBuffer				( GLuint framebuffer, GLenum buf );
    static void					bvglNamedFramebufferDrawBuffers				( GLuint framebuffer, GLsizei n, const GLenum* bufs );
    static void					bvglNamedFramebufferReadBuffer				( GLuint framebuffer, GLenum mode );
    static void					bvglNamedRenderbufferStorage				( GLuint renderbuffer, GLenum internalFormat, GLsizei width, GLsizei height );
    static void					bvglNamedRenderbufferStorageMultisample		( GLuint renderbuffer, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height );
#endif	
    static void					bvglRenderbufferStorageMultisample			( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height );
    static void                 bvglRenderbufferStorage						( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );
// Drawing
    static void					bvglDrawBuffers										( GLsizei n, const GLenum * bufs );
    static void					bvglDrawArrays										( GLenum mode, GLint first, GLsizei count );
    static void					bvglDrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
    static void					bvglDrawArraysInstanced								( GLenum mode, GLint first, GLsizei count, GLsizei primcount );
    static void					bvglDrawElementsInstanced							( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount );
    static void					bvglDrawElementsBaseVertex							( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex );
    static void					bvglDrawArraysInstancedBaseInstance					( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance );
    static void					bvglDrawElementsInstancedBaseVertexBaseInstance		( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance );
    static void					bvglDrawElementsInstancedBaseVertex					( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex );
    static void					bvglDrawArraysIndirect								( GLenum mode, const void *indirect );
    static void					bvglDrawElementsIndirect							( GLenum mode, GLenum type, const void *indirect );
    static void					bvglMultiDrawArraysIndirect							( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride );
    static void					bvglMultiDrawElementsIndirect						( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride );
    static void					bvglMultiDrawArrays									( GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawCount );
    static void					bvglMultiDrawElements								( GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indicies, GLsizei drawCount );
    static void					bvglMultiDrawElementsBaseVertex						( GLenum mode, GLsizei* count, GLenum type, const GLvoid** indicies, GLsizei drawCount, GLsizei* baseVartex );

// Stencil
    static void					bvglClearStencil				( GLint s );

    //static const GLubyte *      bvglGetString               ( GLenum name );
    //static const GLubyte *      bvglGetStringi              ( GLenum name, GLuint index );

    //static void                 bvglGenVertexArrays         ( GLsizei n, GLuint * arrays );
    //static void                 bvglDeleteVertexArrays      ( GLsizei n, const GLuint * arrays );

    //static void                 bvglEnableVertexAttribArray ( GLuint index );
    //static void                 bvglDisableVertexAttribArray( GLuint index );

    //static void                 bvglBindVertexArray         ( GLuint array );

    //static void                 bvglVertexAttribPointer     ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    //static void					bvglVertexAttribDivisor		( GLuint index,  GLuint divisor );

    //static const GLubyte *      bvglGetString               ( GLenum name );
    //static const GLubyte *      bvglGetStringi              ( GLenum name, GLuint index );

    //static void                 bvglGenVertexArrays         ( GLsizei n, GLuint * arrays );
    //static void                 bvglDeleteVertexArrays      ( GLsizei n, const GLuint * arrays );

    //static void                 bvglEnableVertexAttribArray ( GLuint index );
    //static void                 bvglDisableVertexAttribArray( GLuint index );

    //static void                 bvglBindVertexArray         ( GLuint array );

    //static void                 bvglVertexAttribPointer     ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    //pablito
    static void					bvglBindFramebufferEXT		(GLenum target, GLuint framebuffer);
    static void					bvglBindRenderbufferEXT		(GLenum target, GLuint renderbuffer);
    static void					bvglDeleteFramebuffersEXT	(GLsizei n, const GLuint *framebuffers);
    static void					bvglDeleteRenderbuffersEXT	(GLsizei n, const GLuint *renderbuffers);
    //static GLuint				bvglGetError				();
    static void					bvglFlush					();
    static void					bvglFinish					();

//	static void					bvglVertexAttribDivisor		( GLuint index,  GLuint divisor );


    static void					bvglStencilFunc					( GLenum func, GLint ref, GLuint mask );
    static void					bvglStencilFuncSeparate			( GLenum face, GLenum func, GLint ref, GLuint mask );
    static void					bvglStencilMask					( GLuint mask );
    static void					bvglStencilMaskSeparate			( GLenum face, GLuint mask );
    static void					bvglStencilOp					( GLenum sfail, GLenum dpfail, GLenum dppass );
    static void					bvglStencilOpSeparate			( GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass );
};

} //bv
