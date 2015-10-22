#pragma once

//pablito:
#include "VideoCardManager.h"
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

    OffscreenRenderLogic *  m_offscreenRenderLogic;
	bv::videocards::VideoCardManager *      m_VideoCardManager;
    enum CustomLogicType
    {
        CLT_DEFAULT = 0,
        CLT_ALPHA_MASK,
        CLT_NODE_MASK,

        CLT_TOTAL
    };

    std::vector< NodeEffectRenderLogic * >  m_customNodeRenderLogic;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * node );

	//pablito
	void	SetVideoCardManager(bv::videocards::VideoCardManager* videoCardManager, Renderer * renderer);
	void	InitVideoCards     ( Renderer * renderer );
// Temporary transition code START
    void    RenderFrameTM   ( Renderer * renderer, SceneNode * node );
    void    PreFrameSetupTM ( Renderer * renderer );
    void    PostFrameSetupTM( Renderer * renderer );
// Temporary transition code END

public:

    void    RenderNode      ( Renderer * renderer, SceneNode * node );

// Temporary transition code START
    void    RenderNodeTM    ( Renderer * renderer, SceneNode * node );
    bool    UseDefaultMask  ( SceneNode * node ) const;
    bool    UseAlphaMask    ( SceneNode * node ) const;
    bool    UseNodeMask     ( SceneNode * node ) const;

    NodeEffectRenderLogic *     GetNodeEffectRenderLogic    ( SceneNode * node ) const;
// Temporary transition code END

    void    RenderVanilla   ( Renderer * renderer, SceneNode * node );
    void    RenderAlphaMask ( Renderer * renderer, SceneNode * node );
    void    RenderNodeMask  ( Renderer * renderer, SceneNode * node );
    void    RenderNodeMask1 ( Renderer * renderer, SceneNode * node );

    void    DrawNode        ( Renderer * renderer, SceneNode * node );

    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    DrawChildren    ( Renderer * renderer, SceneNode * node, int firstChildIdx = 0 );

public:

    void    FrameRendered   ( Renderer * renderer );
    void    PrintGLStats    ( bool detailed );

};

} // bv
