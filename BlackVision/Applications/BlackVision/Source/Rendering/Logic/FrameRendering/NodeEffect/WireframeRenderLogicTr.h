#pragma once
//
//
//#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"
//
//
//namespace bv {
//
//class WireframeEffect;
//
//class WireframeRenderLogic : public NodeEffectRenderLogic
//{
//private:
//
//    WireframeEffect     *   m_effect;
//
//public:
//
//                    WireframeRenderLogic        ( FrameRenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
//    virtual         ~WireframeRenderLogic       ();
//
//    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;
//
//private:
//
//    void            RenderWireframeOverlay      ( Renderer * renderer, SceneNode * node );
//
//    void            EnableWireframeEffect       ( Renderer * renderer, SceneNode * node );
//
//    void            DrawWirefreameNodeOnly      ( Renderer * renderer, SceneNode * node );
//    void            DrawWireframeChildren       ( Renderer * renderer, SceneNode * node );
//
//};
//
//
//} //bv
