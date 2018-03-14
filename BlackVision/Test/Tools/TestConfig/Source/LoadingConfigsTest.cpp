#include "gtest/gtest.h"

#include "BVConfig.h"

using namespace bv;


// ***********************
// Test loading config with all values set to not-defaults.
TEST( Tools_Config, Loading_NotDefaultValues )
{
    BVConfig config( "TestConfigs/ConfigsTest/NotDefaultConfig.xml" );

    EXPECT_EQ( config.PMFolder(), "media/media/media" );
    EXPECT_EQ( config.FullScreenMode(), false );
    EXPECT_EQ( config.ReadbackFlag(), true );

    EXPECT_EQ( config.LoadSceneFromEnv(), true );
    EXPECT_EQ( config.LoadSceneFromProjectManager(), "PM-scene.scn" );
    EXPECT_EQ( config.SceneFromEnvName(), "TWO_TEXTURED_RECTANGLES" );

    EXPECT_EQ( config.UseDebugLayer(), true );
    EXPECT_EQ( config.DebugFilePath(), "../../../../Logs/" );

    EXPECT_EQ( config.OnFailedTexLoadBehavior(), "LoadTransparent" );

    EXPECT_EQ( config.GlobalGain(), 0.5 );

    EXPECT_EQ( config.FPS(), 5000 );
    EXPECT_EQ( config.TimerFPS(), 50 );
    EXPECT_EQ( config.ClearColor(), glm::vec4( 0.5, 0.5, 0.5, 0.5 ) );
}
