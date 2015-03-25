#include "Texture2DSequenceImpl.h"

#include <cassert>


namespace bv {

// *********************************
//  
Texture2DSequenceImpl::Texture2DSequenceImpl                        ( TextureFormat format, SizeType width, SizeType height )
    : Texture2D( format, width, height, DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE ) //FIXME: are there any chances that other semantics can be used for animations??
    , m_activeTexture( 0 )
{
    assert( width > 0 );
    assert( height > 0 );
}

// *********************************
//  
Texture2DSequenceImpl::~Texture2DSequenceImpl                   ()
{
}

// *********************************
//  
bool                Texture2DSequenceImpl::AddTextureSettingRawData ( MemoryChunkConstPtr data, TextureFormat format, SizeType width, SizeType height )
{
    if( format != GetFormat() || width != GetWidth() || height != GetHeight() )
    {
        return false;
    }

    m_data.push_back( data );

    SetChanged( true );

    return true;
}

// *********************************
//  
SizeType            Texture2DSequenceImpl::NumTextures             () const
{
    return m_data.size();
}

// *********************************
//  
UInt32				Texture2DSequenceImpl::GetNumLevels				() const
{
	return 0;
}

// *********************************
//  
void			Texture2DSequenceImpl::SetActiveTexture       ( SizeType txNum )
{
    assert( txNum < NumTextures() );

    auto changed = txNum != m_activeTexture;
    m_activeTexture = txNum;

    SetChanged( changed );
}

// *********************************
//  
SizeType		Texture2DSequenceImpl::GetActiveTextureNum          () const
{
    return m_activeTexture;
}

// *********************************
//  
SizeType        Texture2DSequenceImpl::GetDataSize                  ( UInt32 level ) const
{
	assert( level == 0 ); // No mipmaps for animation
    return NumTextures() * RawFrameSize();
}

// *********************************
//  
MemoryChunkConstPtr Texture2DSequenceImpl::GetData                  ( UInt32 level ) const
{
	assert( level == 0 ); // No mipmaps for animation
    assert( NumTextures() > 0 );

    return m_data[ m_activeTexture ];
}

} //bv
