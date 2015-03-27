#include "ResourceTrackingDescriptors.h"

#include <cassert>


namespace bv {

// *************************************************************************************************************************************************

// *****************************
//
BufferDesc::BufferDesc              ()
    : size( 0 )
    , usage( 0 )
    , data( nullptr )
{
}

// *****************************
//
void    BufferDesc::Set             ( GLsizeiptr size, GLenum usage, const GLvoid * data )
{
    this->size  = size;
    this->usage = usage;
    this->data  = data;
}

// *****************************
//
void    BufferDesc::Visit           ( VisitorAggregate & visitor ) const
{
    visitor.AddToTotalSize( size );
    visitor.IncNumVisits();
}

// *****************************
//
std::string  BufferDesc::Summary    () const
{
    return "IMPLEMENT ME";
}

// *****************************
//
std::string  BufferDesc::TargetStr  ( GLenum target ) const
{
    return "IMPLEMENT ME";
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

// *****************************
//
void    TextureDesc::Visit  ( VisitorAggregate & visitor ) const
{
    visitor.AddToTotalSize( DataSize() );
    visitor.IncNumVisits();
}

// *****************************
//
std::string  TextureDesc::Summary   () const
{
    return "IMPLEMENT ME";
}

// *****************************
//
std::string  TextureDesc::TargetStr ( GLenum target ) const
{
    return "IMPLEMENT ME";
}

// *************************************************************************************************************************************************

// *****************************
//
RenderbufferDesc::RenderbufferDesc      ()
    : internalformat( 0 )
    , width( 0 )
    , height( 0 )
{
}

// *****************************
//
void    RenderbufferDesc::Set           ( GLenum internalformat, GLsizei width, GLsizei height )
{
    this->internalformat    = internalformat;
    this->width             = width;
    this->height            = height;
}

// *****************************
//
GLuint  RenderbufferDesc::BufferSize    () const
{
    return width * height * PixelSize( internalformat );
}

// *****************************
//
GLuint  RenderbufferDesc::PixelSize     ( GLenum internalformat ) const
{
    assert( internalformat == GL_DEPTH_COMPONENT );

    return 3; // FIXME: make sure that there is no 1-byte padding 
}

// *****************************
//
void    RenderbufferDesc::Visit         ( VisitorAggregate & visitor ) const
{
    visitor.AddToTotalSize( BufferSize() );
    visitor.IncNumVisits();
}

// *****************************
//
std::string  RenderbufferDesc::Summary  () const
{
    return "IMPLEMENT ME";
}

// *****************************
//
std::string  RenderbufferDesc::TargetStr( GLenum target ) const
{
    return "IMPLEMENT ME";
}

// *************************************************************************************************************************************************

// *****************************
//
FramebufferDesc::FramebufferDesc    ()
    : colorAttachment0( 0 )
    , attachment0IsTex( false )
    , colorAttachment1( 0 )
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
        colorAttachment0 = texture;
        attachment0IsTex = true;
    }
    else if( attachment == ( GL_COLOR_ATTACHMENT0 + 1 ) )
    {
        colorAttachment1 = texture;
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

// *****************************
//
void    FramebufferDesc::Visit              ( VisitorAggregate & visitor ) const
{
    if( colorAttachment0 > 0 )
    {
        attachment0IsTex ? visitor.IncNumAtch0Tex() : visitor.IncNumAtch0FB();
    }

    if( colorAttachment1 > 0 )
    {
        attachment1IsTex ? visitor.IncNumAtch1Tex() : visitor.IncNumAtch1FB();
    }

    if( depthAttachment > 0 )
    {
        depthAttachmentIsTex ? visitor.IncNumDBAtTex() : visitor.IncNumDBAtFB();
    }

    visitor.IncNumVisits();
}

// *****************************
//
std::string  FramebufferDesc::Summary   () const
{
    return "IMPLEMENT ME";
}

// *****************************
//
std::string  FramebufferDesc::TargetStr ( GLenum target ) const
{
    return "IMPLEMENT ME";
}

// *************************************************************************************************************************************************

// *****************************
//
VertexArrayAttribDesc::VertexArrayAttribDesc   ()
{
}

// *****************************
//
VertexArrayAttribDesc::VertexArrayAttribDesc   ( GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
    : enabled( false )
{
    Set( buffer, index, size, type, normalized, stride, pointer );
}

// *****************************
//
void    VertexArrayAttribDesc::Set ( GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    this->buffer        = buffer;
    this->index         = index;      
    this->size          = size;
    this->type          = type;
    this->normalized    = normalized;
    this->stride        = stride;
    this->pointer       = pointer;
}

// *****************************
//
std::string  VertexArrayAttribDesc::Summary () const
{
    return "IMPLEMENT ME";
}

// *************************************************************************************************************************************************

// *****************************
//
VertexArrayDesc::VertexArrayDesc        ()
{
}

// *****************************
//
void    VertexArrayDesc::SetAttrPointer ( GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    if( attributePointers.find( index ) != attributePointers.end() )
    {
        attributePointers[ index ].Set( buffer, index, size, type, normalized, stride, pointer );
    }
    else
    {
        attributePointers[ index ] = VertexArrayAttribDesc( buffer, index, size, type, normalized, stride, pointer );
    }
}

// *****************************
//
void    VertexArrayDesc::Enable         ( GLuint index )
{
    if( attributePointers.find( index ) != attributePointers.end() )
    {
        attributePointers[ index ] = VertexArrayAttribDesc();
    }
    
    attributePointers[ index ].enabled = true;
    enabledAttributes.insert( index );
}

// *****************************
//
void    VertexArrayDesc::Disable        ( GLuint index )
{
    if( attributePointers.find( index ) != attributePointers.end() )
    {
        attributePointers[ index ] = VertexArrayAttribDesc();
    }

    attributePointers[ index ].enabled = false;
    enabledAttributes.erase( index );
}

// *****************************
//
void    VertexArrayDesc::Visit          ( VisitorAggregate & visitor ) const
{
    visitor.AddToTotalRegisteredAttrs   ( attributePointers.size() );
    visitor.AddToTotalEnabledAttrs      ( enabledAttributes.size() );
    visitor.IncNumVisits                ();
}

// *****************************
//
std::string  VertexArrayDesc::Summary   () const
{
    return "IMPLEMENT ME";
}

// *****************************
//
std::string  VertexArrayDesc::TargetStr ( GLenum target ) const
{
    return "IMPLEMENT ME";
}

} // bv
