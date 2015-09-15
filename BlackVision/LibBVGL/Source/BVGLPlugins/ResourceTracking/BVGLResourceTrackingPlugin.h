#pragma once

#include "BVGLPlugins/BVGLPlugin.h"

#include "BVGLPlugins/ResourceTracking/ResourceTrackingDescriptors.h"
#include "BVGLPlugins/ResourceTracking/TrackableResources.h"

#include <hash_map>
#include <cassert>

namespace bv {

class BVGLResourceTrackingPlugin : public BVGLPlugin
{

    typedef BVGLPlugin Parent;

private:

    TrackableResources< BufferDesc >            m_buffers;
    TrackableResources< TextureDesc >           m_textures;
    TrackableResources< FramebufferDesc >       m_framebuffers;
    TrackableResources< RenderbufferDesc >      m_renderbuffers;
    TrackableResources< VertexArrayDesc >       m_vertexarrays;

public:

                        BVGLResourceTrackingPlugin  ();
                        ~BVGLResourceTrackingPlugin	();

    virtual void		GenBuffers                  ( GLsizei n, GLuint * buffers ) override;
    virtual void		DeleteBuffers               ( GLsizei n, const GLuint * buffers ) override;
    virtual void		BindBuffer                  ( GLenum target, GLuint buffer ) override;
    virtual void		BufferData                  ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage ) override;

    virtual void        GenTextures                 ( GLsizei n, GLuint * textures ) override;
    virtual void        DeleteTextures              ( GLsizei n, const GLuint * textures );
    virtual void        TexImage2D					( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels ) override;
    virtual void        ActiveTexture				( GLenum texture ) override;
    virtual void        TexSubImage2D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels ) override;
    virtual void        BindTexture					( GLenum target, GLuint texture ) override;

    virtual void        GenRenderbuffers            ( GLsizei n, GLuint * renderbuffers ) override;
    virtual void        DeleteRenderbuffers         ( GLsizei n, const GLuint * renderbuffers ) override;
    virtual void        BindRenderbuffer            ( GLenum target, GLuint renderbuffer ) override;
    virtual void        RenderbufferStorage         ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height ) override;

    virtual void        GenFramebuffers             ( GLsizei n, GLuint * framebuffers ) override;
    virtual void        DeleteFramebuffers          ( GLsizei n, const GLuint * framebuffers ) override;
    virtual void        BindFramebuffer             ( GLenum target, GLuint framebuffer ) override;
    virtual void        FramebufferTexture2D		( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level ) override;
    virtual void        FramebufferRenderbuffer		( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer ) override;

    virtual void        GenVertexArrays             ( GLsizei n, GLuint * arrays ) override;
    virtual void        DeleteVertexArrays          ( GLsizei n, const GLuint * arrays ) override;
    virtual void        VertexAttribPointer         ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer ) override;
    virtual void        BindVertexArray				( GLuint array );
    virtual void        EnableVertexAttribArray		( GLuint index );
    virtual void        DisableVertexAttribArray	( GLuint index );

    virtual void		PrintShortSummary           ( const std::string & message ) override;
    virtual void		PrintCompleteSummary        ( const std::string & message ) override;

#ifdef BV_GL_VERSION_4_4
	virtual void		BindTextures				( GLuint first, GLsizei count, const GLuint* textures ) override;
#endif
#ifdef BV_GL_VERSION_4_5
	virtual void		BindTextureUnit				( GLuint unit, GLuint texture ) override;

	virtual void		TextureStorage1D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width ) override;
	virtual void		TextureStorage2D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height ) override;
	virtual void		TextureStorage3D			( GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth ) override;
	virtual void		TextureStorage2DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations ) override;
	virtual void		TextureStorage3DMultisample	( GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations ) override;
#endif

	virtual void		CompressedTexImage1D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid* data ) override;
	virtual void		CompressedTexImage2D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data ) override;
	virtual void		CompressedTexImage3D		( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data ) override;
	virtual void		CompressedTexSubImage1D		( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid* data ) override;
	virtual void		CompressedTexSubImage2D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data ) override;
	virtual void		CompressedTexSubImage3D		( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data ) override;

	virtual void		TexImage1D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data ) override;
	virtual void		TexImage3D					( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data ) override;
	virtual void		TexImage2DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations ) override;
	virtual void		TexImage3DMultisample		( GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations ) override;

	virtual void		TexSubImage1D				( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels ) override;
	virtual void		TexSubImage3D				( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels ) override;

	virtual void		TexStorage1D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width ) override;
	virtual void		TexStorage2D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height ) override;
	virtual void		TexStorage3D				( GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth ) override;
	virtual void		TexStorage2DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations ) override;
	virtual void		TexStorage3DMultisample		( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations ) override;

	virtual void		TexBuffer					( GLenum target, GLenum internalFormat, GLuint buffer ) override;	
	virtual void		TexBufferRange				( GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size ) override;

	virtual void		BindBufferBase				( GLenum target, GLuint index, GLuint buffer ) override;
	virtual void		BindBufferRange				( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size ) override;
	virtual void		FramebufferTexture			( GLenum target, GLenum attachment, GLuint texture, GLint level ) override;
	virtual void		FramebufferTextureLayer		( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer ) override;
	virtual void		RenderbufferStorageMultisample			( GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height ) override;

#ifdef BV_GL_VERSION_4_5
	virtual void		NamedFramebufferTexture		( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level ) override;
	virtual void		NamedFramebufferTextureLayer( GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer ) override;
	virtual void		NamedFramebufferRenderbuffer( GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer ) override;

	virtual void		NamedRenderbufferStorage				( GLuint renderbuffer, GLenum internalFormat, GLsizei width, GLsizei height ) override;
	virtual void		NamedRenderbufferStorageMultisample		( GLuint renderbuffer, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height ) override;
#endif
#ifdef BV_GL_VERSION_4_4
	virtual void		BufferStorage				( GLenum target, GLsizeiptr size, const GLvoid* data, GLbitfield flags ) override;
#endif
};

} //bv
