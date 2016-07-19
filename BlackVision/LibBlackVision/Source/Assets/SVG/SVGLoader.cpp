#include "stdafx.h"

#include "SVGLoader.h"

#include "SVGAssetDescriptor.h"

namespace bv {

// ***********************
//
AssetConstPtr		SVGLoader::LoadAsset       ( const AssetDescConstPtr & desc )  const
{
    return nullptr; desc;
}

// ***********************
//
AssetDescConstPtr	SVGLoader::CreateDescriptor( const IDeserializer& deser ) const
{
    if( deser.GetAttribute( "type" ) != SVGAssetDescriptor::UID() )
        return nullptr;
    return SVGAssetDescriptor::Create( deser.GetAttribute( "path" ) );
}

// ***********************
//
ThumbnailConstPtr   SVGLoader::LoadThumbnail   ( const AssetDescConstPtr & desc ) const
{
    return nullptr; desc;
}

}
