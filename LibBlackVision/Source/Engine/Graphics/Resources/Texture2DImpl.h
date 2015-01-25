#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Core/MemoryChunk.h"

namespace bv
{

class Texture2DImpl : public Texture2D
{
private:

    MemoryChunkConstPtr m_data;

    unsigned int        m_dataSize;

public:

                                Texture2DImpl   ( TextureFormat format, unsigned int width, unsigned int height, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE_STATIC );
    virtual                     ~Texture2DImpl  ();

    virtual size_t              GetDataSize     () const override;

    virtual MemoryChunkConstPtr GetData         () const override;

    bool                        SetRawData      ( MemoryChunkConstPtr data, TextureFormat format, unsigned int width, unsigned int height );

private:

    unsigned int                SizeInBytes     ( TextureFormat format, unsigned int width, unsigned int height );

};

DEFINE_PTR_TYPE(Texture2DImpl)
DEFINE_CONST_PTR_TYPE(Texture2DImpl)

} //bv
