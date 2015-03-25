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
{
}

// *****************************
//
void    FramebufferDesc::Set        ()
{
}

// *************************************************************************************************************************************************

// *****************************
//
VertexArrayDesc::VertexArrayDesc    ()
{
}

// *****************************
//
void    VertexArrayDesc::Set        ()
{
}

} // bv
