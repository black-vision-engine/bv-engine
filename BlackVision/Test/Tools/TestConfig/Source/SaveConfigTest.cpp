#include "gtest/gtest.h"

#include "BVConfig.h"

using namespace bv;



// ***********************
//
TEST( Tools_Config, Saving_SaveLoadCompare )
{
    BVConfig config( "TestConfigs/ConfigsTest/NotDefaultConfig.xml" );
    config.SaveConfig( "SavedConfig.xml" );

    BVConfig loadSavedConfig( "SavedConfig.xml" );

    EXPECT_EQ( config.PMFolder(), loadSavedConfig.PMFolder() );
    EXPECT_EQ( config.FullScreenMode(), loadSavedConfig.FullScreenMode() );
    EXPECT_EQ( config.ReadbackFlag(), loadSavedConfig.ReadbackFlag() );

    EXPECT_EQ( config.LoadSceneFromEnv(), loadSavedConfig.LoadSceneFromEnv() );
    EXPECT_EQ( config.LoadSceneFromProjectManager(), loadSavedConfig.LoadSceneFromProjectManager() );
    EXPECT_EQ( config.SceneFromEnvName(), loadSavedConfig.SceneFromEnvName() );

    EXPECT_EQ( config.UseDebugLayer(), loadSavedConfig.UseDebugLayer() );
    EXPECT_EQ( config.DebugFilePath(), loadSavedConfig.DebugFilePath() );

    EXPECT_EQ( config.OnFailedTexLoadBehavior(), loadSavedConfig.OnFailedTexLoadBehavior() );

    EXPECT_EQ( config.GlobalGain(), loadSavedConfig.GlobalGain() );

    EXPECT_EQ( config.FPS(), loadSavedConfig.FPS() );
    EXPECT_EQ( config.TimerFPS(), loadSavedConfig.TimerFPS() );
    EXPECT_EQ( config.ClearColor(), loadSavedConfig.ClearColor() );

    EXPECT_EQ( config.RenderToSharedMemory(), loadSavedConfig.RenderToSharedMemory() );

    EXPECT_EQ( config.GetRendererInput().m_DisableVerticalSync, loadSavedConfig.GetRendererInput().m_DisableVerticalSync );
    EXPECT_EQ( config.GetRendererInput().m_EnableGLFinish, loadSavedConfig.GetRendererInput().m_EnableGLFinish );
    EXPECT_EQ( config.GetRendererInput().m_EnableGLFlush, loadSavedConfig.GetRendererInput().m_EnableGLFlush );
    EXPECT_EQ( config.GetRendererInput().m_VerticalBufferFrameCount, loadSavedConfig.GetRendererInput().m_VerticalBufferFrameCount );

    EXPECT_EQ( config.GetWindowMode(), loadSavedConfig.GetWindowMode() );
    EXPECT_EQ( config.DefaultWidth(), loadSavedConfig.DefaultWidth() );
    EXPECT_EQ( config.DefaultHeight(), loadSavedConfig.DefaultHeight() );
    EXPECT_EQ( config.DefaultWindowWidth(), loadSavedConfig.DefaultWindowWidth() );
    EXPECT_EQ( config.DefaultWindowHeight(), loadSavedConfig.DefaultWindowHeight() );

    EXPECT_EQ( config.EventLoopUpdateMillis(), loadSavedConfig.EventLoopUpdateMillis() );
    EXPECT_EQ( config.EnableLockingQueue(), loadSavedConfig.EnableLockingQueue() );

    EXPECT_EQ( config.SockerServerPort(), loadSavedConfig.SockerServerPort() );

    EXPECT_EQ( config.CameraPosition(), loadSavedConfig.CameraPosition() );
    EXPECT_EQ( config.CameraDirection(), loadSavedConfig.CameraDirection() );
    EXPECT_EQ( config.FOV(), loadSavedConfig.FOV() );

}
