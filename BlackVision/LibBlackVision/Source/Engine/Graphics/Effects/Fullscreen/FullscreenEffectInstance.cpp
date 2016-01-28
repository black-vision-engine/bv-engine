#include "FullscreenEffectInstance.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// **************************
//
FullscreenEffectInstance::FullscreenEffectInstance( FullscreenEffectPtr effect )
    : m_initialized( false )
    , m_ctx( nullptr, nullptr, nullptr, 0 )
    , m_effect( effect )
{
    assert( effect != nullptr );
}

// **************************
//
FullscreenEffectInstance::~FullscreenEffectInstance ()
{
}

// **************************
//
void        FullscreenEffectInstance::Render                ( RenderTarget * output, RenderLogicContext * ctx )
{
    InitializeGuard( ctx );

    RenderImpl( output, &m_ctx );
}

// **************************
//
void        FullscreenEffectInstance::UpdateInputRTState    ( const std::vector< RenderTarget * > & renderTargets )
{
    m_inputRenderTargets = renderTargets;

    m_ctx.SetSyncRequired( true );
}

// **************************
//
std::vector< IValuePtr >    FullscreenEffectInstance::GetValues () const
{
    return m_effect->GetValues();
}

// **************************
//
void                        FullscreenEffectInstance::InitializeGuard             ( RenderLogicContext * ctx )
{
    //FIXME: assumes that only one renderer is used
    if( !m_initialized )
    {
        m_ctx.SetRenderer( renderer( ctx ) );
        m_ctx.SetRenderTargetAllocator( allocator( ctx ) );
        m_ctx.SetOutputRenderTarget( nullptr );
        m_ctx.SetInputRenderTargets( &m_inputRenderTargets );
        m_ctx.SetFirstRenderTargetIndex( 0 );

        m_initialized = true;
    }
}

// **************************
//
void                        FullscreenEffectInstance::RenderImpl                  ( RenderTarget * output, FullscreenEffectContext * ctx )
{
    ctx->SetOutputRenderTarget( output );

    m_effect->Render( ctx );

    ctx->SetSyncRequired( false );
}

} //bv
