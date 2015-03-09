#pragma once

#include "BVGLDefaultPlugin.h"
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

class BVGLResourceTrackingPlugin : public BVGLDefaultPlugin
{
private:

    typedef BVGLDefaultPlugin Parent;

private:

    static BVGLResourceTrackingPlugin   bvgl;

    std::hash_map< GLuint, BufferDesc > m_allocatedBuffers;
    std::hash_map< GLenum, GLuint >     m_boundBuffers;

public:

                                BVGLResourceTrackingPlugin  ();
                                ~BVGLResourceTrackingPlugin ();

    static void                 bvglGenBuffers              ( GLsizei n, GLuint * buffers );
    static void                 bvglDeleteBuffers           ( GLsizei n, const GLuint * buffers );
    static void                 bvglBindBuffer              ( GLenum target, GLuint buffer );
    static void                 bvglBufferData              ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );
    
private:

    void                        GenBuffers                  ( GLsizei n, GLuint * buffers );
    void                        DeleteBuffers               ( GLsizei n, const GLuint * buffers );
    void                        BindBuffer                  ( GLenum target, GLuint buffer );
    void                        BufferData                  ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );
    
public:

    static void                 PrintStats                  ();

private:

    void                        PrintBuffersStats           ();

/*
    static void                 bvglGenTextures             ( GLsizei n, GLuint * textures );
    static void                 bvglDeleteTextures          ( GLsizei n, const GLuint * textures );

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
