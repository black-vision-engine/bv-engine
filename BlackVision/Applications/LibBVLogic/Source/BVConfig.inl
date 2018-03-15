#include "BVConfig.h"

namespace bv
{

// *********************************
//
Int32           BVConfig::DefaultWindowWidth                     () const
{
    return m_defaultWindowWidth;
}

// *********************************
//
Int32           BVConfig::DefaultWindowHeight                    () const
{
    return m_defaultWindowHeight;
}

// *********************************
//
inline Int32    BVConfig::DefaultWidth                           () const
{
    return m_defaultWidth;
}

// *********************************
//
inline Int32    BVConfig::DefaultHeight                          () const
{
    return m_defaultHeight;
}

// *********************************
//
inline bool     BVConfig::FullScreenMode                         () const
{
    return m_fullscreeMode;
}

// *********************************
//
inline WindowMode    BVConfig::GetWindowMode                   () const
{
    return m_windowMode;
}

// *********************************
//
inline RendererInput    BVConfig::GetRendererInput                   () const
{
    return m_rendererInput;
}

// *********************************
//
inline bool     BVConfig::ReadbackFlag                           () const
{
    return m_readbackOn;
}

// *********************************
//
inline bool     BVConfig::IsCameraPerspactive                   () const
{
    return m_isCameraPerspective;
}

// *********************************
//
inline bool     BVConfig::RenderToSharedMemory                   () const
{
    return m_renderToSharedMemory;
}

// *********************************
//
inline Float32  BVConfig::GlobalGain							() const
{
	return m_globalGain;
}

// *********************************
//
inline UInt32       BVConfig::EventLoopUpdateMillis             () const
{
    return m_eventLoopUpdateMillis;
}

// *********************************
//
inline UInt32       BVConfig::FPS                               () const
{
    return m_fps;   
}

// *********************************
//
inline UInt32       BVConfig::FrameTimeMillis                   () const
{
    return m_frameTimeMillis;
}

// *********************************
//
inline UInt32       BVConfig::TimerFPS                          () const
{
    return m_timerFPS;
}

// *********************************
//
inline Float32      BVConfig::FOV                               () const
{
    return m_defaultFOV;
}

// *********************************
//
inline Float32      BVConfig::NearClippingPlane                 () const
{
    return m_defaultNearClippingPlane;
}

// *********************************
//
inline Float32      BVConfig::FarClippingPlane                  () const
{
    return m_defaultFarClippingPlane;
}

// *********************************
//
inline const glm::vec3 &    BVConfig::CameraPosition            () const
{
    return m_defaultCameraPosition;
}

// *********************************
//
inline const glm::vec3 &    BVConfig::CameraUp                  () const
{
    return m_defaultCameraUp;
}

// *********************************
//
inline const glm::vec3 &    BVConfig::CameraDirection           () const
{
    return m_defaultCameraDirection;
}

// *********************************
//
inline const glm::vec4 &    BVConfig::ClearColor                () const
{
    return m_defaultClearColor;
}

// *********************************
//
inline Float32              BVConfig::ClearDepth                () const
{
    return m_defaultClearDepth;
}

// *********************************
//
inline UInt32               BVConfig::StatsMAWindowSize         () const
{
    return m_defaultStatsMovingAverageWindowSize;
}

// *********************************
//
inline UInt32               BVConfig::MAVWarmupRounds           () const
{
    return m_defaultWarmupRoundsStatsMAV;
}

// *********************************
//
inline UInt32               BVConfig::StatsRefreshMillisDelta   () const
{
    return m_defaultStatsRefreshMillisDelta;
}

// *********************************
//
inline UInt32               BVConfig::StatsRecalcFramesDelta    () const
{
    return m_defaultStatsRecalcFramesDelta;
}

// *********************************
//
inline UInt32               BVConfig::NumRedbackBuffersPerRT    () const
{
    return m_numRedbackBuffersPerRenderTarget;
}

// *********************************
//
inline UInt32               BVConfig::ProfilerDispWaitMillis    () const
{
    return m_defaultProfilerDisplayWaitMillis;
}

// *********************************
//
inline const char *         BVConfig::FrameStatsSection         () const
{
    return "FRAME";
}

// *********************************
//
inline const std::string &  BVConfig::SceneFromEnvName          () const
{
    return m_sceneFromEnvName;
}

// *********************************
//
inline const std::string &  BVConfig::LoadSceneFromProjectManager() const
{
    return m_loadSceneFromProjectManager;
}

// *********************************
//
inline std::string          BVConfig::DefaultSceneEnvVarName    () const
{
    return m_defaultSceneEnvVarName;
}

// *********************************
//
inline const std::string &  BVConfig::PMFolder                  () const
{
    return m_pmFolder;
}

// *********************************
//
inline Int32                BVConfig::SockerServerPort          () const
{
    return m_sockerServerPort;
}

// *********************************
//
inline bool                 BVConfig::UseDebugLayer             () const
{
    return m_useDebugLayer;
}

// *********************************
//
inline const std::string &  BVConfig::DebugFilePath             () const
{
    return m_debugFilePath;
}

// *********************************
//
inline bool                 BVConfig::LoadSceneFromEnv          () const
{
    return m_loadSceneFromEnv;
}

// *********************************
//
inline bool                 BVConfig::EnableLockingQueue        () const
{
    return m_enableQueueLocking;
}

// ***********************
//
inline const std::string &  BVConfig::OnFailedTexLoadBehavior   () const
{
    return m_onFailedTextureLoadBehavior;
}


} //bv
