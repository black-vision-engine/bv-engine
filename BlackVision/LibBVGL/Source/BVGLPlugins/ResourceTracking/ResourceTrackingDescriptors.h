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

    GLuint          ID;
    GLsizeiptr      size;
    GLenum          usage;
    const GLvoid *  data;

    BufferDesc      ();

    void    SetID   ( GLuint ID );
    void    Set     ( GLsizeiptr size, GLenum usage, const GLvoid * data );

    void    Visit   ( VisitorAggregate & visitor ) const;
    
    string  Summary     () const;
    string  TargetStr   ( GLenum target ) const;
};

/////////////////////////////////////
//
struct TextureDesc
{
    typedef AggregateSizeTracker VisitorAggregate;

    GLuint          ID;
    GLsizei         width;
    GLsizei         height;
	GLsizei			depth;
    GLenum          format;
	GLenum			type;
	GLint			mipmapLevels;
    const GLvoid *  pixels;

    TextureDesc         ();

	void	SetTypeIfFirstBind		( GLenum target );
    void    SetID					( GLuint ID );
    void    Set						( GLsizei width, GLsizei height, GLsizei depth, GLenum format, const GLvoid * pixels );
	void	NewMipmapLevel			( GLint level );
	GLint	GetMipmapLevel			() { return mipmapLevels; }

    GLuint  DataSize    () const;
    GLuint  PixelSize   ( GLenum format ) const;

    void    Visit       ( VisitorAggregate & visitor ) const;

    string  Summary     () const;
    string  TargetStr   ( GLenum target ) const;
};

/////////////////////////////////////
//
struct RenderbufferDesc
{
    typedef AggregateSizeTracker VisitorAggregate;

    GLuint  ID;
    GLenum  internalformat;
    GLsizei width;
    GLsizei height;

    RenderbufferDesc    ();

    void    SetID       ( GLuint ID );
    void    Set         ( GLenum internalformat, GLsizei width, GLsizei height );

    GLuint  BufferSize  () const;
    GLuint  PixelSize   ( GLenum internalformat ) const;

    void    Visit       ( VisitorAggregate & visitor ) const;

    string  Summary     () const;
    string  TargetStr   ( GLenum target ) const;
};

/////////////////////////////////////
//
struct FramebufferDesc
{
    typedef AggregateFrameBufferTracker VisitorAggregate;

    GLuint  ID;

    // FIXME: only rudimentary tracking with textures used as color attachments and 
    GLuint colorAttachment0;
    bool   attachment0IsTex;

    GLuint colorAttachment1;
    bool   attachment1IsTex;

    GLuint depthAttachment;
    bool   depthAttachmentIsTex;

    FramebufferDesc             ();

    void    SetID               ( GLuint ID );

    void    AttachTexture2D     ( GLenum attachment, GLenum target, GLuint texture, GLint level );
    void    AttachRenderbuffer  ( GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );

    void    Visit               ( VisitorAggregate & visitor ) const;

    string  Summary             () const;
    string  TargetStr           ( GLenum target ) const;
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

    GLuint ID;
    
    std::hash_set< GLuint >                         enabledAttributes;
    std::hash_map< GLuint, VertexArrayAttribDesc >  attributePointers;

    VertexArrayDesc         ();

    void    SetID           ( GLuint ID );
    void    SetAttrPointer  ( GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );

    void    Enable          ( GLuint index );
    void    Disable         ( GLuint index );

    void    Visit           ( VisitorAggregate & visitor ) const;

    string  Summary         () const;
    string  TargetStr       ( GLenum target ) const;
};

} // bv
