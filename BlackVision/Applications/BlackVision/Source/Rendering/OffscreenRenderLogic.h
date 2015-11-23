#pragma once

#include <vector>

#include "Engine/Types/Enums.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

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

namespace {

struct RenderTargetData
{
    RenderTarget *      renderTarget;
    TriangleStrip *     quad;

    RenderTargetData    ();
    ~RenderTargetData   ();
};

struct TextureData
{
    unsigned int        m_width;
    unsigned int        m_height;

    TextureFormat       m_fmt;

    TextureData         ( unsigned int width, unsigned int height, TextureFormat fmt );

};

struct OffscreenRenderData
{
    TriangleStrip *             auxQuad;

    Texture2DEffectPtr          effectTexture2D;
    Texture2DEffectWithMaskPtr  effectTexture2DWithMask;

            OffscreenRenderData();

    void    UseTexture2DEffect  ( const IValue * val, Texture2DPtr tex );
    void    UseTexture2DEffect  ( const IValue * val, Texture2DPtr tex, Texture2DPtr mask );
};

typedef std::vector< RenderTarget * >  RenderTargetVec;

}

class OffscreenRenderLogic
{
private:

    TextureData         m_textureData;
    OffscreenRenderData m_renderData;

    unsigned int        m_usedStackedRenderTargets;
    bool                m_topRenderTargetEnabled;

    RenderTargetVec     m_auxRenderTargets;
    RenderTargetData    m_displayRenderTargetData[ 2 ];
    RenderTargetData    m_videoOutputRenderTarget;

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

    void                DrawTopAuxRenderTarget      ( Renderer * renderer, const IValue * alphaVal );
    void                DrawAMTopTwoRenderTargets   ( Renderer * renderer, const IValue * alphaVal );

    void                DrawWithAllVideoEffects     ( Renderer * renderer );
    void                DrawDisplayRenderTarget     ( Renderer * renderer );
    void                SwapDisplayRenderTargets    ();

    unsigned int        TotalNumReadBuffers         () const;
    unsigned int        NumReadBuffersPerRT         () const;

    Texture2DConstPtr   ReadDisplayTarget           ( Renderer * renderer, unsigned int bufNum );
    Texture2DConstPtr   GetColorTextureAt           ( int i ) const;

	//pablito
	std::vector< int >  GetHackBuffersUids          ( Renderer * renderer ) const;

private:

    RenderTarget *      GetRenderTargetAt               ( int i ) const;

    RenderTargetData    CreateDisplayRenderTargetData       () const;
    RenderTargetData    CreateVideoOutputRenderTargetData   () const;

    unsigned int        CurDisplayRenderTargetNum           () const;
    RenderTargetData    CurDisplayRenderTargetData          () const;

};

} //bv
