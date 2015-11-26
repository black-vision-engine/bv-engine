#pragma once

#include <vector>


namespace bv {

class Camera;
class Renderer;
class SceneNode;

class FrameRenderLogic;
class PostFrameRenderLogic;

class RenderLogic
{
private:

    FrameRenderLogic *                      m_frameRenderLogic;
    PostFrameRenderLogic *                  m_postFrameRenderLogic;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * node );

};

} // bv
