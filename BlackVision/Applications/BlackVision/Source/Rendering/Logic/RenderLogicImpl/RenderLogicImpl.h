#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffectRenderLogicSelector.h"


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

    NodeEffectRenderLogicSelector   m_nodeEffectRenderLogicSelector;

    RenderTargetStackAllocator *    m_rtStackAllocator;
    OffscreenDisplay *              m_offscreenDisplay;

public:

                        RenderLogicImpl     ( RenderTargetStackAllocator * renderTargetStackAllocator, bool useTwoDisplayRenderTargets );
    virtual             ~RenderLogicImpl    ();


    virtual     void    RenderFrame         ( Renderer * renderer, SceneNode * sceneRoot );
                void    RenderNode          ( Renderer * renderer, SceneNode * node );

protected:

                void    PreRenderFrame      ( Renderer * renderer );
                void    PostRenderFrame     ( Renderer * renderer );

};

} // bv
