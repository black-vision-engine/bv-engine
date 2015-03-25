#pragma once

#include "BVGLDefs.h"


namespace bv {

/////////////////////////////////////
//
struct BufferDesc
{
    GLsizeiptr      size;
    GLenum          usage;
    const GLvoid *  data;

    BufferDesc      ();

    void    Set     ( GLsizeiptr size, GLenum usage, const GLvoid * data );

};

/////////////////////////////////////
//
struct TextureDesc
{
    GLsizei         width;
    GLsizei         height;
    GLenum          format;
    const GLvoid *  pixels;

    TextureDesc         ();

    void    Set         ( GLsizei width, GLsizei height, GLenum format, const GLvoid * pixels );

    GLuint  DataSize    () const;
    GLuint  PixelSize   ( GLenum format ) const;

};

/////////////////////////////////////
//
struct RenderbufferDesc
{
    GLenum  internalformat;
    GLsizei width;
    GLsizei height;

    RenderbufferDesc    ();

    void    Set         ( GLenum internalformat, GLsizei width, GLsizei height );

    GLuint  BufferSize  () const;
    GLuint  PixelSize   ( GLenum internalformat ) const;

};

/////////////////////////////////////
//
struct FramebufferDesc
{
    FramebufferDesc ();

    void    Set     ();
    //void    Set     ();
};

/////////////////////////////////////
//
struct VertexArrayDesc
{
    VertexArrayDesc ();

    void    Set     ();

};

} // bv
