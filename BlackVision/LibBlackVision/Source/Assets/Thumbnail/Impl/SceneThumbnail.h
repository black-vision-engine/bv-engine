#pragma once

#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

namespace bv
{

class SceneThumbnail;
DEFINE_CONST_PTR_TYPE( SceneThumbnail )

class SceneThumbnail : public Thumbnail
{
private:
    mutable MemoryChunkConstPtr m_data;
    mutable std::string         m_dataBase64;

    explicit        SceneThumbnail   ( const MemoryChunkConstPtr & data );
    explicit        SceneThumbnail   ( const std::string & data );
    explicit        SceneThumbnail   ( IDeserializer & deser );

public:
    virtual MemoryChunkConstPtr             Data        () const override;
    virtual const std::string &             DataBase64  () const override;

    static SceneThumbnailConstPtr           Create      ( const MemoryChunkConstPtr & data );
    static SceneThumbnailConstPtr           Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const override;

    virtual     ~SceneThumbnail  ();
};



}	// bv