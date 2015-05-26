#include "BVGLPlugin.h"

#include <gl/glew.h>


namespace bv {

// *****************************
// FIXME: this initialization should check more caps and return an error code instead of a simple bool
bool        BVGLPlugin::InitializeBVGL						()
{
    glewInit();

    if ( !GLEW_ARB_vertex_program || !glewGetExtension( "GL_ARB_fragment_program" ) ) 
    {		
        return false;
    }
    
    return true;
}

// *****************************
//
void        BVGLPlugin::Viewport						( GLint x, GLint y, GLsizei width, GLsizei height )
{
    glViewport( x, y, width, height );
}

// *****************************
//
void        BVGLPlugin::ClearColor						( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    glClearColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGLPlugin::ClearDepth						( GLclampd depth )
{
    glClearDepth( depth );
}

// *****************************
//
void        BVGLPlugin::Clear							( GLbitfield mask )
{
    glClear( mask );
}

// *****************************
//
void        BVGLPlugin::DepthRange						( GLclampd zNear, GLclampd zFar )
{
    glDepthRange( zNear, zFar );
}

// *****************************
//
void        BVGLPlugin::LinkProgram						( GLuint program )
{
    glLinkProgram( program );
}

// *****************************
//
void        BVGLPlugin::UseProgram						( GLuint program )
{
    glUseProgram( program );
}

// *****************************
//
void        BVGLPlugin::ValidateProgram					( GLuint program )
{
    glValidateProgram( program );
}

// *****************************
//
void        BVGLPlugin::Uniform1i						( GLint location, GLint v )
{
    glUniform1i( location, v );
}

// *****************************
//
void        BVGLPlugin::Uniform1f						( GLint location, GLfloat v )
{
    glUniform1f( location, v );
}

// *****************************
//
void        BVGLPlugin::Uniform2f						( GLint location, GLfloat v, GLfloat v1 )
{
    glUniform2f( location, v, v1 );
}

// *****************************
//
void        BVGLPlugin::Uniform3f						( GLint location, GLfloat v, GLfloat v1, GLfloat v2 )
{
    glUniform3f( location, v, v1, v2 );
}

// *****************************
//
void        BVGLPlugin::Uniform4f						( GLint location, GLfloat v, GLfloat v1, GLfloat v2, GLfloat v3 )
{
    glUniform4f( location, v, v1, v2, v3 );
}

// *****************************
//
void        BVGLPlugin::UniformMatrix2fv				( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    glUniformMatrix2fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLPlugin::UniformMatrix3fv				( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    glUniformMatrix3fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLPlugin::UniformMatrix4fv				( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value )
{
    glUniformMatrix4fv( location, count, transpose, value );
}

// *****************************
//
void        BVGLPlugin::ShaderSource					( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * length )
{
    glShaderSource( shader, count, strings, length ); 
}

// *****************************
//
void        BVGLPlugin::CompileShader					( GLuint shader )
{
    glCompileShader( shader );
}

// *****************************
//
void        BVGLPlugin::AttachShader					( GLuint program, GLuint shader )
{
    glAttachShader( program, shader );
}

// *****************************
//
void        BVGLPlugin::GetProgramiv					( GLuint program, GLenum pname, GLint * param )
{
    glGetProgramiv( program, pname, param );
}

// *****************************
//
void        BVGLPlugin::GetProgramInfoLog				( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    glGetProgramInfoLog( program, bufSize, length, infoLog );
}

// *****************************
//
void        BVGLPlugin::GetActiveUniform				( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    glGetActiveUniform( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGLPlugin::GetUniformLocation				( GLuint program, const GLchar * name )
{
    return glGetUniformLocation( program, name );
}

// *****************************
//
void        BVGLPlugin::GetActiveAttrib					( GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name )
{
    glGetActiveAttrib( program, index, bufSize, length, size, type, name );
}

// *****************************
//
GLint       BVGLPlugin::GetAttribLocation				( GLuint program, const GLchar * name )
{
    return glGetAttribLocation( program, name );
}

// *****************************
//
void        BVGLPlugin::GetShaderiv						( GLuint shader, GLenum pname, GLint * param )
{
    glGetShaderiv( shader, pname, param );
}

// *****************************
//
void        BVGLPlugin::GetShaderInfoLog				( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
{
    glGetShaderInfoLog( shader, bufSize, length, infoLog );
}

// *****************************
//
void        BVGLPlugin::GetIntegerv						( GLenum pname, GLint * params )
{
    glGetIntegerv( pname, params );
}

// *****************************
//
void        BVGLPlugin::GetDoublev						( GLenum pname, GLdouble * params )
{
    glGetDoublev( pname, params );
}

// *****************************
//
void        BVGLPlugin::BindAttribLocation				( GLuint program, GLuint index, const GLchar * name )
{
    glBindAttribLocation( program, index, name );
}

// *****************************
//
void        BVGLPlugin::BindFragDataLocation			( GLuint program, GLuint colorNumber, const GLchar * name )
{
    glBindFragDataLocation( program, colorNumber, name );
}

// *****************************
//
GLuint      BVGLPlugin::CreateProgram					()
{
    return glCreateProgram();
}

// *****************************
//
void        BVGLPlugin::DeleteProgram					( GLuint program )
{
    glDeleteProgram( program );
}

// *****************************
//
GLuint      BVGLPlugin::CreateShader					( GLenum type )
{
    return glCreateShader( type );
}

// *****************************
//
void        BVGLPlugin::DeleteShader					( GLuint shader )
{
    glDeleteShader( shader );
}

// *****************************
//
void        BVGLPlugin::GenBuffers						( GLsizei n, GLuint * buffers )
{
    glGenBuffers( n, buffers );
}

// *****************************
//
void        BVGLPlugin::DeleteBuffers					( GLsizei n, const GLuint * buffers )
{
    glDeleteBuffers( n, buffers );
}

// *****************************
//
void        BVGLPlugin::BindBuffer						( GLenum target, GLuint buffer )
{
    glBindBuffer( target, buffer );
}

// *****************************
//
void        BVGLPlugin::BufferData						( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
{
    glBufferData( target, size, data, usage );
}

// *****************************
//
void        BVGLPlugin::BufferSubData					( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data )
{
	glBufferSubData( target, offset, size, data );
}


// *****************************
//
GLvoid *    BVGLPlugin::MapBuffer						( GLenum target, GLenum access )
{
    return glMapBuffer( target, access );
}

// *****************************
//
GLboolean    BVGLPlugin::UnmapBuffer					( GLenum target )
{
    return glUnmapBuffer( target );
}

// *****************************
//
void         BVGLPlugin::GenTextures					( GLsizei n, GLuint * textures )
{
    glGenTextures( n, textures );
}

// *****************************
//
void         BVGLPlugin::DeleteTextures					( GLsizei n, const GLuint * textures )
{
    glDeleteTextures( n, textures );
}

// *****************************
//
void         BVGLPlugin::TexImage2D						( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels )
{
    glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

// *****************************
//
void         BVGLPlugin::ActiveTexture					( GLenum texture )
{
    glActiveTexture( texture );
}

// *****************************
//
void         BVGLPlugin::TexSubImage2D					( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels )
{
    glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

// *****************************
//
void         BVGLPlugin::BindTexture					( GLenum target, GLuint texture )
{
    glBindTexture( target, texture );
}

// *****************************
//
void         BVGLPlugin::TexParameteri					( GLenum target, GLenum pname, GLint param )
{
    glTexParameteri( target, pname, param );
}

// *****************************
//
void            BVGLPlugin::TexParameterfv				( GLenum target, GLenum pname, const GLfloat * params )
{
    glTexParameterfv( target, pname, params );
}

// *****************************
//
void         BVGLPlugin::ReadBuffer						( GLenum mode )
{
    glReadBuffer( mode );
}

// *****************************
//
void         BVGLPlugin::ReadPixels						( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels )
{
    glReadPixels( x, y, width, height, format, type, pixels );
}

// *****************************
//
void         BVGLPlugin::GenFramebuffers				( GLsizei n, GLuint * framebuffers )
{
    glGenFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGLPlugin::DeleteFramebuffers				( GLsizei n, const GLuint * framebuffers )
{
    glDeleteFramebuffers( n, framebuffers );
}

// *****************************
//
void         BVGLPlugin::GenRenderbuffers				( GLsizei n, GLuint * renderbuffers )
{
    glGenRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGLPlugin::DeleteRenderbuffers			( GLsizei n, const GLuint * renderbuffers )
{
    glDeleteRenderbuffers( n, renderbuffers );
}

// *****************************
//
void         BVGLPlugin::BindRenderbuffer				( GLenum target, GLuint renderbuffer )
{
    glBindRenderbuffer( target, renderbuffer );
}

// *****************************
//
void         BVGLPlugin::RenderbufferStorage			( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
{
    glRenderbufferStorage( target, internalformat, width, height );
}

// *****************************
//
void         BVGLPlugin::BindFramebuffer				( GLenum target, GLuint framebuffer )
{
    glBindFramebuffer( target, framebuffer );
}

// *****************************
//
void         BVGLPlugin::FramebufferTexture2D			( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
    glFramebufferTexture2D( target, attachment, textarget, texture, level );
}

// *****************************
//
void         BVGLPlugin::FramebufferRenderbuffer		( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
    glFramebufferRenderbuffer( target, attachment, renderbuffertarget, renderbuffer );
}

// *****************************
//
void         BVGLPlugin::DrawBuffers					( GLsizei n, const GLenum * bufs )
{
    glDrawBuffers( n, bufs );
}

// *****************************
//
void        BVGLPlugin::DrawArrays						( GLenum mode, GLint first, GLsizei count )
{
    glDrawArrays( mode, first, count );
}

// *****************************
//
void        BVGLPlugin::DrawElements									( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
    glDrawElements( mode, count, type, indices );
}

// *****************************
//
void        BVGLPlugin::DrawArraysInstanced							( GLenum mode, GLint first, GLsizei count, GLsizei primcount )
{
    glDrawArraysInstanced( mode, first, count, primcount );
}

// *****************************
//
void        BVGLPlugin::DrawElementsInstanced						( GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount )
{
    glDrawElementsInstanced( mode, count, type, indices, primcount );
}

// *****************************
//
void        BVGLPlugin::DrawElementsBaseVertex						( GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex )
{
    glDrawElementsBaseVertex( mode, count, type, indices, basevertex );
}

// *****************************
//
void        BVGLPlugin::DrawArraysInstancedBaseInstance				( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance )
{
    glDrawArraysInstancedBaseInstance( mode, first, count, primcount, baseinstance );
}

// *****************************
//
void        BVGLPlugin::DrawElementsInstancedBaseVertexBaseInstance	( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance )
{
    glDrawElementsInstancedBaseVertexBaseInstance( mode, count, type, indices, primcount, basevertex, baseinstance );
}

// *****************************
//
void        BVGLPlugin::DrawElementsInstancedBaseVertex				( GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLsizei primcount, GLint basevertex )
{
    glDrawElementsInstancedBaseVertex( mode, count, type, indices, primcount, basevertex );
}

// *****************************
//
void        BVGLPlugin::DrawArraysIndirect							( GLenum mode, const void *indirect )
{
    glDrawArraysIndirect( mode, indirect );
}

// *****************************
//
void        BVGLPlugin::DrawElementsIndirect							( GLenum mode, GLenum type, const void *indirect )
{
    glDrawElementsIndirect( mode, type, indirect );
}

// *****************************
//
void        BVGLPlugin::MultiDrawArraysIndirect						( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    glMultiDrawArraysIndirect( mode, indirect, drawcount, stride );
}

// *****************************
//
void        BVGLPlugin::MultiDrawElementsIndirect					( GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride )
{
    glMultiDrawElementsIndirect( mode, type, indirect, drawcount, stride );
}

// *****************************
//
GLenum      BVGLPlugin::CheckFramebufferStatus			( GLenum target )
{
    return glCheckFramebufferStatus( target );
}

// *****************************
//
void        BVGLPlugin::Enable							( GLenum cap )
{
    glEnable( cap );
}

// *****************************
//
void        BVGLPlugin::Disable							( GLenum cap )
{
    glDisable( cap );
}

// *****************************
//
void        BVGLPlugin::DepthFunc						( GLenum func )
{
    glDepthFunc( func );
}

// *****************************
//
void        BVGLPlugin::DepthMask						( GLboolean flag )
{
    glDepthMask( flag );
}

// *****************************
//
void        BVGLPlugin::BlendFunc						( GLenum sfactor, GLenum dfactor )
{
    glBlendFunc( sfactor, dfactor );
}

// *****************************
//
void        BVGLPlugin::BlendColor						( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    glBlendColor( red, green, blue, alpha );
}

// *****************************
//
void        BVGLPlugin::FrontFace						( GLenum mode )
{
    glFrontFace( mode );
}

// *****************************
//
void        BVGLPlugin::CullFace						( GLenum mode )
{
    glCullFace( mode );
}

// *****************************
//
void        BVGLPlugin::PolygonMode						( GLenum face, GLenum mode )
{
    glPolygonMode( face, mode );
}

// *****************************
//
void        BVGLPlugin::PolygonOffset					( GLfloat factor, GLfloat units )
{
    glPolygonOffset( factor, units );
}

// *****************************
//
GLenum      BVGLPlugin::GetError						()
{
    return glGetError();
}

// *****************************
//
const GLubyte * BVGLPlugin::GetString					( GLenum name )
{
    return glGetString( name );
}

// *****************************
//
const GLubyte * BVGLPlugin::GetStringi					( GLenum name, GLuint index )
{
    return glGetStringi( name, index );
}

// *****************************
//
void            BVGLPlugin::GenVertexArrays				( GLsizei n, GLuint * arrays )
{
    glGenVertexArrays( n, arrays );
}

// *****************************
//
void            BVGLPlugin::DeleteVertexArrays			( GLsizei n, const GLuint * arrays )
{
    glDeleteVertexArrays( n, arrays );
}

// *****************************
//
void            BVGLPlugin::EnableVertexAttribArray		( GLuint index )
{
    glEnableVertexAttribArray( index );
}

// *****************************
//
void            BVGLPlugin::DisableVertexAttribArray	( GLuint index )
{
    glDisableVertexAttribArray( index );
}

// *****************************
//
void            BVGLPlugin::BindVertexArray				( GLuint array )
{
    glBindVertexArray( array );
}

// *****************************
//
void            BVGLPlugin::VertexAttribPointer			( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    glVertexAttribPointer( index, size, type, normalized, stride, pointer );
}



//pablito
// *****************************
//
void				BVGLPlugin::BindFramebufferEXT			(GLenum target, GLuint framebuffer)
{
	glBindFramebufferEXT(target,framebuffer);
}
// *****************************
//
void				BVGLPlugin::BindRenderbufferEXT			(GLenum target, GLuint renderbuffer)
{
	glBindRenderbufferEXT(target,renderbuffer);
}
// *****************************
//
void				BVGLPlugin::DeleteFramebufferEXT		(GLsizei n, const GLuint *framebuffers)
{
	glDeleteFramebuffersEXT(n, framebuffers);
}
// *****************************
//
void				BVGLPlugin::DeleteRenderbufferEXT		(GLsizei n, const GLuint *renderbuffers)
{
	glDeleteRenderbuffersEXT(n,renderbuffers);
}

// *****************************
//
void				BVGLPlugin::Finish		()
{
	glFinish();
}


// *****************************
//
void				BVGLPlugin::Flush		()
{
	glFlush();
}


// *****************************
//
/*GLuint				GetError					()
{
	return glGetError();
}*/

// *****************************
//
void			BVGLPlugin::VertexAttribDivisor			( GLuint index,  GLuint divisor )
{
    glVertexAttribDivisor( index, divisor );
}

// *****************************
//
void			BVGLPlugin::PrintShortSummary           ( const std::string & )
{
}

// *****************************
//
void			BVGLPlugin::PrintCompleteSummary        ( const std::string & )
{
}



} //bv
