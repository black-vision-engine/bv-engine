#include "stdafx.h"

#include "FullscreenEffectInstance.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"
#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"
#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/InputFullscreenEffectGraphNode.h"

namespace bv {

// **************************
//
FullscreenEffectInstance::FullscreenEffectInstance( FullscreenEffectPtr effect )
    : m_initialized( false )
    , m_ctx( nullptr, nullptr, nullptr, 0 )
    , m_effect( effect )
    , m_inputRenderTargets( effect->GetNumInputs() )
{
    assert( effect != nullptr );

    for( unsigned int i = 0; i < effect->GetNumInputs(); ++i )
    {
        m_inputRenderTargets[ i ] = nullptr;
    }
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
void        FullscreenEffectInstance::UpdateInputRenderTargets  ( const std::vector< RenderTarget * > & renderTargets )
{
    assert( m_inputRenderTargets.size() == renderTargets.size() );

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
unsigned int                FullscreenEffectInstance::GetNumInputRenderTargets    () const
{
    return (unsigned int) m_inputRenderTargets.size();
}

// **************************
//
const RenderTarget *        FullscreenEffectInstance::GetRenderTarget             ( unsigned int i ) const
{
    assert( i < m_inputRenderTargets.size() );

    return m_inputRenderTargets[ i ];
}

// **************************
//
void                        FullscreenEffectInstance::SetRenderTarget             ( unsigned int i, RenderTarget * rt )
{
    assert( i < m_inputRenderTargets.size() );

    m_inputRenderTargets[ i ] = rt;
}

// **************************
//
bool                        FullscreenEffectInstance::SetValue                    ( const std::string & name, int value )
{
    for( auto val : GetValues() )
    {
        if ( val->GetName() == name )
        {
            auto intVal = QueryTypedValue< ValueIntPtr  >( val );
            intVal->SetValue( value );

            return true;
        }
    }

    return false;
}

// **************************
//
bool                        FullscreenEffectInstance::SetValue                    ( const std::string & name, float value )
{
    for( auto val : GetValues() )
    {
        if ( val->GetName() == name )
        {
            auto floatVal = QueryTypedValue< ValueFloatPtr  >( val );
            floatVal->SetValue( value );

            return true;
        }
    }

    return false;
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

// **************************
//
void                        FullscreenEffectInstance::Update                      ()
{
    m_effect->Update();
}

// **************************
//
void                        FullscreenEffectInstance::AddTexture                  ( const ITextureDescriptorConstPtr & txDesc )
{
    m_effect->AddTexture( txDesc );
}

} //bv
