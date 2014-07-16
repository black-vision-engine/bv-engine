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

    RenderTarget *      m_displayRenderTargets[ 2 ];
    RenderTarget *      m_auxRenderTarget;
    
    TriangleStrip *     m_displayQuads[ 2 ];
    TriangleStrip *     m_auxQuad;

    unsigned int        m_curDisplayTarget;

    Texture2DEffect *   m_auxTexture2DEffect;

    std::vector< Texture2DPtr > m_readbackTextures;

    Camera *            m_displayCamera;
    Camera *            m_rendererCamera;

    bool                m_displayRTEnabled;
    bool                m_auxRTEnabled;

public:

                        OffscreenRenderLogic        ( unsigned int width, unsigned int height, unsigned int numReadBuffers, Camera * camera = nullptr, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~OffscreenRenderLogic       ();

    void                SetRendererCamera           ( Camera * camera );

    void                EnableDisplayRenderTarget   ( Renderer * renderer );
    void                EnableAuxRenderTarget       ( Renderer * renderer );

    void                DisableDisplayRenderTarget  ( Renderer * renderer );
    void                DisableAuxRenderTarget      ( Renderer * renderer );

    void                SetAuxAlphaModelValue       ( const IValue * val );

    void                SwapDisplayRenderTargets    ();

    bool                DisplayRenderTargetEnabled  () const;
    bool                AuxRenderTargetEnabled      () const;

    void                DrawDisplayRenderTarget     ( Renderer * renderer );
    void                DrawAuxRenderTarget         ( Renderer * renderer );

    unsigned int        NumReadBuffers              () const;

    Texture2DConstPtr   ReadDisplayTarget           ( Renderer * renderer, unsigned int bufNum );

private:

    RenderTarget *      CurDisplayRenderTarget      () const;
    TriangleStrip *     CurDisplayQuad              () const;
};

} //bv
