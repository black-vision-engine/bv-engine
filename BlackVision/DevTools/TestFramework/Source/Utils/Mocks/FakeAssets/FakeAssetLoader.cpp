#include "FakeAssetLoader.h"

#include "FakeDescriptor.h"
#include "FakeAsset.h"


#include <thread>


namespace bv
{

// ***********************
//
AssetConstPtr               FakeAssetLoader::LoadAsset          ( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< FakeDescriptorConstPtr >( desc );
    assert( typedDesc );

    std::this_thread::sleep_for( std::chrono::milliseconds( typedDesc->GetSleepTime() ) );

    return std::make_shared< FakeAsset >( typedDesc->GetPath() );
}

// ***********************
//
AssetDescConstPtr           FakeAssetLoader::CreateDescriptor   ( const IDeserializer & deserializeObject ) const
{
    assert( !"Implement when necessary" );
    return AssetDescConstPtr();
}

// ***********************
//
ThumbnailConstPtr           FakeAssetLoader::LoadThumbnail      ( const AssetDescConstPtr & desc ) const
{
    assert( !"Implement when necessary" );
    return ThumbnailConstPtr();
}


}	// bv