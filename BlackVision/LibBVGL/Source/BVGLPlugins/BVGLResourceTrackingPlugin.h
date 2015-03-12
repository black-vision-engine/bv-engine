#pragma once

#include "BVGLPlugin.h"

#include <hash_map>


namespace bv {

struct BufferDesc
{
    GLsizeiptr      size;
    GLenum          usage;
    const GLvoid *  data;

    BufferDesc()
        : size( 0 )
        , usage( 0 )
        , data( 0 )
    {
    }
};

class BVGLResourceTrackingPlugin : public BVGLPlugin
{

    typedef BVGLPlugin Parent;

private:

    std::hash_map< GLuint, BufferDesc > m_allocatedBuffers;
    std::hash_map< GLenum, GLuint >     m_boundBuffers;

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

    virtual void        GenRenderbuffers            ( GLsizei n, GLuint * renderbuffers );
    virtual void        DeleteRenderbuffers         ( GLsizei n, const GLuint * renderbuffers );

    virtual void		PrintStats                  () override;

private:

    void                PrintBuffersStats           ();

/*

    static void                 bvglGenFramebuffers         ( GLsizei n, GLuint * framebuffers );
    static void                 bvglDeleteFramebuffers      ( GLsizei n, const GLuint * framebuffers );
    
    static void                 bvglGenRenderbuffers        ( GLsizei n, GLuint * renderbuffers );
    static void                 bvglDeleteRenderbuffers     ( GLsizei n, const GLuint * renderbuffers );

    static void                 bvglBindRenderbuffer        ( GLenum target, GLuint renderbuffer );
    static void                 bvglRenderbufferStorage     ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );

    static void                 bvglBindFramebuffer         ( GLenum target, GLuint framebuffer );
    static void                 bvglGenVertexArrays         ( GLsizei n, GLuint * arrays );
    static void                 bvglDeleteVertexArrays      ( GLsizei n, const GLuint * arrays );
    static void                 bvglVertexAttribPointer     ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );
*/
};

} //bv
