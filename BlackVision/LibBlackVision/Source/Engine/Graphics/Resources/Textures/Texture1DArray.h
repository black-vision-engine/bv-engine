#pragma once

#include "Engine/Graphics/Resources/Texture.h"
#include "Engine/Graphics/Resources/Textures/Texture1D.h"

namespace bv
{

class Texture1DArray : public Texture
{
private:
    
	UInt32				m_width;
	
	UInt32				m_layers;

public:

                        Texture1DArray  ( TextureFormat format, UInt32 width, DataBuffer::Semantic semantic, UInt32 layers, UInt32 levels );
    virtual             ~Texture1DArray ();

    UInt32			    GetWidth        ( UInt32 level = 0 ) const;

	UInt32				GetNumLayers    () const;

	MemoryChunkConstPtr	GetData			( UInt32 layer, UInt32 level = 0 ) const;

	void				SetData			( MemoryChunkConstPtr data, UInt32 layer, UInt32 level = 0 );
	void				SetData			( const std::vector< MemoryChunkConstPtr > & data );

    SizeType			RawFrameSize    ( UInt32 level = 0 ) const;
    static SizeType		RawFrameSize    ( TextureFormat format, UInt32 width, UInt32 level = 0 );
	
private:

	UInt32				GetIndex		( UInt32 layer, UInt32 level ) const;
};

DEFINE_PTR_TYPE(Texture1DArray)
DEFINE_CONST_PTR_TYPE(Texture1DArray)

} //bv
