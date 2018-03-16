
namespace bv
{

// *********************************
//
int            BVConfig::DefaultWindowWidth                     () const
{
    return m_defaultWindowWidth;
}

// *********************************
//
int            BVConfig::DefaultWindowHeight                    () const
{
    return m_defaultWindowHeight;
}

// *********************************
//
inline int     BVConfig::DefaultWidth                           () const
{
    return m_defaultWidth;
}

// *********************************
//
inline int     BVConfig::DefaultHeight                          () const
{
    return m_defaultHeight;
}

// *********************************
//
inline bool    BVConfig::FullScreenMode                         () const
{
    return m_fullscreeMode;
}

// *********************************
//
inline bool    BVConfig::ReadbackFlag                           () const
{
    return m_readbackOn;
}

// *********************************
//
inline bool     BVConfig::DisplayVideoCardOutput                () const
{
    return m_displayVideoCardOutput;
}

// *********************************
//
inline bool     BVConfig::IsCameraPerspactive                   () const
{
    return m_isCameraPerspective;
}

// *********************************
//
inline unsigned int BVConfig::EventLoopUpdateMillis             () const
{
    return m_eventLoopUpdateMillis;
}

// *********************************
//
inline unsigned int BVConfig::FPS                               () const
{
    return m_fps;   
}

// *********************************
//
inline unsigned int BVConfig::FrameTimeMillis                   () const
{
    return m_frameTimeMillis;
}

// *********************************
//
inline float        BVConfig::FOV                               () const
{
    return m_defaultFOV;
}

// *********************************
//
inline float        BVConfig::NearClippingPlane                 () const
{
    return m_defaultNearClippingPlane;
}

// *********************************
//
inline float        BVConfig::FarClippingPlane                  () const
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
inline float                BVConfig::ClearDepth                () const
{
    return m_defaultClearDepth;
}

// *********************************
//
inline unsigned int         BVConfig::StatsMAWindowSize         () const
{
    return m_defaultStatsMovingAverageWindowSize;
}

// *********************************
//
inline unsigned int         BVConfig::MAVWarmupRounds           () const
{
    return m_defaultWarmupRoundsStatsMAV;
}

// *********************************
//
inline unsigned int         BVConfig::StatsRefreshMillisDelta   () const
{
    return m_defaultStatsRefreshMillisDelta;
}

// *********************************
//
inline unsigned int         BVConfig::StatsRecalcFramesDelta    () const
{
    return m_defaultStatsRecalcFramesDelta;
}

// *********************************
//
inline unsigned int         BVConfig::NumRedbackBuffersPerRT    () const
{
    return m_numRedbackBuffersPerRenderTarget;
}

// *********************************
//
inline unsigned int         BVConfig::ProfilerDispWaitMillis    () const
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
inline std::string          BVConfig::DefaultSceneEnvVarName    () const
{
    return m_defaultSceneEnvVarName;
}

} //bv
