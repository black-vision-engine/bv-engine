#pragma once

#include <vector>

#include "Engine/Types/Enums.h"
#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Effects/Texture2DEffect.h"
#include "Engine/Graphics/Effects/Texture2DEffectWithMask.h"

namespace bv {

class RenderTarget;
class Renderer;
class Camera;
class TriangleStrip;
class Rednerer;
class Texture2DEffect;
class Texture2DEffectWithMask;
class IValue;

struct RenderTargetData
{
    RenderTarget *              renderTarget;
    TriangleStrip *             quad;
    Texture2DEffectPtr          effectTexture2D;
    Texture2DEffectWithMaskPtr  effectTexture2DWithMask;

    RenderTargetData    ();
    ~RenderTargetData   ();
};

typedef std::vector< RenderTargetData >  RenderTargetDataVec;

class OffscreenRenderLogic
{
private:

    unsigned int        m_width;
    unsigned int        m_height;

    TextureFormat       m_fmt;

    unsigned int        m_usedStackedRenderTargets;
    bool                m_topRenderTargetEnabled;

    RenderTargetDataVec m_auxRenderTargetsData;
    RenderTargetData    m_displayRenderTargetData[ 2 ];

    unsigned int        m_curDisplayTarget;
    unsigned int        m_buffersPerTarget;

    std::vector< Texture2DPtr > m_readbackTextures;

    Camera *            m_displayCamera;
    Camera *            m_rendererCamera;

    bool                m_displayRTEnabled;

public:

                        OffscreenRenderLogic        ( unsigned int width, unsigned int height, unsigned int numReadBuffers, Camera * camera = nullptr, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~OffscreenRenderLogic       ();

    void                SetRendererCamera           ( Camera * camera );

    void                AllocateNewRenderTarget     ( Renderer * renderer );
    void                EnableTopRenderTarget       ( Renderer * renderer );
    void                DiscardCurrentRenderTarget  ( Renderer * renderer );
    void                DisableTopRenderTarget      ( Renderer * renderer );

    unsigned int        GetNumAllocatedRenderTargets() const;

    void                DrawTopAuxRenderTarget      ( Renderer * renderer, const IValue * alphaVal );
    void                DrawAMTopTwoRenderTargets   ( Renderer * renderer, const IValue * alphaVal );

    void                DrawDisplayRenderTarget     ( Renderer * renderer );
    void                SwapDisplayRenderTargets    ();

    unsigned int        TotalNumReadBuffers         () const;
    unsigned int        NumReadBuffersPerRT         () const;

    Texture2DConstPtr   ReadDisplayTarget           ( Renderer * renderer, unsigned int bufNum );

private:

    RenderTargetData    GetTopRenderTargetData          () const;
    RenderTargetData    GetRenderTargetDataAt           ( unsigned int i ) const;

    RenderTargetData    CreateDisplayRenderTargetData   () const;
    RenderTargetData    CreateAuxRenderTargetData       () const;

    RenderTargetData    CreateRenderTargetData          ( RenderTarget * rt, TriangleStrip * ts, Texture2DEffectPtr effTx, Texture2DEffectWithMaskPtr effTxMask ) const;

    void                SetDefaultTransform             ( TriangleStrip * ts ) const;

    unsigned int        CurDisplayRenderTargetNum       () const;
    RenderTargetData    CurDisplayRenderTargetData      () const;

};

} //bv
