#include "OverwriteAlphaFullscreenEffect.h"

#include "Rendering/Utils/FullscreenUtils.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include <cassert>


namespace bv {

// **************************
//
OverwriteAlphaFullscreenEffect::OverwriteAlphaFullscreenEffect              ( float alpha )
    : m_alpha( alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );
}

// **************************
//
OverwriteAlphaFullscreenEffect::~OverwriteAlphaFullscreenEffect             ()
{
}

// **************************
//
void    OverwriteAlphaFullscreenEffect::SetAlpha                            ( float alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );

    m_alpha = alpha;
}

// **************************
//
float   OverwriteAlphaFullscreenEffect::GetAlpha                            () const
{
    return m_alpha;
}

// **************************
//
RenderableEntity *      OverwriteAlphaFullscreenEffect::CreateFullscreenQuad    ( Renderer * renderer ) const
{
    auto effect = CreateRenderableEffect( renderer );

    RenderableEntity * renderable = FullscreenUtils::CreateFullscreenQuad( effect, 1 );

    return renderable;
}

// **************************
//
RenderableEffectPtr     OverwriteAlphaFullscreenEffect::CreateRenderableEffect  ( Renderer * renderer ) const
{
    // Create All shaders
    { renderer; }

    return nullptr;
}

} //bv
