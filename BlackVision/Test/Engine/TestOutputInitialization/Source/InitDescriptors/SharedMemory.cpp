#include "gtest/gtest.h"

#include "Helpers/RenderLogicInitializerAccessor.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"
#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/OutputLogicDesc.h"

#include "BVConfig.h"

using namespace bv;



// ***********************
// Test initialization from config.
TEST( Engine_SharedMemory, OutputDescCreation )
{
    BVConfig config( "TestConfigs/OutputsTests/SharedMemory.xml" );

    OutputLogicDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    auto outDescriptor = desc.AccessDesc( ( int )CustomOutputType::COT_STREAM );
    ASSERT_EQ( outDescriptor.GetRepresentedOutputType(), CustomOutputType::COT_STREAM );

    auto & outProperties = outDescriptor.GetOutputProperties();

    ASSERT_EQ( outProperties.size(), 1 );

    // Check properties
    auto out1 = outProperties[ 0 ];
    EXPECT_EQ( out1[ "Name" ], "BV_SHM" );

    EXPECT_EQ( outDescriptor.GetHeight(), 300 );
    EXPECT_EQ( outDescriptor.GetWidth(), 350 );
}

// ***********************
// No shared memory entries in config.
TEST( Engine_SharedMemory, OutputDescCreation_NoEntries )
{
    BVConfig config( "TestConfigs/OutputsTests/SharedMemory_NoEntries.xml" );

    OutputLogicDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    auto outDescriptor = desc.AccessDesc( ( int )CustomOutputType::COT_STREAM );
    ASSERT_EQ( outDescriptor.GetRepresentedOutputType(), CustomOutputType::COT_STREAM );

    auto & outProperties = outDescriptor.GetOutputProperties();

    ASSERT_EQ( outProperties.size(), 1 );

    // Check properties
    auto out1 = outProperties[ 0 ];
    EXPECT_EQ( out1[ "Name" ], "BV" );

    EXPECT_EQ( outDescriptor.GetHeight(), 1080 );
    EXPECT_EQ( outDescriptor.GetWidth(), 1920 );
    EXPECT_FALSE( outDescriptor.IsEnabled() );
}
