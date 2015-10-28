#pragma once

#include <vector>


namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class Camera;
class NodeEffectRenderLogic;


class RenderLogic
{
private:

    enum CustomLogicType
    {
        CLT_DEFAULT = 0,
        CLT_ALPHA_MASK,
        CLT_NODE_MASK,

        CLT_TOTAL
    };

private:

    OffscreenRenderLogic *                  m_offscreenRenderLogic;

    std::vector< NodeEffectRenderLogic * >  m_customNodeRenderLogic;

    mutable CustomLogicType m_yp;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrameTM   ( Renderer * renderer, SceneNode * node );
    void    PreFrameSetupTM ( Renderer * renderer );
    void    PostFrameSetupTM( Renderer * renderer );

public:

    void    RenderNodeTM    ( Renderer * renderer, SceneNode * node );

    bool    UseDefaultMaskTM( SceneNode * node ) const;
    bool    UseAlphaMaskTM  ( SceneNode * node ) const;
    bool    UseNodeMaskTM   ( SceneNode * node ) const;

    NodeEffectRenderLogic *     GetNodeEffectRenderLogic    ( SceneNode * node ) const;

    void    DrawNodeTM      ( Renderer * renderer, SceneNode * node );

    void    DrawNodeOnlyTM  ( Renderer * renderer, SceneNode * node );
    void    DrawChildrenTM  ( Renderer * renderer, SceneNode * node, int firstChildIdx = 0 );

public:

    void    FrameRendered   ( Renderer * renderer );
    void    PrintGLStats    ( bool detailed );

};

} // bv
