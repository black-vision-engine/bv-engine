#pragma once

#include "Engine/Graphics/Resources/Texture.h"

namespace bv
{

class Texture1D : public Texture
{
private:

    UInt32				m_width;

public:

                        Texture1D       ( TextureFormat format, UInt32 width, DataBuffer::Semantic semantic, UInt32 levels );
    virtual             ~Texture1D      ();

    UInt32			    GetWidth        ( UInt32 level = 0 ) const;

	MemoryChunkConstPtr	GetData			( UInt32 level = 0 ) const;

	void				SetData			( MemoryChunkConstPtr data, UInt32 level = 0 );
	void				SetData			( const std::vector< MemoryChunkConstPtr > & data );

    SizeType			RawFrameSize    ( UInt32 level = 0 ) const;
    static SizeType		RawFrameSize    ( TextureFormat format, UInt32 width, UInt32 level = 0 );

};

DEFINE_PTR_TYPE(Texture1D)
DEFINE_CONST_PTR_TYPE(Texture1D)

} //bv
