#pragma once

#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "DataTypes/Hash.h"

namespace bv
{

class FontAssetThumbnail;
DEFINE_CONST_PTR_TYPE( FontAssetThumbnail )

class FontAssetThumbnail : public Thumbnail
{
public:
    mutable MemoryChunkConstPtr m_data;
    mutable std::string         m_dataBase64;
    Hash                        m_origDataHash;

    explicit        FontAssetThumbnail   ( const MemoryChunkConstPtr & data, const Hash & h );
    explicit        FontAssetThumbnail   ( const std::string & data, const Hash & h );
    explicit        FontAssetThumbnail   ( IDeserializer & deser );

public:
    virtual MemoryChunkConstPtr             Data        () const override;
    virtual const std::string &             DataBase64  () const override;
    const Hash &                            GetHash     () const;

    static FontAssetThumbnailConstPtr  Create      ( const MemoryChunkConstPtr & data, const Hash & h );
    static FontAssetThumbnailConstPtr  Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const override;

    virtual     ~FontAssetThumbnail  ();
};

} // bv