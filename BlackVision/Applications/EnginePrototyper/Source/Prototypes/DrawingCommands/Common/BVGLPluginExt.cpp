#include "BVGLPluginExt.h"

#include <assert.h> 
#include <gl/glew.h>

namespace  bv {

// *****************************
//
			BVGLPluginExt::BVGLPluginExt								()
{
}

// *****************************
//
void        BVGLPluginExt::DrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
    glDrawElements( mode, count, type, indices );
}

// *****************************
//
void        BVGLPluginExt::DrawArraysInstanced							( GLenum mode, GLint first, GLsizei count, GLsizei primcount )
{
    glDrawArraysInstanced( mode, first, count, primcount );
}

// *****************************
//
void        BVGLPluginExt::DrawElementsInstanced						( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount )
{
    glDrawElementsInstanced( mode, count, type, indices, primcount );
}

// *****************************
//
void        BVGLPluginExt::DrawElementsBaseVertex						( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex )
{
	if (!GLEW_VERSION_3_2) 
		assert( false );
    glDrawElementsBaseVertex( mode, count, type, indices, basevertex );
}

// *****************************
//
void        BVGLPluginExt::DrawArraysInstancedBaseInstance				( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance )
{
	if (!GLEW_VERSION_4_2) 
		assert( false );
    glDrawArraysInstancedBaseInstance( mode, first, count, primcount, baseinstance );
}

// *****************************
//
void        BVGLPluginExt::DrawElementsInstancedBaseVertexBaseInstance	( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance )
{
	if (!GLEW_VERSION_4_2) 
		assert( false );
    glDrawElementsInstancedBaseVertexBaseInstance( mode, count, type, indices, primcount, basevertex, baseinstance );
}

// *****************************
//
void        BVGLPluginExt::DrawElementsInstancedBaseVertex				( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex )
{
	if (!GLEW_VERSION_3_2) 
		assert( false );
    glDrawElementsInstancedBaseVertex( mode, count, type, indices, primcount, basevertex );
}

// *****************************
//
void        BVGLPluginExt::DrawArraysIndirect							( GLenum mode, const void *indirect )
{
	if (!GLEW_VERSION_4_0) 
		assert( false );
    glDrawArraysIndirect( mode, indirect );
}

// *****************************
//
void        BVGLPluginExt::DrawElementsIndirect							( GLenum mode, GLenum type, const void *indirect )
{
	if (!GLEW_VERSION_4_0) 
		assert( false );
    glDrawElementsIndirect( mode, type, indirect );
}

// *****************************
//
void        BVGLPluginExt::MultiDrawArraysIndirect						( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride )
{
	if (!GLEW_VERSION_4_3) 
		assert( false );
    glMultiDrawArraysIndirect( mode, indirect, drawcount, stride );
}

// *****************************
//
void        BVGLPluginExt::MultiDrawElementsIndirect					( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride )
{
	if (!GLEW_VERSION_4_3) 
		assert( false );
    glMultiDrawElementsIndirect( mode, type, indirect, drawcount, stride );
}

// *****************************
//
void        BVGLPluginExt::VertexAttribDivisor							( GLuint index,  GLuint divisor )
{
	if (!GLEW_VERSION_3_2) 
		assert( false );
    glVertexAttribDivisor( index, divisor );
}

// *****************************
//
void        BVGLPluginExt::BufferSubData								( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data )
{
	glBufferSubData( target, offset, size, data );
}

// *****************************
//
void *		BVGLPluginExt::MapBufferRange								( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access )
{
	return glMapBufferRange( target, offset, length, access );
}


//overrides...

// *****************************
//
void        BVGLPluginExt::BufferData									( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )\
{
	glBufferData( target, size, data, usage );
}


} //bv
