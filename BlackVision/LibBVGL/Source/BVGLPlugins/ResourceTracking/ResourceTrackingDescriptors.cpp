#include "ResourceTrackingDescriptors.h"

#include <cassert>


namespace bv {

// *************************************************************************************************************************************************

// *****************************
//
BufferDesc::BufferDesc()
    : size( 0 )
    , usage( 0 )
    , data( nullptr )
{
}

// *****************************
//
void    BufferDesc::Set     ( GLsizeiptr size, GLenum usage, const GLvoid * data )
{
    this->size  = size;
    this->usage = usage;
    this->data  = data;
}

// *************************************************************************************************************************************************

// *****************************
//
TextureDesc::TextureDesc()
    : width( 0 )
    , height( 0 )
    , format( 0 )
    , pixels( nullptr )
{
}

// *****************************
//
void    TextureDesc::Set    ( GLsizei width, GLsizei height, GLenum format, const GLvoid * pixels )
{
    this->width     = width;
    this->height    = height;
    this->format    = format;
    this->pixels    = pixels;
}

// *****************************
//
GLuint TextureDesc::DataSize() const
{
    return width * height * PixelSize( format );
}

// *****************************
//
GLuint TextureDesc::PixelSize( GLenum format ) const
{
    assert( format == GL_RGBA || format == GL_BGRA );

    return 4;
}

// *************************************************************************************************************************************************

// *****************************
//
RenderbufferDesc::RenderbufferDesc  ()
    : internalformat( 0 )
    , width( 0 )
    , height( 0 )
{
}

// *****************************
//
void    RenderbufferDesc::Set       ( GLenum internalformat, GLsizei width, GLsizei height )
{
    this->internalformat    = internalformat;
    this->width             = width;
    this->height            = height;
}

// *****************************
//
GLuint  RenderbufferDesc::BufferSize  () const
{
    return width * height * PixelSize( internalformat );
}

// *****************************
//
GLuint  RenderbufferDesc::PixelSize   ( GLenum internalformat ) const
{
    assert( internalformat == GL_DEPTH_COMPONENT );

    return 3; // FIXME: make sure that there is no 1-byte padding 
}

// *************************************************************************************************************************************************

// *****************************
//
FramebufferDesc::FramebufferDesc    ()
    : textureAttachment0( 0 )
    , attachment0IsTex( false )
    , textureAttachment1( 0 )
    , attachment1IsTex( false )
    , depthAttachment( 0 )
    , depthAttachmentIsTex( false )
{
}

// *****************************
//
void    FramebufferDesc::AttachTexture2D    ( GLenum attachment, GLenum target, GLuint texture, GLint level )
{
    ( target );
 
    assert( attachment == GL_COLOR_ATTACHMENT0 || attachment == ( GL_COLOR_ATTACHMENT0 + 1 ) );
    assert( level == 0 );

    if( attachment == GL_COLOR_ATTACHMENT0 )
    {
        textureAttachment0 = texture;
        attachment0IsTex = true;
    }
    else if( attachment == ( GL_COLOR_ATTACHMENT0 + 1 ) )
    {
        textureAttachment1 = texture;
        attachment1IsTex = true;
    }
}

// *****************************
//
void    FramebufferDesc::AttachRenderbuffer ( GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
    assert( GL_DEPTH_ATTACHMENT == attachment );
    assert( GL_RENDERBUFFER == renderbuffertarget );

    depthAttachment = renderbuffer;
    depthAttachmentIsTex = false;
}

// *************************************************************************************************************************************************

// *****************************
//
void    VertexArrayAttribDesc::Set ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    this->index         = index;      
    this->size          = size;
    this->type          = type;
    this->normalized    = normalized;
    this->stride        = stride;
    this->pointer       = pointer;     
}

// *************************************************************************************************************************************************

// *****************************
//
VertexArrayDesc::VertexArrayDesc        ()
{
}

// *****************************
//
void    VertexArrayDesc::AttrPointer    ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    auto d = VertexArrayAttribDesc();
    
    d.Set( index, size, type, normalized, stride, pointer );

    attributePointers[ index ] = d;
    d.enabled = false;
}

// *****************************
//
void    VertexArrayDesc::Enable         ( GLuint index )
{
    assert( attributePointers.find( index ) != attributePointers.end() );
    
    attributePointers[ index ].enabled = true;
    enabledAttributes[ index ] = index;
}

// *****************************
//
void    VertexArrayDesc::Disable     ( GLuint index )
{
    assert( attributePointers.find( index ) != attributePointers.end() );

    attributePointers[ index ].enabled = false;
    enabledAttributes.erase( index );
}

} // bv
