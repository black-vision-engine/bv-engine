#include "gtest/gtest.h"

#include "Helpers/RenderLogicInitializerAccessor.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"
#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/OutputLogicDesc.h"

#include "BVConfig.h"

using namespace bv;




// ***********************
// Output descriptor creation from good config.
TEST( Engine_RenderChannels, OutputDescCreation )
{
    BVConfig config( "TestConfigs/OutputsTests/TwoRenderChannels.xml" );

    OutputLogicDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    auto outDescriptor = desc.AccessDesc( (int)CustomOutputType::COT_VIDEO );
    ASSERT_EQ( outDescriptor.GetRepresentedOutputType(), CustomOutputType::COT_VIDEO );

    auto & outProperties = outDescriptor.GetOutputProperties();
    
    ASSERT_EQ( outProperties.size(), 2 );

    // Check first VideoOutput key in config
    auto out1 = outProperties[ 0 ];
    EXPECT_EQ( out1[ "outputID" ], "0" );
    EXPECT_EQ( out1[ "width" ], "1920" );
    EXPECT_EQ( out1[ "height" ], "1080" );
    EXPECT_EQ( out1[ "renderChannelID" ], "0" );
    
    // Check second VideoOutput key in config
    auto out2 = outProperties[ 1 ];
    EXPECT_EQ( out2[ "outputID" ], "1" );
    EXPECT_EQ( out2[ "width" ], "1920" );
    EXPECT_EQ( out2[ "height" ], "1080" );
    EXPECT_EQ( out2[ "renderChannelID" ], "1" );

}


// ***********************
// RenderChannel can contain many VideoOutputs. 
TEST( Engine_RenderChannels, OutputDesc_MultiOutputsFromChannel )
{
    BVConfig config( "TestConfigs/OutputsTests/MultiOutputsFromChannel.xml" );

    OutputLogicDesc desc;
    
    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    auto outDescriptor = desc.AccessDesc( ( int )CustomOutputType::COT_VIDEO );
    ASSERT_EQ( outDescriptor.GetRepresentedOutputType(), CustomOutputType::COT_VIDEO );

    auto & outProperties = outDescriptor.GetOutputProperties();

    ASSERT_EQ( outProperties.size(), 2 );

    // Check first VideoOutput key in config
    auto out1 = outProperties[ 0 ];
    EXPECT_EQ( out1[ "outputID" ], "0" );
    EXPECT_EQ( out1[ "width" ], "1920" );
    EXPECT_EQ( out1[ "height" ], "1080" );
    EXPECT_EQ( out1[ "renderChannelID" ], "0" );

    // Check second VideoOutput key in config
    auto out2 = outProperties[ 1 ];
    EXPECT_EQ( out2[ "outputID" ], "1" );
    EXPECT_EQ( out2[ "width" ], "1920" );
    EXPECT_EQ( out2[ "height" ], "1080" );
    EXPECT_EQ( out2[ "renderChannelID" ], "0" );

}

// ***********************
// There are two RenderChannel entries with id = 0. Each one has VideoOutput inside.
// Only VideoOutput from first RenderChannel should be created.
TEST( Engine_RenderChannels, OutputDesc_OutputsInDuplicateChannels )
{
    BVConfig config( "TestConfigs/OutputsTests/DuplicateRenderChannels.xml" );

    OutputLogicDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    auto outDescriptor = desc.AccessDesc( ( int )CustomOutputType::COT_VIDEO );
    ASSERT_EQ( outDescriptor.GetRepresentedOutputType(), CustomOutputType::COT_VIDEO );

    auto & outProperties = outDescriptor.GetOutputProperties();

    // Second VideoOuptut Entry should be ingored.
    ASSERT_EQ( outProperties.size(), 1 );

    auto out1 = outProperties[ 0 ];
    EXPECT_EQ( out1[ "outputID" ], "0" );
    EXPECT_EQ( out1[ "renderChannelID" ], "0" );
}

// ***********************
// There are two RenderChannel entries with id = 0. Each one has VideoOutput inside.
// Only VideoOutput from first RenderChannel should be created. This case differs from previous.
// Both RenderChannels are enabled.
TEST( Engine_RenderChannels, OutputDesc_OutputsInDuplicateEnabledChannels )
{
    BVConfig config( "TestConfigs/OutputsTests/DuplicateEnabledChannels.xml" );

    OutputLogicDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    auto outDescriptor = desc.AccessDesc( ( int )CustomOutputType::COT_VIDEO );
    ASSERT_EQ( outDescriptor.GetRepresentedOutputType(), CustomOutputType::COT_VIDEO );

    auto & outProperties = outDescriptor.GetOutputProperties();

    // Second VideoOuptut Entry should be ingored.
    ASSERT_EQ( outProperties.size(), 1 );

    auto out1 = outProperties[ 0 ];
    EXPECT_EQ( out1[ "outputID" ], "0" );
    EXPECT_EQ( out1[ "renderChannelID" ], "0" );
}

