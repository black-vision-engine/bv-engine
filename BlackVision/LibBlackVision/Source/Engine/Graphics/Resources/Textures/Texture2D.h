#pragma once

#include "Engine/Graphics/Resources/Textures/Texture.h"

namespace bv
{

class Texture2D : public Texture
{
private:

    UInt32		m_width;
    UInt32      m_height;

public:

                        Texture2D       ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 levels );
    virtual             ~Texture2D      ();

    UInt32			    GetWidth        ( UInt32 level = 0 ) const;
    UInt32              GetHeight       ( UInt32 level = 0 ) const;

	MemoryChunkConstPtr	GetData			( UInt32 level = 0 ) const;

	void				SetData			( MemoryChunkConstPtr data, UInt32 level = 0 );
	void				SetData			( const std::vector< MemoryChunkConstPtr > & data, TextureFormat format, UInt32 width, UInt32 height, UInt32 levels );

    SizeType			RawFrameSize    ( UInt32 level = 0 ) const;
    static SizeType		RawFrameSize    ( TextureFormat format, UInt32 width, UInt32 height, UInt32 level = 0 );

};

DEFINE_PTR_TYPE(Texture2D)
DEFINE_CONST_PTR_TYPE(Texture2D)

} //bv
