#include "FullscreenEffectRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"


namespace bv {

namespace {

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer * CreateRectangleArrayData  ( unsigned int )
{
    return nullptr;
}

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer * CreateTriStripArrayData( unsigned int numVertices, float * vbData )
{
    { numVertices; vbData; }
    return nullptr;
}

} // anonymous

// *********************************
//
FullscreenEffectRenderLogic::FullscreenEffectRenderLogic ()
    : m_fullscreenQuad( nullptr )
{
}

// *********************************
//
FullscreenEffectRenderLogic::~FullscreenEffectRenderLogic()
{
    delete m_fullscreenQuad;
}

// *********************************
//
void        FullscreenEffectRenderLogic::Render  ( Renderer * renderer, FullscreenEffect * effect )
{
    { renderer; }
    { effect; }
    // Plus camera
//    renderer->Enable( effect->GetPass( 0 ), m_fullscreenQuad );
//    renderer->DrawRenderable( m_fullscreenQuad );
}

} // bv
