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
    mutable MemoryChunkConstPtr m_data;
    mutable std::string         m_dataBase64;
    Hash                        m_origDataHash;

    explicit        TextureAssetThumbnail   ( const MemoryChunkConstPtr & data, const Hash & h );
    explicit        TextureAssetThumbnail   ( const std::string & data, const Hash & h );
    explicit        TextureAssetThumbnail   ( IDeserializer & deser );

public:
    virtual MemoryChunkConstPtr             Data        () const override;
    virtual const std::string &             DataBase64  () const override;
    const Hash &                            GetHash     () const;

    static TextureAssetThumbnailConstPtr    Create      ( const MemoryChunkConstPtr & data, const Hash & h );
    static TextureAssetThumbnailConstPtr    Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const override;

    virtual     ~TextureAssetThumbnail  ();
};

} // bv