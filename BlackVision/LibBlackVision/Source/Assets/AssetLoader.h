#pragma once

#include "AssetDescriptor.h"
#include "Asset.h"
#include "Serialization/ISerializer.h"

namespace bv { 

class AssetLoader
{
public:

	virtual AssetConstPtr		LoadAsset        ( const AssetDescConstPtr & desc )  const = 0;
    virtual AssetDescConstPtr	CreateDescriptor ( ISerializer& deserializeObject ) const = 0;


            AssetLoader			();
    virtual ~AssetLoader		();

};

DEFINE_CONST_PTR_TYPE( AssetLoader )




} // bv
