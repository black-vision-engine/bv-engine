#pragma once

#include "AssetDescriptor.h"
#include "Asset.h"

namespace bv { 

class AssetLoader
{
public:

	virtual AssetConstPtr		LoadAsset        ( const AssetDescConstPtr & desc )  const = 0;
	virtual AssetDescConstPtr	CreateDescriptor ( const std::string& jsonString )  const { jsonString; return nullptr; }

            AssetLoader			();
    virtual ~AssetLoader		();

};

DEFINE_CONST_PTR_TYPE( AssetLoader )

} // bv
