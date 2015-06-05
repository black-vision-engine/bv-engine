#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Memory/MemoryChunk.h"

namespace bv
{

class Texture2DImpl : public Texture2D
{
private:

    std::vector< MemoryChunkConstPtr > m_data;

public:

                                Texture2DImpl   ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE_STATIC );
    virtual                     ~Texture2DImpl  ();

	virtual UInt32			    GetNumLevels    () const override;

    virtual SizeType            GetDataSize     ( UInt32 level = 0 ) const override;

    virtual MemoryChunkConstPtr GetData         ( UInt32 level = 0 ) const override;

    bool                        SetRawData      ( const std::vector< MemoryChunkConstPtr > & data, TextureFormat format, UInt32 width, UInt32 height );

private:

    SizeType					SizeInBytes     ( TextureFormat format, UInt32 width, UInt32 height );

};

DEFINE_PTR_TYPE(Texture2DImpl)
DEFINE_CONST_PTR_TYPE(Texture2DImpl)

} //bv
