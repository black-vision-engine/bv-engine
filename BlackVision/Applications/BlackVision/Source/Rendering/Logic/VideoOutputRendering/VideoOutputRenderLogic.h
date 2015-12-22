#pragma once


namespace bv {

class Renderer;
class RenderTarget;
class VideoOutputFullscreenEffect;


class VideoOutputRenderLogic
{
private:

    VideoOutputFullscreenEffect *   m_effect;

    unsigned int                    m_height;
    bool                            m_interlaceOdd;

public:

    VideoOutputRenderLogic          ( unsigned int height, bool interlaceOdd = false );
    ~VideoOutputRenderLogic         ();

    void    Render                  ( Renderer * renderer, RenderTarget * videoRenderTarget, RenderTarget * curFrameRenderTarget, RenderTarget * prevFrameRenderTarget );

    void    SetChannelMapping       ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx );
    void    SetOverwriteAlpha       ( bool overwriteAlpha );
    void    SetAlpha                ( float alpha );
    void    SetHeight               ( int height );

private:

    VideoOutputFullscreenEffect *  AccessEffect  ( RenderTarget * curFrameRenderTarget, RenderTarget * prevFrameRenderTarget );

};

} //bv
