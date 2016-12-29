#include "NRenderLogicState.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicState::NRenderLogicState   ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType )
    : m_initialized( false )
    , m_renderTargetAllocator( width, height )
    , m_renderResult( &m_renderTargetAllocator, numTrackedRenderTargetsPerOutputType )
{
    m_ctx.SetAllocator( &m_renderTargetAllocator );
}

// **************************
//
RenderResult *              NRenderLogicState::GetRenderResult     ()
{
    return &m_renderResult;
}

// **************************
//
OutputLogic *               NRenderLogicState::GetOutputLogic      ()
{
    return &m_outputLogic;
}

// **************************
//
NRenderContext *			NRenderLogicState::GetRenderContext		()
{
    return &m_ctx;
}

// **************************
//
bool                        NRenderLogicState::IsInitialized       () const
{
    return m_initialized;
}

// **************************
//
void                        NRenderLogicState::Initialize          ( Renderer * renderer )
{
    m_ctx.SetRenderer( renderer );

    m_initialized = true;
}

} // nrl
} // bv
