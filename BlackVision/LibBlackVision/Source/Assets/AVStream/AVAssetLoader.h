#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/AVStream/AVAsset.h"
#include "Assets/AVStream/AVAssetDescriptor.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"


namespace bv {


class AVAssetLoader : public AssetLoader
{
public:

    AssetConstPtr		LoadAsset			( const AssetDescConstPtr & desc ) const override;
    AssetDescConstPtr	CreateDescriptor	( const IDeserializer& deserializeObject ) const override;

    ThumbnailConstPtr   LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;

    virtual ~AVAssetLoader(){}

private:

    ThumbnailConstPtr   LoadThumbnail       ( const AVAssetDescConstPtr & desc );

};

} // bv



