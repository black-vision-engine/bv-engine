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
    return MeshUtils::LoadMesh( meshDesc->GetKey() );
}

// ******************************
//
AssetDescConstPtr MeshLoader::CreateDescriptor( const IDeserializer & deser ) const
{
    { deser; }
	return nullptr;
}

// ******************************
//
ThumbnailConstPtr MeshLoader::LoadThumbnail   ( const AssetDescConstPtr & desc ) const
{
    { desc; }
	return nullptr;
}


} // bv
