#pragma once

namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class RenderLogic;

class NodeEffectRenderLogic
{
private:

    RenderLogic *           m_renderLogic;
    OffscreenRenderLogic *  m_offscreenRenderLogic;

public:

                    NodeEffectRenderLogic   ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~NodeEffectRenderLogic  ();

    virtual void    RenderNode              ( Renderer * renderer, SceneNode * node ) = 0;

protected:

    RenderLogic *           GetRenderLogic          ();
    OffscreenRenderLogic *  GetOffscreenRenderLogic ();

};

} //bv
