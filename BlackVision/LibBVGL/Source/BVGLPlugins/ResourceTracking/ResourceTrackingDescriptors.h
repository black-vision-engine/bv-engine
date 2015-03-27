#pragma once

#include <vector>
#include <hash_map>
#include <hash_set>
#include <string>

#include "BVGLPlugins/ResourceTracking/ResourceTrackingAggregates.h"
#include "BVGLDefs.h"

using std::string;

namespace bv {

/////////////////////////////////////
//
struct BufferDesc
{
    typedef AggregateSizeTracker VisitorAggregate;

    GLsizeiptr      size;
    GLenum          usage;
    const GLvoid *  data;

    BufferDesc      ();

    void    Set     ( GLsizeiptr size, GLenum usage, const GLvoid * data );

    void    Visit   ( VisitorAggregate & visitor ) const;
    
    string  Summary () const;
};

/////////////////////////////////////
//
struct TextureDesc
{
    typedef AggregateSizeTracker VisitorAggregate;

    GLsizei         width;
    GLsizei         height;
    GLenum          format;
    const GLvoid *  pixels;

    TextureDesc         ();

    void    Set         ( GLsizei width, GLsizei height, GLenum format, const GLvoid * pixels );

    GLuint  DataSize    () const;
    GLuint  PixelSize   ( GLenum format ) const;

    void    Visit       ( VisitorAggregate & visitor ) const;

    string  Summary () const;
};

/////////////////////////////////////
//
struct RenderbufferDesc
{
    typedef AggregateSizeTracker VisitorAggregate;

    GLenum  internalformat;
    GLsizei width;
    GLsizei height;

    RenderbufferDesc    ();

    void    Set         ( GLenum internalformat, GLsizei width, GLsizei height );

    GLuint  BufferSize  () const;
    GLuint  PixelSize   ( GLenum internalformat ) const;

    void    Visit       ( VisitorAggregate & visitor ) const;

    string  Summary () const;
};

/////////////////////////////////////
//
struct FramebufferDesc
{
    typedef AggregateFrameBufferTracker VisitorAggregate;

    // FIXME: only rudimentary tracking with textures used as color attachments and 
    GLuint colorAttachment0;
    bool   attachment0IsTex;

    GLuint colorAttachment1;
    bool   attachment1IsTex;

    GLuint depthAttachment;
    bool   depthAttachmentIsTex;

    FramebufferDesc             ();

    void    AttachTexture2D     ( GLenum attachment, GLenum target, GLuint texture, GLint level );
    void    AttachRenderbuffer  ( GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

    void    Visit               ( VisitorAggregate & visitor ) const;

    string  Summary () const;
};


/////////////////////////////////////
//
struct VertexArrayAttribDesc
{
    GLuint          buffer;
    GLuint          index;
 	GLint           size;
 	GLenum          type;
 	GLboolean       normalized;
 	GLsizei         stride;
 	const GLvoid *  pointer;
    bool            enabled;

    VertexArrayAttribDesc   ();
    VertexArrayAttribDesc   ( GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    void    Set             ( GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    string  Summary         () const;
};

/////////////////////////////////////
//
struct VertexArrayDesc
{
    typedef AggregateVertexArrayTracker VisitorAggregate;

    std::hash_set< GLuint >                         enabledAttributes;
    std::hash_map< GLuint, VertexArrayAttribDesc >  attributePointers;

    VertexArrayDesc         ();

    void    SetAttrPointer  ( GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    void    Enable          ( GLuint index );
    void    Disable         ( GLuint index );

    void    Visit           ( VisitorAggregate & visitor ) const;

    string  Summary () const;
};

} // bv
