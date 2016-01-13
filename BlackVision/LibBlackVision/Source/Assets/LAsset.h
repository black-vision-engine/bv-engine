#pragma once

#include "Asset.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

namespace bv {

struct LAsset // = loaded asset :)
{
    std::string                     name;

    AssetDescConstPtr               assetDesc;
    AssetConstPtr                   asset;
    model::ResourceStateModelPtr    rsm;

    LAsset( std::string name, AssetDescConstPtr assetDesc, AssetConstPtr asset, model::ResourceStateModelPtr rsm )
        : name( name ), assetDesc( assetDesc ), asset( asset ), rsm( rsm ) {}

};

DEFINE_PTR_TYPE( LAsset );
DEFINE_CONST_PTR_TYPE( LAsset );

}
