#pragma once

namespace bv {

class Renderer;
class OffscreenRenderLogic;

class DefaultVideoOutputRenderLogic
{
private:

    bool    m_useVideoCard;
    bool    m_displayVideoOutputOnPreview;

public:


            DefaultVideoOutputRenderLogic   ( bool useVideoCard, bool displayVideoOutputOnPreview );
            ~DefaultVideoOutputRenderLogic  ();

    void    FrameRendered                   ( Renderer * renderer, OffscreenRenderLogic * offscreenRenderLogic );

};


} //bv
