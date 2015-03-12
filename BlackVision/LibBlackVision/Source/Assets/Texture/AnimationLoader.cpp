#include "AnimationLoader.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/TextureAsset.h"
#include "Assets/AssetManager.h"

#include <cassert>

namespace bv { namespace model {

// *******************************
//
TextureResourceConstPtr	AnimationLoader::LoadFrame( const TextureResourceDescConstPtr & frameDesc ) const
{
    auto res = ResourceManager::GetInstance().LoadResource( frameDesc );

	auto texRes = QueryTypedRes< TextureResourceConstPtr >( res );

    if ( texRes == nullptr )
    {
        return nullptr;
    }

    return texRes;
}


// ******************************
//
ResourceConstPtr AnimationLoader::LoadResource( const ResourceDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< AnimationResourceDescConstPtr >( desc );

	assert( typedDesc );

	auto frames = typedDesc->GetFrames();

	if ( frames.size() == 0 )
    {
        return nullptr;
    }

	std::vector< TextureResourceConstPtr > framesResources;
	
    printf( "Loading animation\n" );

	auto i = 0;
	for( auto f : frames )
	{
		framesResources.push_back( LoadFrame( f ) );
		printf( "\rLoaded %d out of %d total frames                ", i + 1, frames.size() );
		++i;
	}

	printf( "\n" );

	return AnimationResource::Create( framesResources );
}

} // model
} // bv
