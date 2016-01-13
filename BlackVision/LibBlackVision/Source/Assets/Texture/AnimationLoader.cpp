#include "AnimationLoader.h"

#include "Assets/Assets.h"

#include "ProjectManager.h"

#include "IO/DirIO.h"

#include <cassert>

namespace bv {

// *******************************
//
TextureAssetConstPtr	AnimationLoader::LoadFrame( const TextureAssetDescConstPtr & frameDesc ) const
{
    auto asset = AssetManager::GetInstance().LoadAsset( frameDesc );

	auto texRes = QueryTypedRes< TextureAssetConstPtr >( asset );

    if ( texRes == nullptr )
    {
        return nullptr;
    }

    return texRes;
}


// ******************************
//
AssetConstPtr AnimationLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< AnimationAssetDescConstPtr >( desc );

	assert( typedDesc );

    auto p = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetPath() );
    auto files = bv::Path::List( p.Str(), false, typedDesc->GetFilter() );//Dir::ListFiles( p.Str(), typedDesc->GetFilter() );

    TextureAssetDescVec framesDesc;

    if ( files.size() > 0 )
    {
	    for( auto f : files )
	    {
            framesDesc.push_back( TextureAssetDesc::Create( "file:/" + f.Str(), true ) );
        }
    }

	if ( framesDesc.size() == 0 )
    {
        return nullptr;
    }

	std::vector< TextureAssetConstPtr > framesAssets;
	
    printf( "Loading animation\n" );

	auto i = 0;
	for( auto f : framesDesc )
	{
		framesAssets.push_back( LoadFrame( f ) );
		printf( "\rLoaded %d out of %d total frames                ", i + 1, framesDesc.size() );
		++i;
	}

	printf( "\n" );

	return AnimationAsset::Create( framesAssets );
}

// ******************************
//
AssetDescConstPtr	AnimationLoader::CreateDescriptor	( const IDeserializer& deserializeObject ) const
{
	return std::static_pointer_cast<const AssetDesc>( AnimationAssetDesc::Create( deserializeObject ) );
}

} // bv
