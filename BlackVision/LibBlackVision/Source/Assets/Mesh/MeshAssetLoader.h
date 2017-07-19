#pragma once

#include "Assets/AssetLoader.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Engine/Types/Enums.h"

#include <mutex>


namespace bv {

class MeshLoader : public AssetLoader
{
private:

    mutable std::mutex      m_lock;

public:

    AssetConstPtr						LoadAsset			( const AssetDescConstPtr & desc ) const override;
    AssetDescConstPtr					CreateDescriptor	( const IDeserializer & deser ) const override;

    ThumbnailConstPtr                   LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;

    virtual								~MeshLoader		    (){}
};

} // bv
