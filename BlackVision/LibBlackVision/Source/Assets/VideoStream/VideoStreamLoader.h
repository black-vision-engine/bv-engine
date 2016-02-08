#pragma once


#include "Assets/AssetLoader.h"
#include "Assets/VideoStream/VideoStreamAsset.h"
#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

namespace bv {


class VideoStreamLoader : public AssetLoader
{
public:

	AssetConstPtr		LoadAsset			( const AssetDescConstPtr & desc ) const override;
    AssetDescConstPtr	CreateDescriptor	( const IDeserializer& deserializeObject ) const override;

    ThumbnailConstPtr   LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;

    virtual ~VideoStreamLoader(){}
};


} // bv



