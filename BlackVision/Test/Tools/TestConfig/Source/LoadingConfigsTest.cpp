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

    EXPECT_EQ( config.RenderToSharedMemory(), true );

    EXPECT_EQ( config.GetRendererInput().m_DisableVerticalSync, true );
    EXPECT_EQ( config.GetRendererInput().m_EnableGLFinish, false );
    EXPECT_EQ( config.GetRendererInput().m_EnableGLFlush, false );
    EXPECT_EQ( config.GetRendererInput().m_VerticalBufferFrameCount, 0 );

    EXPECT_EQ( config.GetWindowMode(), WindowMode::MULTIPLE_SCREENS );
    EXPECT_EQ( config.DefaultWidth(), 500 );
    EXPECT_EQ( config.DefaultHeight(), 600 );
    EXPECT_EQ( config.DefaultWindowWidth(), 300 );
    EXPECT_EQ( config.DefaultWindowHeight(), 400 );

    EXPECT_EQ( config.EventLoopUpdateMillis(), 40 );
    EXPECT_EQ( config.EnableLockingQueue(), true );

    EXPECT_EQ( config.SockerServerPort(), 11011 );

    EXPECT_EQ( config.CameraPosition(), glm::vec3( 1, 1, 4 ) );
    EXPECT_EQ( config.CameraDirection(), glm::vec3( 0.5, 0.5, -1.0 ) );
    EXPECT_EQ( config.FOV(), 60 );
}


// ***********************
// Config should load default values if there are empty entries.
TEST( Tools_Config, Loading_EmptyEntries )
{
    BVConfig config( "TestConfigs/ConfigsTest/EmptyConfig.xml" );


    EXPECT_EQ( config.PMFolder(), "bv_media" );
    EXPECT_EQ( config.FullScreenMode(), false );
    EXPECT_EQ( config.ReadbackFlag(), false );

    EXPECT_EQ( config.LoadSceneFromEnv(), false );
    EXPECT_EQ( config.LoadSceneFromProjectManager(), "" );
    EXPECT_EQ( config.SceneFromEnvName(), "" );

    EXPECT_EQ( config.UseDebugLayer(), false );
    EXPECT_EQ( config.DebugFilePath(), "" );

    EXPECT_EQ( config.OnFailedTexLoadBehavior(), "LoadChecker" );

    EXPECT_EQ( config.GlobalGain(), 1.0 );

    EXPECT_EQ( config.FPS(), 60000 );
    EXPECT_EQ( config.TimerFPS(), 60 );
    EXPECT_EQ( config.ClearColor(), glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );

    EXPECT_EQ( config.RenderToSharedMemory(), false );

    EXPECT_EQ( config.GetRendererInput().m_DisableVerticalSync, false );
    EXPECT_EQ( config.GetRendererInput().m_EnableGLFinish, true );
    EXPECT_EQ( config.GetRendererInput().m_EnableGLFlush, true );
    EXPECT_EQ( config.GetRendererInput().m_VerticalBufferFrameCount, 1 );

    EXPECT_EQ( config.GetWindowMode(), WindowMode::WINDOWED );
    EXPECT_EQ( config.DefaultWidth(), 1920 );
    EXPECT_EQ( config.DefaultHeight(), 1080 );
    EXPECT_EQ( config.DefaultWindowWidth(), 1920 );
    EXPECT_EQ( config.DefaultWindowHeight(), 1080 );

    EXPECT_EQ( config.EventLoopUpdateMillis(), 20 );
    EXPECT_EQ( config.EnableLockingQueue(), false );

    EXPECT_EQ( config.SockerServerPort(), 12345 );

    EXPECT_EQ( config.CameraPosition(), glm::vec3( 0, 0, 5 ) );
    EXPECT_EQ( config.CameraDirection(), glm::vec3( 0, 0, 5 ) );
    EXPECT_EQ( config.FOV(), 90 );
}


