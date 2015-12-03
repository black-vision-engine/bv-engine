#pragma once

namespace bv {

class RenderTargetStackAllocator;
class Renderer;
class SceneNode;
class OffscreenDisplay;


class RenderLogicImpl
{
private:

    bool                    m_useVideoCard;

protected:

    RenderTargetStackAllocator *    m_rtStackAllocator;
    OffscreenDisplay *              m_offscreenDisplay;

public:

                        RenderLogicImpl     ( RenderTargetStackAllocator * renderTargetStackAllocator, bool useTwoDisplayRenderTargets );
    virtual             ~RenderLogicImpl    ();


    virtual     void    RenderFrame         ( Renderer * renderer, SceneNode * sceneRoot );

protected:

                void    PreRenderFrame      ( Renderer * renderer );
                void    PostRenderFrame     ( Renderer * renderer );

};

} // bv
