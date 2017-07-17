#include "gtest/gtest.h"

#include "Assets/Cache/HardDriveRawDataCache.h"

#include "System/Path.h"


using namespace bv;


// This constant was copied from HardDriveRawDataCache.cpp file.
static const std::string RAW_DATA_CACHE_DIR = "cache/raw_data_cache/";


TEST( TestCache, HardDriveRawDataCacheTest )
{
    auto chunk = MemoryChunk::Create( 512 );
    for( int i = 0; i < chunk->Size(); ++i )
        ( chunk->GetWritable() )[ i ] = (char)i;

    std::string chunkStr = std::string( chunk->Get(), chunk->Size() );
    Hash chunkHash = Hash::FromString( chunkStr );

    auto fileName = RAW_DATA_CACHE_DIR + chunkHash.Get();

    // Remove file from previous sessions.
    if( Path::Exists( fileName ) )
        Path::Remove( fileName );


    // Add chunk to cache.
    ASSERT_TRUE( HardDriveRawDataCache::GetInstance().Add( chunkHash, chunk, true ) );

    // Check if file was written to disk.
    EXPECT_TRUE( Path::Exists( fileName ) );

    // Try to get this data from cache.
    auto returnChunk = HardDriveRawDataCache::GetInstance().Get( chunkHash );
    ASSERT_TRUE( returnChunk.get() != nullptr && returnChunk->Get() != nullptr );

    // Compare chunk content loaded from disk.
    ASSERT_TRUE( returnChunk->Size() == chunk->Size() );
    EXPECT_TRUE( memcmp( returnChunk->Get(), chunk->Get(), chunk->Size() ) == 0 );


}





