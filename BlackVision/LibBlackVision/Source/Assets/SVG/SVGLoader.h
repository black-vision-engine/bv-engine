#pragma once

#include "Assets/AssetLoader.h"
#include "SVGAsset.h"

namespace bv {

class SVGLoader : public AssetLoader
{
public:

    virtual AssetConstPtr		LoadAsset       ( const AssetDescConstPtr & desc )  const;
    virtual AssetDescConstPtr	CreateDescriptor( const IDeserializer & deserializeObject ) const;

    virtual ThumbnailConstPtr   LoadThumbnail   ( const AssetDescConstPtr & desc ) const;

//            SVGLoader			();
//    virtual ~AssetLoader		();

private:
    void                        ParsePath       ( IDeserializer & deser, SVGAssetPtr mesh ) const;
    void                        ParseGroup      ( IDeserializer & deser, SVGAssetPtr mesh ) const;
};

} // bv
