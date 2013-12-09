#pragma once

namespace bv
{

class BVConfig
{
private:

    int     m_defaultWidth;
    int     m_defaultHeight;

    bool    m_fullscreeMode;
    bool    m_readbackOn;

private:

    BVConfig();
    ~BVConfig();

public:

    int     DefaultWidth        () const;
    int     DefaultHeight       () const;

    bool    FullScreenMode      () const;
    bool    ReadbackFlag        () const;

    static const BVConfig & Instance  ();

};

} //bv
