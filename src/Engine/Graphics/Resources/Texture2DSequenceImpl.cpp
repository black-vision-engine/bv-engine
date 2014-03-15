#include "Texture2DSequenceImpl.h"

#include <cassert>


namespace bv {

// *********************************
//  
Texture2DSequenceImpl::Texture2DSequenceImpl                    ( TextureFormat format, int width, int height )
    : Texture2D( format, width, height, DataBuffer::Semantic::S_TEXTURE ) //FIXME: are there any chances that other semantics can be used for animations??
    , m_activeTexture( 0 )
{
    assert( width > 0 );
    assert( height > 0 );
}

// *********************************
//  
Texture2DSequenceImpl::~Texture2DSequenceImpl                   ()
{
    for( auto rawMem : m_data )
    {
        delete[] rawMem;
    }
}

// *********************************
//  
bool                Texture2DSequenceImpl::AddTextureWritingBits( const char * data, TextureFormat format, int width, int height )
{
    if( format != GetFormat() || width != GetWidth() || height != GetHeight() )
    {
        return false;
    }

    auto newSize = width * height * GetPixelSize();
    auto dstData = new char[ width * height * GetPixelSize() ];

    memcpy( dstData, data, newSize );
    m_data.push_back( dstData );

    SetChanged( true );

    return true;
}

// *********************************
//  
unsigned int         Texture2DSequenceImpl::NumTextures         () const
{
    return m_data.size();
}

// *********************************
//  
void                  Texture2DSequenceImpl::SetActiveTexture   ( unsigned int txNum )
{
    assert( txNum < NumTextures() );

    auto changed = txNum != m_activeTexture;
    m_activeTexture = txNum;

    SetChanged( changed );
}

// *********************************
//  
unsigned int    Texture2DSequenceImpl::GetActiveTextureNum     () const
{
    return m_activeTexture;
}

// *********************************
//  
size_t          Texture2DSequenceImpl::GetDataSize             () const
{
    return NumTextures() * GetPixelSize() * GetWidth() * GetHeight();
}

// *********************************
//  
char *          Texture2DSequenceImpl::GetData                 ()
{
    assert( NumTextures() > 0 );

    return m_data[ m_activeTexture ];
}

// *********************************
//  
const char *    Texture2DSequenceImpl::GetData                 () const
{
    assert( NumTextures() > 0 );

    return m_data[ m_activeTexture ];
}

} //bv
