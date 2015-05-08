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

struct EffectRenderData
{
    TriangleStrip *             auxQuad;

	RenderableEffectPtr			effect;

            EffectRenderData( const RenderableEffectPtr & e );
};

typedef std::vector< RenderTarget * >  RenderTargetVec;

}

class EffectRenderLogic
{
private:

    TextureData         m_textureData;
    EffectRenderData	m_renderData;

    RenderTarget *		m_renderTarget;
    RenderTargetData    m_renderTargetData;

    Texture2DPtr		m_readbackTexture;

    Camera *            m_Camera;

    bool                m_displayRTEnabled;

public:

                        EffectRenderLogic        ( unsigned int width, unsigned int height, const RenderableEffectPtr & effect, Camera * camera = nullptr, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~EffectRenderLogic       ();

    void                SetCamera					( Camera * camera );

    void                Draw						( Renderer * renderer );

    Texture2DConstPtr   ReadTarget					( Renderer * renderer );

private:

    RenderTargetData    CreateRenderTargetData			() const;
};

} //bv
