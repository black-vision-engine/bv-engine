#pragma once

#include "BVGL.h"
#include "BVGLDefsExt.h"

namespace  bv {

//BVGL extended with more drawing commands
class BVGLExt : public BVGL
{
public:
	static void                bvglDrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );

	static void                bvglDrawElementsBaseVertex						( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex );

	static void                bvglDrawArraysInstanced							( GLenum mode, GLint first, GLsizei count, GLsizei primcount );
	static void                bvglDrawElementsInstanced						( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount );

	static void                bvglDrawArraysInstancedBaseInstance				( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance );
	static void                bvglDrawElementsInstancedBaseVertexBaseInstance	( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance );
	static void                bvglDrawElementsInstancedBaseVertex				( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex );

	static void                bvglDrawArraysIndirect							( GLenum mode, const void *indirect );
	static void                bvglDrawElementsIndirect							( GLenum mode, GLenum type, const void *indirect );
	
	static void                bvglMultiDrawArraysIndirect						( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride );
	static void                bvglMultiDrawElementsIndirect					( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride );

	static void				   bvglVertexAttribDivisor							( GLuint index,  GLuint divisor );
	
	static void				   bvglBufferSubData								( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data );

	//override 'hack'...
	static  void               bvglBufferDataOverride							( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );

};

} //bv
