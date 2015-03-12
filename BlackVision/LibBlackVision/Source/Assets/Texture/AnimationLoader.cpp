#include "AnimationLoader.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/TextureAsset.h"
#include "Assets/AssetManager.h"

#include <cassert>

namespace bv { namespace model {

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

	auto frames = typedDesc->GetFrames();

	if ( frames.size() == 0 )
    {
        return nullptr;
    }

	std::vector< TextureAssetConstPtr > framesAssets;
	
    printf( "Loading animation\n" );

	auto i = 0;
	for( auto f : frames )
	{
		framesAssets.push_back( LoadFrame( f ) );
		printf( "\rLoaded %d out of %d total frames                ", i + 1, frames.size() );
		++i;
	}

	printf( "\n" );

	return AnimationAsset::Create( framesAssets );
}

} // model
} // bv
