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

private:

    void                PrintBuffersStats           ();
    void                PrintTextureStats           ();
    void                PrintRenderbuffersStats     ();
    void                PrintFramebufersStats       ();

};

} //bv
