#pragma once


#include "Assets/AssetLoader.h"


namespace bv {



// ***********************
//
class FakeAssetLoader : public AssetLoader
{
public:

    AssetConstPtr						LoadAsset			( const AssetDescConstPtr & desc ) const override;
    AssetDescConstPtr					CreateDescriptor	( const IDeserializer& deserializeObject ) const override;

    ThumbnailConstPtr                   LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;

    virtual								~FakeAssetLoader		(){}
};

DEFINE_PTR_TYPE( FakeAssetLoader )


} // bv


