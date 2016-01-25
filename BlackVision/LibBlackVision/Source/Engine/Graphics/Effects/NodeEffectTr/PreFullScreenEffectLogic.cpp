#include "PreFullScreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
PreFullscreenEffectLogic::PreFullscreenEffectLogic      ()
    : m_lastUsedRenderTargetNum( 0 )
{
}

// *********************************
//
PreFullscreenEffectLogic::~PreFullscreenEffectLogic     ()
{
}

// *********************************
//
bool    PreFullscreenEffectLogic::UpdateOutputRenderTargets   ( RenderLogicContext * ctx, std::vector< RenderTarget * > * outputRtVec )
{
    bool changed = false;

    auto alc = allocator( ctx );

    if ( alc->GetTopIndex() != m_lastUsedRenderTargetNum )
    {

        for( unsigned int i = 0; i < outputRtVec->size(); ++i )
        {
            (*outputRtVec)[ i ] = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        }

        changed = true;
    }

    return changed;
}

// *********************************
//
void    PreFullscreenEffectLogic::RenderToRenderTarget  ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node )
{
    enable( ctx, rt );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->RenderNode( node, ctx ); 

    disableBoundRT( ctx );
}

} //bv
