#pragma once

#include <vector>
#include <hash_map>
#include <hash_set>


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
    // FIXME: onlu rudimentary tracking with textures used as color attachments and 
    GLuint textureAttachment0;
    bool   attachment0IsTex;

    GLuint textureAttachment1;
    bool   attachment1IsTex;

    GLuint depthAttachment;
    bool   depthAttachmentIsTex;

    FramebufferDesc             ();

    void    AttachTexture2D     ( GLenum attachment, GLenum target, GLuint texture, GLint level );
    void    AttachRenderbuffer  ( GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );
};


/////////////////////////////////////
//
struct VertexArrayAttribDesc
{
    GLuint          index;
 	GLint           size;
 	GLenum          type;
 	GLboolean       normalized;
 	GLsizei         stride;
 	const GLvoid *  pointer;
    bool            enabled;

    VertexArrayAttribDesc   ();
    VertexArrayAttribDesc   ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    void    Set             ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

};

/////////////////////////////////////
//
struct VertexArrayDesc
{

    std::hash_set< GLuint >                         enabledAttributes;
    std::hash_map< GLuint, VertexArrayAttribDesc >  attributePointers;

    VertexArrayDesc         ();

    void    SetAttrPointer  ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    void    Enable          ( GLuint index );
    void    Disable         ( GLuint index );
};

} // bv
