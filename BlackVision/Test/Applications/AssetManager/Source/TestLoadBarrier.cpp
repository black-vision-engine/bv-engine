// ***********************
// Test based on https://github.com/nieznanysprawiciel/swGraphicAPI/blob/extResourceManager/Tests/TestResourceManager/LoadBarrierTest.cpp


#include "gtest/gtest.h"


#include "Threading/ThreadsBarrier.h"
#include "Assets/AsyncLoading/LoadBarrier.h"

#include "FakeAssets/FakeDescriptor.h"
#include "FakeAssets/FakeAsset.h"



// ========================================================================= //
// Load Barrier accessor class
// ========================================================================= //


namespace bv
{


class LoadBarrier_Tester
{
private:
public:

    static std::vector< WaitingAsset* > &       GetWaitingAssets    ( LoadBarrier & barrier )   { return barrier.m_waitingAssets;  }

};

}	// bv

// ========================================================================= //
// Test helper functions
// ========================================================================= //

const int numThreads = 4;

bv::ThreadsBarrier          gPreAccessBarrier( numThreads );
bv::ThreadsBarrier          gPreWaitBarrier( numThreads - 1 );
bv::LoadBarrier             gLoadBarrier;
std::vector< int >          gOrderChecker;
std::mutex                  gCheckerLock;

bv::FakeDescriptorConstPtr  assetFile = std::make_shared< bv::FakeDescriptor >( "assets/wombat.jpg" );
bv::FakeDescriptorConstPtr  independentAsset = std::make_shared< bv::FakeDescriptor >( "assets/texture.jpg" );

bool threadsInternalResult[ numThreads ];
bool notNullWaitingAsset[ numThreads ];



// ================================ //
//
void        PushThreadNum        ( int threadNum )
{
    gCheckerLock.lock();
    gOrderChecker.push_back( threadNum );
    gCheckerLock.unlock();
}


// ================================ //
//
void        FakeLoadAsset        ( int threadNum )
{
    PushThreadNum( threadNum );

    gPreWaitBarrier.ArriveAndWait();

    std::this_thread::sleep_for( std::chrono::milliseconds( 13 ) );

    auto asset = bv::FakeAsset::Create( assetFile->GetKey() );

    PushThreadNum( threadNum );

    gLoadBarrier.LoadingCompleted( assetFile, asset );
}

// ================================ //
//
void        LoadingThread        ( int threadNum )
{
    auto result = gLoadBarrier.RequestAsset( assetFile );

    // Allow other threads to call RequestAsset.
    gPreAccessBarrier.ArriveAndWait();

    threadsInternalResult[ threadNum ] = result.second;
    notNullWaitingAsset[ threadNum ] = result.first != nullptr;

    FakeLoadAsset( threadNum );
}

// ================================ //
//
void        RequestingThread    ( int threadNum )
{
    // LoadingThread must RequestAsset first. Wait for it.
    gPreAccessBarrier.ArriveAndWait();

    auto result = gLoadBarrier.RequestAsset( assetFile );

    threadsInternalResult[ threadNum ] = result.second;
    notNullWaitingAsset[ threadNum ] = result.first != nullptr;

    gPreWaitBarrier.ArriveAndWait();
    gLoadBarrier.WaitUntilLoaded( result.first );

    PushThreadNum( threadNum );
}

// ================================ //
//
void        IndependentAssetThread    ( int threadNum )
{
    // LoadingThread must RequestAsset first. Wait for it.
    gPreAccessBarrier.ArriveAndWait();

    auto result = gLoadBarrier.RequestAsset( independentAsset );

    // Independent thread should pass without blocking. result.second should be false.
    threadsInternalResult[ threadNum ] = result.second;
    notNullWaitingAsset[ threadNum ] = result.first != nullptr;

    gLoadBarrier.LoadingCompleted( independentAsset, nullptr );
}


// ================================ //
// Only one thread should enter loading at the same time. Rest should wait.
TEST( LoadBarrierTest, AssetsLoading )
{
    std::thread threads[ numThreads ];

    threads[ 0 ] = std::thread( &LoadingThread, 0 );
    for( int i = 1; i < numThreads - 1; ++i )
        threads[ i ] = std::thread( &RequestingThread, i );

    // Independent thread
    threads[ numThreads - 1 ] = std::thread( &IndependentAssetThread, numThreads - 1 );

    for( auto& thread : threads )
        thread.join();


    // Waiting asset should never be nullptr.
    for( auto notNull : notNullWaitingAsset )
        EXPECT_TRUE( notNull == true );

    EXPECT_TRUE( threadsInternalResult[ 0 ] == false );
    EXPECT_TRUE( threadsInternalResult[ 1 ] == true );
    EXPECT_TRUE( threadsInternalResult[ 2 ] == true );
    EXPECT_TRUE( threadsInternalResult[ 3 ] == false );

    ASSERT_TRUE( gOrderChecker.size() == 4 );

    EXPECT_TRUE( gOrderChecker[ 0 ] == 0 );
    EXPECT_TRUE( gOrderChecker[ 1 ] == 0 );
    EXPECT_TRUE( ( gOrderChecker[ 2 ] == 1 || gOrderChecker[ 2 ] == 2 ) );
    EXPECT_TRUE( ( gOrderChecker[ 3 ] == 1 || gOrderChecker[ 3 ] == 2 ) );


    // Waiting assets list should be cleaned.
    EXPECT_EQ( bv::LoadBarrier_Tester::GetWaitingAssets( gLoadBarrier ).size(), 0 );
}



bv::LoadBarrier     gBarrier;


// ================================ //
// Load asset from single thread. Barrier should end in clean state without waiting assets on list.
// Note that this checks state cleaning in situation, when there're no other threads what differs
// from previous test case.
TEST( LoadBarrierTest, SingleAssetLoading )
{
    auto result = gBarrier.RequestAsset( assetFile );

    // New Waiting asset should be created in this case.
    ASSERT_EQ( result.second, false );
    EXPECT_TRUE( result.first != nullptr );

    // Here in real application should be loading code.

    gBarrier.LoadingCompleted( assetFile, nullptr );

    // Waiting assets list should be cleaned.
    EXPECT_EQ( bv::LoadBarrier_Tester::GetWaitingAssets( gBarrier ).size(), 0 );
}

