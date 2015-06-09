#pragma once

#include "BVGLDefs.h"


namespace bv {

class BVGL
{
public:

	static bool                 InitializeBVGL              ();

    static void				    PrintShortSummary	        ( const char * message );
	static void				    PrintCompleteSummary	    ( const char * message );

	static void                 bvglViewport                ( GLint x, GLint y, GLsizei width, GLsizei height );

	static void                 bvglClearColor              ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
	static void                 bvglClearDepth              ( GLclampd depth );
	static void                 bvglClear                   ( GLbitfield mask );

	static void                 bvglDepthRange              ( GLclampd zNear, GLclampd zFar );

	static void                 bvglLinkProgram             ( GLuint program );
	static void                 bvglUseProgram              ( GLuint program );
	static void                 bvglValidateProgram         ( GLuint program );

	static void                 bvglUniform1i               ( GLint location, GLint v );

	static void                 bvglUniform1f               ( GLint location, GLfloat v );
	static void                 bvglUniform2f               ( GLint location, GLfloat v, GLfloat v1 );
	static void                 bvglUniform3f               ( GLint location, GLfloat v, GLfloat v1, GLfloat v2 );
	static void                 bvglUniform4f               ( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 );

	static void                 bvglUniformMatrix2fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
	static void                 bvglUniformMatrix3fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
	static void                 bvglUniformMatrix4fv        ( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );

	static void                 bvglShaderSource            ( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length );
	static void                 bvglCompileShader           ( GLuint shader );
	static void                 bvglAttachShader            ( GLuint program, GLuint shader );

	static void                 bvglGetProgramiv            ( GLuint program, GLenum pname, GLint * param );
	static void                 bvglGetProgramInfoLog       ( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );

	static void                 bvglGetActiveUniform        ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
	static GLint                bvglGetUniformLocation      ( GLuint program, const GLchar * name );

	static void                 bvglGetActiveAttrib         ( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name );
	static GLint                bvglGetAttribLocation       ( GLuint program, const GLchar * name );

	static void                 bvglGetShaderiv             ( GLuint shader, GLenum pname, GLint * param );
	static void                 bvglGetShaderInfoLog        ( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog );

	static void                 bvglGetIntegerv             ( GLenum pname, GLint * params );
	static void                 bvglGetDoublev              ( GLenum pname, GLdouble * params );

	static void                 bvglBindAttribLocation      ( GLuint program, GLuint index, const GLchar * name );
	static void                 bvglBindFragDataLocation    ( GLuint program, GLuint colorNumber, const GLchar * name );

	static GLuint               bvglCreateProgram           ();
	static void                 bvglDeleteProgram           ( GLuint program );

	static GLuint               bvglCreateShader            ( GLenum type );
	static void                 bvglDeleteShader            ( GLuint shader );

	static void                 bvglGenBuffers              ( GLsizei n, GLuint * buffers );
	static void                 bvglDeleteBuffers           ( GLsizei n, const GLuint * buffers );

	static void                 bvglBindBuffer              ( GLenum target, GLuint buffer );
	static void                 bvglBufferData              ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );
	static void					bvglBufferSubData			( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data );

	static GLvoid *             bvglMapBuffer               ( GLenum target, GLenum access );
	static GLboolean            bvglUnmapBuffer             ( GLenum target );

	static void                 bvglGenTextures             ( GLsizei n, GLuint * textures );
	static void                 bvglDeleteTextures          ( GLsizei n, const GLuint * textures );

	static void                 bvglTexImage2D              ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels );
	static void                 bvglActiveTexture           ( GLenum texture );

	static void                 bvglTexSubImage2D           ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels );
	static void                 bvglBindTexture             ( GLenum target, GLuint texture );
	static void                 bvglTexParameteri           ( GLenum target, GLenum pname, GLint param );
	static void                 bvglTexParameterfv          ( GLenum target, GLenum pname, const GLfloat * params );

	static void                 bvglReadBuffer              ( GLenum mode );
	static void                 bvglReadPixels              ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );

	static void                 bvglGenFramebuffers         ( GLsizei n, GLuint * framebuffers );
	static void                 bvglDeleteFramebuffers      ( GLsizei n, const GLuint * framebuffers );

	static void                 bvglGenRenderbuffers        ( GLsizei n, GLuint * renderbuffers );
	static void                 bvglDeleteRenderbuffers     ( GLsizei n, const GLuint * renderbuffers );

	static void                 bvglBindRenderbuffer        ( GLenum target, GLuint renderbuffer );
	static void                 bvglRenderbufferStorage     ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );

	static void                 bvglBindFramebuffer         ( GLenum target, GLuint framebuffer );
	static void                 bvglFramebufferTexture2D    ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level ) ;
	static void                 bvglFramebufferRenderbuffer ( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

	static void                 bvglDrawBuffers					( GLsizei n, const GLenum * bufs );
	static void                 bvglDrawArrays					( GLenum mode, GLint first, GLsizei count );
	static void					bvglDrawElements				( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
	static void					bvglDrawElementsBaseVertex		( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex );
	static void					bvglDrawArraysInstanced			( GLenum mode, GLint first, GLsizei count, GLsizei primcount );
	static void					bvglDrawElementsInstanced		( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount );
	static void					bvglDrawArraysInstancedBaseInstance				( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance );
	static void					bvglDrawElementsInstancedBaseVertexBaseInstance	( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance );
	static void					bvglDrawElementsInstancedBaseVertex				( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex );
	static void					bvglDrawArraysIndirect			( GLenum mode, const void *indirect );
	static void					bvglDrawElementsIndirect		( GLenum mode, GLenum type, const void *indirect );
	static void					bvglMultiDrawArraysIndirect		( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride );
	static void					bvglMultiDrawElementsIndirect	( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride );

	static GLenum               bvglCheckFramebufferStatus  ( GLenum target );

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

	static const GLubyte *      bvglGetString               ( GLenum name );
	static const GLubyte *      bvglGetStringi              ( GLenum name, GLuint index );

	static void                 bvglGenVertexArrays         ( GLsizei n, GLuint * arrays );
	static void                 bvglDeleteVertexArrays      ( GLsizei n, const GLuint * arrays );

	static void                 bvglEnableVertexAttribArray ( GLuint index );
	static void                 bvglDisableVertexAttribArray( GLuint index );

	static void                 bvglBindVertexArray         ( GLuint array );

	static void                 bvglVertexAttribPointer     ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

	//pablito
	static void					bvglBindFramebufferEXT		(GLenum target, GLuint framebuffer);
	static void					bvglBindRenderbufferEXT		(GLenum target, GLuint renderbuffer);
	static void					bvglDeleteFramebuffersEXT	(GLsizei n, const GLuint *framebuffers);
	static void					bvglDeleteRenderbuffersEXT	(GLsizei n, const GLuint *renderbuffers);
	//static GLuint				bvglGetError				();
	static void					bvglFlush					();
	static void					bvglFinish					();

	static void					bvglVertexAttribDivisor		( GLuint index,  GLuint divisor );


};

} //bv
