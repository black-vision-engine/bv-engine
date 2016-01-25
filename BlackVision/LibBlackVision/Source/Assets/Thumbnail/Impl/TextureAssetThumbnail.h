#pragma once

#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

namespace bv
{

class TextureAssetThumbnail;
DEFINE_CONST_PTR_TYPE( TextureAssetThumbnail )

class TextureAssetThumbnail : public Thumbnail
{
private:
    MemoryChunkConstPtr m_data;
    SizeType            m_width;
    SizeType            m_height;
    UInt32              m_bpp;

    explicit        TextureAssetThumbnail   ( const MemoryChunkConstPtr & data, SizeType width, SizeType height, UInt32 bpp );
    explicit        TextureAssetThumbnail   ( IDeserializer & deser );

public:
    virtual const char *                    Data        () const override;
    virtual const char *                    DataBase64  () const override;

    static TextureAssetThumbnailConstPtr    Create      ( const MemoryChunkConstPtr & data, SizeType width, SizeType height, UInt32 bpp );
    static TextureAssetThumbnailConstPtr    Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const;

    virtual     ~TextureAssetThumbnail  ();
};

} // bv