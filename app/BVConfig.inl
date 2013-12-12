#include "BVConfig.h"

namespace bv
{

// *********************************
//
inline int     BVConfig::DefaultWidth                   () const
{
    return m_defaultWidth;
}

// *********************************
//
inline int     BVConfig::DefaultHeight                  () const
{
    return m_defaultHeight;
}

// *********************************
//
inline bool    BVConfig::FullScreenMode                 () const
{
    return m_fullscreeMode;
}

// *********************************
//
inline bool    BVConfig::ReadbackFlag                   () const
{
    return m_readbackOn;
}

// *********************************
//
inline unsigned int BVConfig::EventLoopUpdateMillis     () const
{
    return m_eventLoopUpdateMillis;
}

// *********************************
//
inline unsigned int BVConfig::FPS                       () const
{
    return m_fps;   
}

// *********************************
//
inline unsigned int BVConfig::FrameTimeMillis           () const
{
    return m_frameTimeMillis;
}

// *********************************
//
inline float        BVConfig::FOV                       () const
{
    return m_defaultFOV;
}

// *********************************
//
inline float        BVConfig::NearClippingPlane         () const
{
    return m_defaultNearClippingPlane;
}

// *********************************
//
inline float        BVConfig::FarClippingPlane          () const
{
    return m_defaultFarClippingPlane;
}

// *********************************
//
inline const glm::vec3 &    BVConfig::CameraPosition    () const
{
    return m_defaultCameraPosition;
}

// *********************************
//
inline const glm::vec3 &    BVConfig::CameraUp          () const
{
    return m_defaultCameraUp;
}

// *********************************
//
inline const glm::vec3 &    BVConfig::CameraDirection   () const
{
    return m_defaultCameraDirection;
}

} //bv
