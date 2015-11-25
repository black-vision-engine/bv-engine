#pragma once

namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class FrameRenderLogic;

class NodeEffectRenderLogic
{
private:

    FrameRenderLogic *      m_renderLogic;
    OffscreenRenderLogic *  m_offscreenRenderLogic;

public:

                    NodeEffectRenderLogic   ( FrameRenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~NodeEffectRenderLogic  ();

    virtual void    RenderNode              ( Renderer * renderer, SceneNode * node ) = 0;

protected:

    FrameRenderLogic *      GetRenderLogic          ();
    OffscreenRenderLogic *  GetOffscreenRenderLogic ();

};

} //bv
