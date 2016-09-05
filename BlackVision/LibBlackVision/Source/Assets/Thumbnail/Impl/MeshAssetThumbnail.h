#pragma once

#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "DataTypes/Hash.h"


namespace bv
{


class MeshAssetThumbnail;
DEFINE_CONST_PTR_TYPE( MeshAssetThumbnail )

class MeshAssetThumbnail : public Thumbnail
{
private:

    static const std::string    m_type;

private:

    mutable MemoryChunkConstPtr m_data;
    mutable std::string         m_dataBase64;

    explicit                    MeshAssetThumbnail ( const MemoryChunkConstPtr & data );
    explicit                    MeshAssetThumbnail ( const std::string & data );
    explicit                    MeshAssetThumbnail ( IDeserializer & deser );

public:

    virtual MemoryChunkConstPtr         Data        () const override;
    virtual const std::string &         DataBase64  () const override;

    static MeshAssetThumbnailConstPtr   Create      ( const MemoryChunkConstPtr & data );
    static MeshAssetThumbnailConstPtr   Create      ( IDeserializer & deser );

    void                                Serialize   ( ISerializer & ser ) const override;

    virtual     ~MeshAssetThumbnail  ();

};



}	// bv