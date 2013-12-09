#pragma once

namespace bv
{

class BVConfig
{
private:

    int             m_defaultWidth;
    int             m_defaultHeight;

    unsigned int    m_eventLoopUpdateMillis;
    unsigned int    m_fps;
    unsigned int    m_frameTimeMillis;

    bool            m_fullscreeMode;
    bool            m_readbackOn;

private:

    BVConfig();
    ~BVConfig();

public:

    inline int          DefaultWidth            () const;
    inline int          DefaultHeight           () const;

    inline bool         FullScreenMode          () const;
    inline bool         ReadbackFlag            () const;

    inline unsigned int EventLoopUpdateMillis   () const;
    inline unsigned int FPS                     () const;
    inline unsigned int FrameTimeMillis         () const;

    static const BVConfig & Instance  ();

};

} //bv

#include "BVConfig.inl"
