#pragma once

#include "Assets/AssetLoader.h"

namespace bv {

class SVGLoader : public AssetLoader
{
public:

    virtual AssetConstPtr		LoadAsset       ( const AssetDescConstPtr & desc )  const;
    virtual AssetDescConstPtr	CreateDescriptor( const IDeserializer & deserializeObject ) const;

    virtual ThumbnailConstPtr   LoadThumbnail   ( const AssetDescConstPtr & desc ) const;

//            SVGLoader			();
//    virtual ~AssetLoader		();

};

} // bv
