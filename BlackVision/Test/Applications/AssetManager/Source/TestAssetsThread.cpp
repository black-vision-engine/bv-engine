#include "gtest/gtest.h"

#include "Engine/Editors/AssetsLoading/Async/AssetsThread.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Editors/AssetsLoading/Async/AssetAsyncLoadFinishedEvent.h"

#include "Utils/Mocks/FakeAssets/FakeAsset.h"
#include "Utils/Mocks/FakeAssets/FakeDescriptor.h"
#include "Utils/Mocks/FakeAssets/FakeAssetLoader.h"

#include <thread>



using namespace bv;





// ========================================================================= //
// Helpers
// ========================================================================= //


AssetConstPtr           gLoadedAsset;
LoadAssetRequest        gLoadRequest;

// ***********************
//
void        AssetLoaded     ( IEventPtr evt )
{
    if( evt->GetEventType() == AssetAsyncLoadFinishedEvent::Type() )
    {
        AssetAsyncLoadFinishedEventPtr loadedEvent = std::static_pointer_cast< AssetAsyncLoadFinishedEvent >( evt );

        gLoadedAsset = loadedEvent->Asset;
        gLoadRequest = std::move( loadedEvent->Request );
    }
}




// ***********************
//
TEST( Assets_AssetsThread, AssetLoading )
{
    GetDefaultEventManager().AddListener( &AssetLoaded, AssetAsyncLoadFinishedEvent::Type() );
    bv::AssetManager::GetInstance().RegisterLoader( FakeDescriptor::UID(), std::make_shared< FakeAssetLoader >() );

    bv::AssetsThread assetsThread;

    auto desc = std::make_shared< FakeDescriptor >( "path", 20 );

    LoadAssetRequest request;
    request.Descriptor = desc;
    request.Node = nullptr;
    request.Plugin = nullptr;
    request.Scene = nullptr;
    request.RequestID = 3;

    assetsThread.QueueRequest( std::move( request ) );
    
    // Wait 40s for asset thread to load asset.
    std::this_thread::sleep_for( std::chrono::milliseconds( 40 ) );

    // Force EventManager to call AssetLoaded function.
    GetDefaultEventManager().Update( 100 );

    // Check global variables state
    ASSERT_TRUE( gLoadedAsset != nullptr );
    EXPECT_TRUE( std::static_pointer_cast< const FakeAsset >( gLoadedAsset )->GetPath() == "path" );

    EXPECT_TRUE( gLoadRequest.Descriptor == desc );
    EXPECT_TRUE( gLoadRequest.Node == nullptr );
    EXPECT_TRUE( gLoadRequest.Plugin == nullptr );
    EXPECT_TRUE( gLoadRequest.Scene == nullptr );
    EXPECT_TRUE( gLoadRequest.RequestID == 3 );


// ***********************
// Clean global state

    gLoadedAsset = nullptr;
    gLoadRequest = LoadAssetRequest();
    GetDefaultEventManager().RemoveListener( &AssetLoaded, AssetAsyncLoadFinishedEvent::Type() );
    bv::AssetManager::GetInstance().UnregisterLoader( FakeDescriptor::UID() );
}

