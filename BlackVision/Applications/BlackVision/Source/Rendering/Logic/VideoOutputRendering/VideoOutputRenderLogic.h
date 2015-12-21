#pragma once


namespace bv {

class InterlaceOverwriteChannelsFullscreenEffect;


class VideoOutputRenderLogic
{
private:

    InterlaceOverwriteChannelsFullscreenEffect *    m_effect;

    unsigned int                                    m_height;
    bool                                            m_interlaceOdd;

public:

    VideoOutputRenderLogic          ( unsigned int height, bool interlaceOdd = false );
    ~VideoOutputRenderLogic         ();

    void    Render                  ();

private:

    InterlaceOverwriteChannelsFullscreenEffect *  AccessEffect  ();

};

} //bv
