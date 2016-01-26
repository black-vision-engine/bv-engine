#pragma once

#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "DataTypes/Hash.h"

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
    Hash                m_origDataHash;

    explicit        TextureAssetThumbnail   ( const MemoryChunkConstPtr & data, SizeType width, SizeType height, UInt32 bpp, const Hash & h );
    explicit        TextureAssetThumbnail   ( IDeserializer & deser );

public:
    virtual const char *                    Data        () const override;
    virtual const char *                    DataBase64  () const override;
    const Hash &                            GetHash     () const;

    static TextureAssetThumbnailConstPtr    Create      ( const MemoryChunkConstPtr & data, SizeType width, SizeType height, UInt32 bpp, const Hash & h );
    static TextureAssetThumbnailConstPtr    Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const override;

    virtual     ~TextureAssetThumbnail  ();
};

} // bv