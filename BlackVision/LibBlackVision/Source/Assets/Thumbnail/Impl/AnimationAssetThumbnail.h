#pragma once

#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "DataTypes/Hash.h"

namespace bv
{

class AnimationAssetThumbnail;
DEFINE_CONST_PTR_TYPE( AnimationAssetThumbnail )

class AnimationAssetThumbnail : public Thumbnail
{
public:
    MemoryChunkVector   m_data;
    Hash                m_origDataHash;

    explicit        AnimationAssetThumbnail   ( const MemoryChunkVector & data, const Hash & h );
    explicit        AnimationAssetThumbnail   ( IDeserializer & deser );

public:
    virtual const char *                    Data        () const override;
    virtual const char *                    DataBase64  () const override;
    const Hash &                            GetHash     () const;

    static AnimationAssetThumbnailConstPtr  Create      ( const MemoryChunkVector & data, const Hash & h );
    static AnimationAssetThumbnailConstPtr  Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const override;

    virtual     ~AnimationAssetThumbnail  ();
};

} // bv