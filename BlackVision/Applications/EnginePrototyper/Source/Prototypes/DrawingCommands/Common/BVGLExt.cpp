#include "BVGLExt.h"
#include "BVGLPluginExt.h"
#include "BVGLPlugins\BVGLPluginFactory.h"

namespace  bv {

namespace {
	
BVGLPluginExt bvgl = (BVGLPluginExt&)BVGLPluginFactory::BVGL( BVGLPluginType::BVGL_DEFAULT );

} // anonymous

// *****************************
//
void        BVGLExt::bvglDrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
    bvgl.DrawElements( mode, count, type, indices );
}

// *****************************
//
void        BVGLExt::bvglDrawElementsBaseVertex							( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex )
{
    bvgl.DrawElementsBaseVertex( mode, count, type, indices, basevertex );
}

// *****************************
//
void        BVGLExt::bvglDrawArraysInstanced							( GLenum mode, GLint first, GLsizei count, GLsizei primcount )
{
    bvgl.DrawArraysInstanced( mode, first, count, primcount );
}

// *****************************
//
void        BVGLExt::bvglDrawElementsInstanced							( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount )
{
    bvgl.DrawElementsInstanced( mode, count, type, indices, primcount );
}

// *****************************
//
void        BVGLExt::bvglDrawArraysInstancedBaseInstance				( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance )
{
    bvgl.DrawArraysInstancedBaseInstance( mode, first, count, primcount, baseinstance );
}

// *****************************
//
void        BVGLExt::bvglDrawElementsInstancedBaseVertexBaseInstance	( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance )
{
    bvgl.DrawElementsInstancedBaseVertexBaseInstance( mode, count, type, indices, primcount, basevertex, baseinstance );
}

// *****************************
//
void        BVGLExt::bvglDrawElementsInstancedBaseVertex				( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex )
{
    bvgl.DrawElementsInstancedBaseVertex( mode, count, type, indices, primcount, basevertex );
}

// *****************************
//
void        BVGLExt::bvglDrawArraysIndirect								( GLenum mode, const void *indirect )
{
    bvgl.DrawArraysIndirect( mode, indirect );
}

// *****************************
//
void        BVGLExt::bvglDrawElementsIndirect							( GLenum mode, GLenum type, const void *indirect )
{
    bvgl.DrawElementsIndirect( mode, type, indirect );
}

// *****************************
//
void        BVGLExt::bvglMultiDrawArraysIndirect						( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    bvgl.MultiDrawArraysIndirect( mode, indirect, drawcount, stride );
}

// *****************************
//
void        BVGLExt::bvglMultiDrawElementsIndirect						( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    bvgl.MultiDrawElementsIndirect( mode, type, indirect, drawcount, stride );
}

// *****************************
//
void		BVGLExt::bvglVertexAttribDivisor							( GLuint index,  GLuint divisor )
{
	bvgl.VertexAttribDivisor( index, divisor );
}

// *****************************
//
void		BVGLExt::bvglBufferSubData									( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data )
{
	bvgl.BufferSubData( target, offset, size, data );
}

// *****************************
//
void *		BVGLExt::bvglMapBufferRange									( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access )
{
	return bvgl.MapBufferRange( target, offset, length, access );
}

//------------------------overrides---------------------------

// *****************************
//
void		BVGLExt::bvglBufferDataOverride								( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
	bvgl.BufferData( target, size, data, usage );
}

} //bv
