#include "OverwriteAlphaFullscreenEffect.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Rendering/Utils/FullscreenUtils.h"

#include "Rendering/Logic/FullScreen/Impl/FullscreenRenderableEffect.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"


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
RenderableEntity *      OverwriteAlphaFullscreenEffect::CreateFullscreenQuad    () const
{
    auto effect = CreateRenderableEffect();

    RenderableEntity * renderable = FullscreenUtils::CreateFullscreenQuad( effect, 1 );

    return renderable;
}

// **************************
//
RenderableEffectPtr     OverwriteAlphaFullscreenEffect::CreateRenderableEffect  () const
{
    auto vs = FullscreenRenderableEffect::CreateVS( 1 );
    auto ps = CreatePS();

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );

    auto sinst = pass->GetStateInstance();
    
    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = false;
    ds->enabled = false;
    cs->enabled = false;

    auto effect = std::make_shared< FullscreenRenderableEffect >( pass );

    return effect;
}

// **************************
//
PixelShader *       OverwriteAlphaFullscreenEffect::CreatePS                    () const
{
    //FIXME: implement
    return nullptr;
}

} //bv
