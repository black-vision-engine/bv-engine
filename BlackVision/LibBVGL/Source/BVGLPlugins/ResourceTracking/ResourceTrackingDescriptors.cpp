#include "ResourceTrackingDescriptors.h"

#include <cassert>


namespace bv {

////////////////////////////////
//
BufferDesc::BufferDesc()
    : size( 0 )
    , usage( 0 )
    , data( nullptr )
{
}

////////////////////////////////
//
TextureDesc::TextureDesc()
    : width( 0 )
    , height( 0 )
    , format( 0 )
    , pixels( nullptr )
{
}

////////////////////////////////
//
GLuint TextureDesc::DataSize() const
{
    return width * height * PixelSize( format );
}

////////////////////////////////
//
GLuint TextureDesc::PixelSize( GLenum format )
{
    assert( format == GL_RGBA || format == GL_BGRA );

    return 4;
}

////////////////////////////////
//
RenderbufferDesc::RenderbufferDesc()
    : internalformat( 0 )
    , width( 0 )
    , height( 0 )
{
}

} // bv
