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
bool    PreFullscreenEffectLogic::AllocateOutputRenderTargets   ( RenderLogicContext * ctx, std::vector< RenderTarget * > * outputRtVec )
{
    auto changed = false;

    if ( outputRtVec )
    {
        auto alc = allocator( ctx );

        //FIXME: optimize (if necessary) by simply incrementing allocator inner index instead of calling full Allocate method
        // Always allocate
        for( unsigned int i = 0; i < outputRtVec->size(); ++i )
        {
            (*outputRtVec)[ i ] = alc->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        }

        changed = alc->GetTopIndex() != m_lastUsedRenderTargetNum;

        m_lastUsedRenderTargetNum = alc->GetTopIndex();
    }

    return changed;
}

// *********************************
//
void    PreFullscreenEffectLogic::FreeOutputRenderTargets   ( RenderLogicContext * ctx, const std::vector< RenderTarget * > * outputRtVec )
{
    if( outputRtVec && outputRtVec->size() > 0 )
    {
        auto alc = allocator( ctx );
        // Always free
        assert( m_lastUsedRenderTargetNum == alc->GetTopIndex() );

        alc->Free( (unsigned int) outputRtVec->size() );
    }
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
