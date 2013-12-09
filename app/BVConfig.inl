#include "BVConfig.h"

namespace bv
{

// *********************************
//
inline int     BVConfig::DefaultWidth          () const
{
    return m_defaultWidth;
}

// *********************************
//
inline int     BVConfig::DefaultHeight         () const
{
    return m_defaultHeight;
}

// *********************************
//
inline bool    BVConfig::FullScreenMode        () const
{
    return m_fullscreeMode;
}

// *********************************
//
inline bool    BVConfig::ReadbackFlag          () const
{
    return m_readbackOn;
}

// *********************************
//
inline unsigned int BVConfig::EventLoopUpdateMillis () const
{
    return m_eventLoopUpdateMillis;
}

// *********************************
//
inline unsigned int BVConfig::FPS                   () const
{
    return m_fps;   
}

// *********************************
//
inline unsigned int BVConfig::FrameTimeMillis       () const
{
    return m_frameTimeMillis;
}

} //bv
