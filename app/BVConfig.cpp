#include "BVConfig.h"

//#define USE_READBACK_API
//#define FULLSCREEN_MODE


namespace bv
{

// *********************************
//FIXME: read default values from a configuration file
BVConfig::BVConfig                      ()
{
#ifdef FULLSCREEN_MODE
    m_defaultWidth      = 1920;
    m_defaultHeight     = 1080;
    m_fullscreeMode     = true;
#else
    m_defaultWidth      = 960;
    m_defaultHeight     = 540;
    m_fullscreeMode     = false;
#endif

    m_fps               = 6000;
    m_frameTimeMillis   = 1000 / m_fps;

#ifdef USE_READBACK_API
    m_readbackOn        = true;
#else
    m_readbackOn        = false;
#endif

    m_eventLoopUpdateMillis = 20;

    m_defaultFOV                = 90.f;
    m_defaultNearClippingPlane  = 0.1f;
    m_defaultFarClippingPlane   = 100.f;

    m_defaultCameraPosition  = glm::vec3( 0.f, 0.f, 1.f );
    m_defaultCameraDirection = glm::vec3( 0.f, 0.f, 0.f );
    m_defaultCameraUp        = glm::vec3( 0.f, 1.f, 0.f );

    m_defaultStatsMovingAverageWindowSize   = 500; //500
    m_defaultWarmupRoundsStatsMAV           = 10; //10
    m_defaultStatsRefreshMillisDelta        = 300;
    m_defaultStatsRecalcFramesDelta         = m_defaultStatsMovingAverageWindowSize * m_defaultWarmupRoundsStatsMAV; //* 30
    m_defaultProfilerDisplayWaitMillis      = 10000; //1000

    m_defaultClearColor     = glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f );
    m_defaultClearDepth     = 1.0f;

}

// *********************************
//
BVConfig::~BVConfig                     ()
{
}

// *********************************
//
const BVConfig & BVConfig::Instance     ()
{
    static BVConfig instance;

    return instance;
}

} //bv
