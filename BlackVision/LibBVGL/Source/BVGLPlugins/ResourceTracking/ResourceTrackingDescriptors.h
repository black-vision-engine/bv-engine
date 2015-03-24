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

    BufferDesc  ();
};

/////////////////////////////////////
//
struct TextureDesc
{
    GLsizei width;
    GLsizei height;
    GLenum format;
    const GLvoid * pixels;

    TextureDesc ();

    GLuint DataSize () const;
    GLuint PixelSize( GLenum format ) const;
};

/////////////////////////////////////
//
struct RenderbufferDesc
{
    GLenum  internalformat;
    GLsizei width;
    GLsizei height;

    RenderbufferDesc ();

};

} // bv
