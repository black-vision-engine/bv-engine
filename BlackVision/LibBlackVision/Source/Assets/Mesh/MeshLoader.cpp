#include "stdafx.h"

#include "MeshLoader.h"
#include "Assets/Mesh/MeshAssetDescriptor.h"
#include "Assets/Mesh/MeshUtils.h"

#include "Tools/IncludeJSON.h"


namespace bv {


// ******************************
//
AssetConstPtr MeshLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
    auto meshDesc = std::static_pointer_cast< const MeshAssetDesc >( desc );
    return MeshUtils::LoadMesh( meshDesc );
}

// ******************************
//
AssetDescConstPtr MeshLoader::CreateDescriptor( const IDeserializer & deser ) const
{
    return std::static_pointer_cast< const AssetDesc >( MeshAssetDesc::Create( deser ) );
}

// ******************************
//
ThumbnailConstPtr MeshLoader::LoadThumbnail   ( const AssetDescConstPtr & desc ) const
{
    //FIXME
    { desc; }
	return nullptr;
}


} // bv
