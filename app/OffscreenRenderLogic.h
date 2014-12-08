#pragma once

#include <vector>

#include "Engine/Types/Enums.h"
#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class RenderTarget;
class Renderer;
class Camera;
class TriangleStrip;
class Rednerer;
class Texture2DEffect;
class IValue;

class OffscreenRenderLogic
{
private:

    unsigned int        m_width;
    unsigned int        m_height;

    TextureFormat       m_fmt;

    int                 m_usedStackedRenderTargets;
    bool                m_topRenderTargetEnabled;

    std::vector<RenderTarget *>     m_auxRenderTargetVec;
    std::vector<TriangleStrip *>    m_auxQuadVec;

    RenderTarget *      m_displayRenderTargets[ 2 ];    
    TriangleStrip *     m_displayQuads[ 2 ];

    unsigned int        m_curDisplayTarget;
    unsigned int        m_buffersPerTarget;

    std::vector< Texture2DPtr > m_readbackTextures;

    Camera *            m_displayCamera;
    Camera *            m_rendererCamera;

    bool                m_displayRTEnabled;

public:

                        OffscreenRenderLogic        ( unsigned int width, unsigned int height, unsigned int numReadBuffers, Camera * camera = nullptr, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~OffscreenRenderLogic       ();

    void                AllocateNewRenderTarget     ( Renderer * renderer );
    void                EnableTopRenderTarget       ( Renderer * renderer );
    void                DiscardCurrentRenderTarget  ( Renderer * renderer );
    void                DisableTopRenderTarget      ( Renderer * renderer );

    void                DrawTopAuxRenderTarget      ( Renderer * renderer, const IValue * alphaVal );

private:

    RenderTarget *      GetTopRenderTarget          () const;
    RenderTarget *      GetPrevRenderTarget         () const;
    TriangleStrip *     GetTopRenderQuad            () const;

    RenderTarget *      CreateAuxRenderTarget       ( Renderer * renderer );
    TriangleStrip *     CreateAuxQuad               ( RenderTarget * rt );

public:

    void                SetRendererCamera           ( Camera * camera );

    void                SwapDisplayRenderTargets    ();

    void                DrawDisplayRenderTarget     ( Renderer * renderer );

    unsigned int        TotalNumReadBuffers         () const;
    unsigned int        NumReadBuffersPerRT         () const;

    Texture2DConstPtr   ReadDisplayTarget           ( Renderer * renderer, unsigned int bufNum );

private:

    unsigned int        CurDisplayRenderTargetNum   () const;

    RenderTarget *      CurDisplayRenderTarget      () const;
    TriangleStrip *     CurDisplayQuad              () const;
};

} //bv
