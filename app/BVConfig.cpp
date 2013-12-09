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

    m_fps               = 1000;
    m_frameTimeMillis   = 1000 / m_fps;

#ifdef USE_READBACK_API
    m_readbackOn        = true;
#else
    m_readbackOn        = false;
#endif

    m_eventLoopUpdateMillis = 20;
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
