#include "stdafx.h"

#include "MeshAssetLoader.h"
#include "Assets/Mesh/MeshAssetDescriptor.h"
#include "Assets/Mesh/MeshUtils.h"

#include "Tools/IncludeJSON.h"




#include "Memory/MemoryLeaks.h"



namespace bv {


// ******************************
//
AssetConstPtr MeshLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
    // This lock sucks. But FBX SDK can't be used in multithreaded environment.
    std::lock_guard< std::mutex > lock( m_lock );

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
    auto typedDesc = QueryTypedDesc< MeshAssetDescConstPtr >( desc );

    assert( typedDesc );

    return MeshUtils::LoadThumbnail( typedDesc );
}


} // bv
