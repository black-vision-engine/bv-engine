#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"


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

    void    FrameRendered                   ( Renderer * renderer, OffscreenRenderLogic * offscreenRenderLogic );

private:

    void    PushToVideoCard                 ( Texture2DConstPtr & frame );

};


} //bv
