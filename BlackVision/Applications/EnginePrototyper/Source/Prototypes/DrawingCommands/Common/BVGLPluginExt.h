#pragma once

#include "BVGLPlugins\BVGLPlugin.h"

namespace  bv {

//BVGLPlugin extended with more drawing commands
class BVGLPluginExt : public BVGLPlugin
{
public:
								BVGLPluginExt									();

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
	
	virtual void				VertexAttribDivisor								( GLuint index,  GLuint divisor );
	
	virtual void				BufferSubData									( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data );
	
	//overrides..
	virtual void                BufferData										( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage ) override;

};

} //bv
