#include "gtest/gtest.h"


#include "DataTypes/Reusable.h"
#include "Memory/MemoryChunk.h"


using namespace bv;


// ***********************
// Reusable should return provided chunks in circular manner.
TEST( LibCore_DataTypes, Reusable_GetNext )
{
    std::vector< MemoryChunkPtr > chunks;

    chunks.push_back( MemoryChunk::Create( 1024 ) );
    chunks.push_back( MemoryChunk::Create( 1024 ) );
    chunks.push_back( MemoryChunk::Create( 1024 ) );
    chunks.push_back( MemoryChunk::Create( 1024 ) );

    Reusable< MemoryChunkPtr > reusables( chunks );

    EXPECT_EQ( reusables.GetNext(), chunks[ 0 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 1 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 2 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 3 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 0 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 1 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 2 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 3 ] );
    EXPECT_EQ( reusables.GetNext(), chunks[ 0 ] );
}

