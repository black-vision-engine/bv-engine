#pragma once


#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "DataTypes/Hash.h"


namespace bv
{
class VideoStreamAssetThumbnail;
DEFINE_CONST_PTR_TYPE( VideoStreamAssetThumbnail )


class VideoStreamAssetThumbnail : public Thumbnail
{
private:

    mutable MemoryChunkConstPtr m_data;
    mutable std::string         m_dataBase64;
    Hash                        m_origDataHash;


    explicit        VideoStreamAssetThumbnail   ( const MemoryChunkConstPtr & data, const Hash & h );
    explicit        VideoStreamAssetThumbnail   ( const std::string & data, const Hash & h );
    explicit        VideoStreamAssetThumbnail   ( IDeserializer & deser );

public:

    virtual MemoryChunkConstPtr             Data        () const override;
    virtual const std::string &             DataBase64  () const override;
    const Hash &                            GetHash     () const;

    static VideoStreamAssetThumbnailConstPtr    Create      ( const MemoryChunkConstPtr & data, const Hash & h );
    static VideoStreamAssetThumbnailConstPtr    Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const override;

    virtual     ~VideoStreamAssetThumbnail  ();
};


}   // bv
