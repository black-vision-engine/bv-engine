#pragma once

#include "Assets/Thumbnail/Thumbnail.h"
#include "Memory/MemoryChunk.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

namespace bv
{

class PresetThumbnail;
DEFINE_CONST_PTR_TYPE( PresetThumbnail )

class PresetThumbnail : public Thumbnail
{
private:
    mutable MemoryChunkConstPtr m_data;
    mutable std::string         m_dataBase64;

    explicit        PresetThumbnail ( const MemoryChunkConstPtr & data );
    explicit        PresetThumbnail ( const std::string & data );
    explicit        PresetThumbnail ( IDeserializer & deser );

public:
    virtual MemoryChunkConstPtr             Data        () const override;
    virtual const std::string &             DataBase64  () const override;

    static PresetThumbnailConstPtr          Create      ( const MemoryChunkConstPtr & data );
    static PresetThumbnailConstPtr          Create      ( IDeserializer & deser );

    void                                    Serialize   ( ISerializer & ser ) const override;

    virtual     ~PresetThumbnail  ();
};



}	// bv