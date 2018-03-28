#pragma once

#include <string>

#include "Mathematics/glm_inc.h"


namespace bv
{

class BVConfig
{
private:

    int             m_defaultWindowWidth;
    int             m_defaultWindowHeight;

    int             m_defaultWidth;
    int             m_defaultHeight;

    unsigned int    m_eventLoopUpdateMillis;
    unsigned int    m_fps;
    unsigned int    m_frameTimeMillis;

    bool            m_fullscreeMode;
    bool            m_readbackOn;
    bool            m_displayVideoCardOutput;
    bool            m_isCameraPerspective;

    float           m_defaultFOV;
    float           m_defaultNearClippingPlane;
    float           m_defaultFarClippingPlane;

    glm::vec3       m_defaultCameraPosition;
    glm::vec3       m_defaultCameraUp;
    glm::vec3       m_defaultCameraDirection;

    unsigned int    m_defaultStatsMovingAverageWindowSize;
    unsigned int    m_defaultWarmupRoundsStatsMAV;
    unsigned int    m_defaultStatsRefreshMillisDelta;
    unsigned int    m_defaultStatsRecalcFramesDelta;
    unsigned int    m_defaultProfilerDisplayWaitMillis;

    unsigned int    m_numRedbackBuffersPerRenderTarget;

    glm::vec4       m_defaultClearColor;
    float           m_defaultClearDepth;

    std::string     m_defaultSceneEnvVarName;

private:

    BVConfig    ();
    ~BVConfig   ();

public:

    inline int                  DefaultWindowWidth      () const;
    inline int                  DefaultWindowHeight     () const;

    inline int                  DefaultWidth            () const;
    inline int                  DefaultHeight           () const;

    inline bool                 FullScreenMode          () const;
    inline bool                 ReadbackFlag            () const;
    inline bool                 DisplayVideoCardOutput  () const;
    inline bool                 IsCameraPerspactive     () const;

    inline unsigned int         EventLoopUpdateMillis   () const;
    inline unsigned int         FPS                     () const;
    inline unsigned int         FrameTimeMillis         () const;

    inline float                FOV                     () const;
    inline float                NearClippingPlane       () const;
    inline float                FarClippingPlane        () const;

    inline const glm::vec3 &    CameraPosition          () const;
    inline const glm::vec3 &    CameraUp                () const;
    inline const glm::vec3 &    CameraDirection         () const;

    inline const glm::vec4 &    ClearColor              () const;
    inline float                ClearDepth              () const;

    inline unsigned int         StatsMAWindowSize       () const;
    inline unsigned int         MAVWarmupRounds         () const;
    inline unsigned int         StatsRefreshMillisDelta () const;
    inline unsigned int         StatsRecalcFramesDelta  () const;

    inline unsigned int         NumRedbackBuffersPerRT  () const;

    inline unsigned int         ProfilerDispWaitMillis  () const;

    inline const char *         FrameStatsSection       () const;

    inline std::string          DefaultSceneEnvVarName  () const;

    static const BVConfig & Instance  ();

};

#define DefaultConfig   BVConfig::Instance()

} //bv

#include "BVConfig.inl"
