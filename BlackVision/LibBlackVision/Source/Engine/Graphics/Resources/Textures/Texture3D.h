#pragma once

#include "Engine/Graphics/Resources/Texture.h"

namespace bv
{

class Texture3D : public Texture
{
private:

    UInt32		m_width;
    UInt32		m_height;
    UInt32		m_depth;

public:

                        Texture3D       ( TextureFormat format, UInt32 width, UInt32 height, UInt32 depth, DataBuffer::Semantic semantic, UInt32 levels );
    virtual             ~Texture3D      ();

    UInt32			    GetWidth        ( UInt32 level = 0 ) const;
    UInt32			    GetHeight       ( UInt32 level = 0 ) const;
    UInt32			    GetDepth        ( UInt32 level = 0 ) const;

	MemoryChunkConstPtr	GetData			( UInt32 level = 0 ) const;

	void				SetData			( MemoryChunkConstPtr data, UInt32 level = 0 );
	void				SetData			( const std::vector< MemoryChunkConstPtr > & data );

    SizeType			RawFrameSize    ( UInt32 level = 0 ) const;
    static SizeType		RawFrameSize    ( TextureFormat format, UInt32 width, UInt32 height, UInt32 depth, UInt32 level = 0 );

};

DEFINE_PTR_TYPE(Texture3D)
DEFINE_CONST_PTR_TYPE(Texture3D)

} //bv
