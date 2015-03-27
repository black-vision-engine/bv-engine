#include "ResourceTrackingDescriptors.h"

#include <cassert>
#include <sstream>
#include <iomanip>

#include "BVGLTools/BVGLTranslator.h"
#include "BVGLTools/BVGLStringFormatters.h"


namespace bv {

// *************************************************************************************************************************************************

// *****************************
//
BufferDesc::BufferDesc              ()
    : ID( 0 )
    , size( 0 )
    , usage( 0 )
    , data( nullptr )
{
}

// *****************************
//
void    BufferDesc::SetID           ( GLuint ID )
{
    this->ID = ID;
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
    //printf( "Buf: %2d, Size: %7s, Usage: %15s", bufId, siz.c_str(), BVGLTranslator::TranslateBufferUsage( bufDesc.usage ).c_str() )
    std::stringstream ss;

    ss << "Buf: " << std::setw( 2 ) << std::setfill( ' ' ) << ID << " ";
    ss << "Size: " << std::setw( 10 ) << FormatSizeString( size ) << " ";
    ss << "Usage: " << std::setw( 15 ) << BVGLTranslator::TranslateBufferUsage( usage );

    return ss.str();
}

// *****************************
//
std::string  BufferDesc::TargetStr  ( GLenum target ) const
{
    return BVGLTranslator::TranslateBufferTarget( target );
}

// *************************************************************************************************************************************************

// *****************************
//
TextureDesc::TextureDesc()
    : ID( 0 )
    , width( 0 )
    , height( 0 )
    , format( 0 )
    , pixels( nullptr )
{
}

// *****************************
//
void    TextureDesc::SetID  ( GLuint ID )
{
    this->ID = ID;
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
    //printf( "Tex: %2d (%4d, %4d), Size: %7s, Format: %10s", texId, texDesc.width, texDesc.height, siz.c_str(), BVGLTranslator::TranslateTextureFormat( texDesc.format ).c_str() );
    std::stringstream ss;

    ss << "Tex: " << std::setw( 2 ) << std::setfill( ' ' ) << ID << " (" << std::setw( 4 ) << width << ", " << std::setw( 4 ) << height << "), ";
    ss << "Size: " << std::setw( 7 ) << FormatSizeString( DataSize() ) << " ";
    ss << "Format: " << std::setw( 10 ) << BVGLTranslator::TranslateTextureFormat( format );

    return ss.str();
}

// *****************************
//
std::string  TextureDesc::TargetStr ( GLenum target ) const
{
    return BVGLTranslator::TranslateTextureTarget( target );
}

// *************************************************************************************************************************************************

// *****************************
//
RenderbufferDesc::RenderbufferDesc      ()
    : ID( 0 )
    , internalformat( 0 )
    , width( 0 )
    , height( 0 )
{
}

// *****************************
//
void    RenderbufferDesc::SetID         ( GLuint ID )
{
    this->ID = ID;
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
    //printf( "RenderBuffer: %2d (%4d, %4d), Size: %7s, Internal Format: %10s", ID, width, height, siz.c_str(), BVGLTranslator::TranslateRenderbufferInternalFormat( internalformat ).c_str() );
    std::stringstream ss;

    ss << "RenderBuffer: " << std::setw( 2 ) << std::setfill( ' ' ) << ID << " (" << std::setw( 4 ) << width << ", " << std::setw( 4 ) << height << "), ";
    ss << "Size: " << std::setw( 7 ) << FormatSizeString( BufferSize() ) << " ";
    ss << "Internal format: " << std::setw( 10 ) << BVGLTranslator::TranslateRenderbufferInternalFormat( internalformat );

    return ss.str();
}

// *****************************
//
std::string  RenderbufferDesc::TargetStr( GLenum target ) const
{
    return BVGLTranslator::TranslateRenderbufferTarget( target );
}

// *************************************************************************************************************************************************

// *****************************
//
FramebufferDesc::FramebufferDesc            ()
    : ID( 0 )
    , colorAttachment0( 0 )
    , attachment0IsTex( false )
    , colorAttachment1( 0 )
    , attachment1IsTex( false )
    , depthAttachment( 0 )
    , depthAttachmentIsTex( false )
{
}

// *****************************
//
void    FramebufferDesc::SetID              ( GLuint ID )
{
    this->ID = ID;
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
    std::stringstream ss;

    ss << "FrameBuffer: " << std::setw( 2 ) << std::setfill( ' ' ) << ID;
    
    if( colorAttachment0 > 0 )
    {
        ss << " Color 0 Attachment: " << ( attachment0IsTex ? "TEXTURE" : "RENDER BUFFER" );
    }

    if( colorAttachment1 > 0 )
    {
        ss << " Color 1 Attachment: " << ( attachment1IsTex ? "TEXTURE" : "RENDER BUFFER" );
    }

    if( depthAttachment > 0 )
    {
        ss << " Depth Attachment: " << ( attachment1IsTex ? "TEXTURE" : "RENDER BUFFER" );
    }

    return ss.str();
}

// *****************************
//
std::string  FramebufferDesc::TargetStr ( GLenum target ) const
{
    return BVGLTranslator::TranslateFramebufferTarget( target );
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
    std::stringstream ss;

    ss << "    Attrib: " << std::setw( 2 ) << std::setfill( ' ' ) << index << " ";
    ss << "Buf: " << std::setw( 2 ) << std::setfill( ' ' ) << buffer << " ";
    ss << "Size: " << std::setw( 1 ) <<size << " ";
    ss << "Type: " << BVGLTranslator::TranslateAttribPointerType( type ) << " ";
    ss << "Normalized: " << normalized << " ";
    ss << "Stride: " << std::setw( 2 ) << stride;

    return ss.str();
}

// *************************************************************************************************************************************************

// *****************************
//
VertexArrayDesc::VertexArrayDesc        ()
    : ID( 0 )
{
}

// *****************************
//
void    VertexArrayDesc::SetID          ( GLuint ID )
{
    this->ID = ID;
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
    std::stringstream ss;

    ss << "Vertex Array: " << std::setw( 2 ) << std::setfill( ' ' ) << ID;

    if( attributePointers.size() > 0 )
    {
        ss << std::endl;

        for( auto it = attributePointers.begin(); it != attributePointers.end(); ++it )
        {
            ss << it->second.Summary() << std::endl;
        }
    }

    return ss.str();
}

// *****************************
//
std::string  VertexArrayDesc::TargetStr ( GLenum target ) const
{
    assert( target == 1 );

    return "DEFAULT";
}

} // bv
