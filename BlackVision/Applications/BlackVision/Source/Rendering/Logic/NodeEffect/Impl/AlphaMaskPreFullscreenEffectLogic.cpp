#include "AlphaMaskPreFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
AlphaMaskPreFullscreenEffectLogic::AlphaMaskPreFullscreenEffectLogic    ()
    : m_intRT( nullptr )
{
}

// *********************************
//
void    AlphaMaskPreFullscreenEffectLogic::Render                       ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs )
{
    { outputs;} 
    //FIXME: some better allocation patter for render targets (current pattern requires resetting effect textures every frame, where render target was changed).
    auto mainRT = disableBoundRT( ctx );

    //FIXME: make sure
    if( m_intRT == nullptr )
    {
        m_intRT = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
    }
    else
    {
        auto rt = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        assert( rt == m_intRT ); //FIXME this assumption is invalid
    }

    enable( ctx, m_intRT );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( node, ctx );

    disableBoundRT( ctx );
    allocator( ctx )->Free();

    enable( ctx, mainRT );
}

} //bv
