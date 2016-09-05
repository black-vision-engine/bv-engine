#pragma once

#include "Assets/AssetLoader.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Engine/Types/Enums.h"


namespace bv
{


class DataArrayLoader : public AssetLoader
{
public:

	AssetConstPtr						LoadAsset			( const AssetDescConstPtr & desc ) const override;
	AssetDescConstPtr					CreateDescriptor	( const IDeserializer& deserializeObject ) const override;

    ThumbnailConstPtr                   LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;

    virtual								~DataArrayLoader    (){}
};



}	// bv