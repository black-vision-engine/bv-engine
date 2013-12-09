#include "BVConfig.h"

namespace bv
{

// *********************************
//FIXME: read default values from a configuration file
BVConfig::BVConfig                      ()
{
}

// *********************************
//
BVConfig::~BVConfig                     ()
{
}

// *********************************
//
int     BVConfig::DefaultWidth          () const
{
    return m_defaultWidth;
}

// *********************************
//
int     BVConfig::DefaultHeight         () const
{
    return m_defaultHeight;
}

// *********************************
//
bool    BVConfig::FullScreenMode        () const
{
    return m_fullscreeMode;
}

// *********************************
//
bool    BVConfig::ReadbackFlag          () const
{
    return m_readbackOn;
}

// *********************************
//
const BVConfig & BVConfig::Instance     ()
{
    static BVConfig instance;

    return instance;
}

} //bv
