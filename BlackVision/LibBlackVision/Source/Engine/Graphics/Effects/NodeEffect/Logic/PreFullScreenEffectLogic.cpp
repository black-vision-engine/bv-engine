#include "stdafx.h"

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
void    PreFullscreenEffectLogic::Render                        ( SceneNode * node, RenderLogicContext * ctx )
{
    RenderImpl( node, ctx, m_outputRenderTargets );
}

// *********************************
//
bool    PreFullscreenEffectLogic::AllocateOutputRenderTargets   ( RenderLogicContext * ctx )
{
    auto changed = false;

    auto numOutputs = GetPreferredNumOutputs();

    if( numOutputs != (unsigned int) m_outputRenderTargets.size() )
    {
        m_outputRenderTargets.resize( numOutputs );
    }

    if( numOutputs > 0 )
    {
        auto alc = allocator( ctx );

        //FIXME: optimize (if necessary) by simply incrementing allocator inner index instead of calling full Allocate method
        //Always allocate
        for( unsigned int i = 0; i < numOutputs; ++i )
        {
            m_outputRenderTargets[ i ] = alc->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        }

        changed = alc->GetTopIndex() != m_lastUsedRenderTargetNum;

        m_lastUsedRenderTargetNum = alc->GetTopIndex();
    }

    return changed;
}

// *********************************
//
void    PreFullscreenEffectLogic::FreeOutputRenderTargets   ( RenderLogicContext * ctx )
{
    if( m_outputRenderTargets.size() > 0 )
    {
        auto alc = allocator( ctx );
        
        // Always free
        assert( m_lastUsedRenderTargetNum == alc->GetTopIndex() );

        alc->Free( (unsigned int) m_outputRenderTargets.size() );
    }
}

// *********************************
//
const std::vector< RenderTarget * > &   PreFullscreenEffectLogic::GetOutputRenderTargets    () const
{
    return m_outputRenderTargets;
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
