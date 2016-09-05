#pragma once

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {

class Renderer;
class RenderTarget;
class FullscreenEffectInstance;
class RenderLogicContext;


class VideoOutputRenderLogic
{
private:

    FullscreenEffectInstance *      m_videoOutputEffect;
    std::vector< RenderTarget * >   m_vidTargets;

    unsigned int                    m_height;
    bool                            m_interlaceOdd;

    Texture2DPtr                    m_videoFrame;

public:

    VideoOutputRenderLogic          ( unsigned int height, bool interlaceOdd = false );
    ~VideoOutputRenderLogic         ();

    void    Render                  ( RenderTarget * videoRenderTarget, RenderTarget * curFrameRenderTarget, RenderTarget * prevFrameRenderTarget, RenderLogicContext * ctx );

    void    VideoFrameRendered      ( RenderTarget * videoRenderTarget, RenderLogicContext * ctx );

    void    SetChannelMapping       ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx );
    void    SetOverwriteAlpha       ( bool overwriteAlpha );
    void    SetAlpha                ( float alpha );
    void    SetHeight               ( int height );

    Texture2DPtr GetLastVideoFrame  ();

};

} //bv
