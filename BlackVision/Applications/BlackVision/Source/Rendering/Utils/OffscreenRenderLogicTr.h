//#pragma once
//
//#include <vector>
//
//#include "Engine/Types/Enums.h"
//#include "Engine/Graphics/Resources/Texture2D.h"
//
//#include "Engine/Graphics/Effects/Texture2DEffect.h"
//#include "Engine/Graphics/Effects/Texture2DEffectWithMask.h"
//
//#include "Rendering/Utils/RenderTargetStack.h"
//
//
//namespace bv {
//
//class RenderTarget;
//class Renderer;
//class Camera;
//class TriangleStrip;
//class Rednerer;
//class Texture2DEffect;
//class Texture2DEffectWithMask;
//class IValue;
//
//namespace {
//
//struct RenderTargetData
//{
//    RenderTarget *      renderTarget;
//    TriangleStrip *     quad;
//
//    RenderTargetData    ();
//    ~RenderTargetData   ();
//};
//
//struct TextureData
//{
//    unsigned int        m_width;
//    unsigned int        m_height;
//
//    TextureFormat       m_fmt;
//
//    TextureData         ( unsigned int width, unsigned int height, TextureFormat fmt );
//
//};
//
//struct OffscreenRenderData
//{
//    TriangleStrip *             auxQuad;
//
//    Texture2DEffectPtr          effectTexture2D;
//    Texture2DEffectWithMaskPtr  effectTexture2DWithMask;
//
//            OffscreenRenderData();
//
//    void    UseTexture2DEffect  ( const IValue * val, Texture2DPtr tex );
//    void    UseTexture2DEffect  ( const IValue * val, Texture2DPtr tex, Texture2DPtr mask );
//};
//
//typedef std::vector< RenderTarget * >  RenderTargetVec;
//
//} // anonymous
//
//class OffscreenRenderLogicTr
//{
//private:
//
//    Camera *            m_displayCamera;
//    Camera *            m_rendererCamera;
//
//public:
//
//                        OffscreenRenderLogicTr      ();
//                        ~OffscreenRenderLogicTr     ();
//
//    void                SetRendererCamera           ( Camera * camera );
//
//    void                AllocateNewRenderTarget     ( Renderer * renderer );
//    void                EnableTopRenderTarget       ( Renderer * renderer );
//    void                DiscardCurrentRenderTarget  ( Renderer * renderer );
//    void                DisableTopRenderTarget      ( Renderer * renderer );
//
//    void                DrawTopAuxRenderTarget      ( Renderer * renderer, const IValue * alphaVal );
//    void                DrawAMTopTwoRenderTargets   ( Renderer * renderer, const IValue * alphaVal );
//
//    void                DrawWithAllVideoEffects     ( Renderer * renderer );
//    void                DrawDisplayRenderTarget     ( Renderer * renderer );
//    void                SwapDisplayRenderTargets    ();
//
//    unsigned int        TotalNumReadBuffers         () const;
//    unsigned int        NumReadBuffersPerRT         () const;
//
//    Texture2DConstPtr   ReadDisplayTarget           ( Renderer * renderer, unsigned int bufNum );
//
//private:
//
//    RenderTarget *      GetRenderTargetAt               ( int i );
//
//    RenderTargetData    CreateDisplayRenderTargetData       () const;
//    RenderTargetData    CreateVideoOutputRenderTargetData   () const;
//
//    unsigned int        CurDisplayRenderTargetNum           () const;
//    RenderTargetData &  CurDisplayRenderTargetData          ();
//
//};
//
//} //bv
