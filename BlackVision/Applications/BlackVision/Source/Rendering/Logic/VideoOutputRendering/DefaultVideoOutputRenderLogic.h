#pragma once

#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "VideoCardManager.h"

namespace bv {

class Renderer;
class OffscreenRenderLogic;

class DefaultVideoOutputRenderLogic
{
private:

    bool    m_useVideoCard;
    bool    m_displayVideoOutputOnPreview;

    unsigned int m_width;
    unsigned int m_height;

    unsigned int m_curReadbackFrame;

public:


            DefaultVideoOutputRenderLogic   ( bool useVideoCard, bool displayVideoOutputOnPreview );
            ~DefaultVideoOutputRenderLogic  ();

    void    FrameRendered                   ( Renderer * renderer, OffscreenRenderLogic * offscreenRenderLogic, videocards::VideoCardManager * videoCardManager );
    void    FrameRenderedNewImpl            ( Renderer * renderer, OffscreenRenderLogic * offscreenRenderLogic );

private:

    void    PushToVideoCard                 ( Texture2DConstPtr & frame, videocards::VideoCardManager * videoCardManager );

};


} //bv
