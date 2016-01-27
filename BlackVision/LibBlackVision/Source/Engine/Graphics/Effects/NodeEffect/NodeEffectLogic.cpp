#include "NodeEffectLogic.h"

#include <set>

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
NodeEffectLogic::NodeEffectLogic                    ()
    : m_FSEInitialized( false )
    , m_FSECtx( nullptr, nullptr, nullptr, 0 )
    , m_preFSELogic( nullptr )
    , m_FSE( nullptr )
    , m_postFSELogic( nullptr )
{
}

// *********************************
//
NodeEffectLogic::~NodeEffectLogic                   ()
{
    delete m_preFSELogic;
    delete m_FSE;
    delete m_postFSELogic;
}

// *********************************
//
void    NodeEffectLogic::Render                      ( SceneNode * node, RenderLogicContext * ctx )
{
    auto fseCtx     = &m_FSECtx;
    auto fseRtVec   = &m_renderTargetsFSE;

    auto mainTarget = ctx->GetBoundRenderTarget();
    assert( mainTarget != nullptr );

    FSEInitializedGuard( ctx, fseRtVec, fseCtx );

    PreFSERenderLogic( node, ctx, m_renderTargetsFSE );
    assert( mainTarget == ctx->GetBoundRenderTarget() );

    FSERenderLogic( mainTarget, fseCtx );

    PostFSERenderLogic( node, ctx );
}

// *********************************
//
void    NodeEffectLogic::SetPreFullscreenEffectLogic ( PreFullscreenEffectLogic * logic )
{
    delete m_preFSELogic;

    m_preFSELogic = logic;

    RecreateValues( m_values );
}

// *********************************
//
void    NodeEffectLogic::SetPostFullscreenEffectLogic( PostFullscreenEffectLogic * logic )
{
    delete m_postFSELogic;

    m_postFSELogic = logic;

    RecreateValues( m_values );
}

// *********************************
//
void    NodeEffectLogic::SetFullscreenEffect         ( FullscreenEffect * fse )
{
    delete m_FSE;

    m_FSE = fse;

    m_renderTargetsFSE.clear();

    if( fse )
    {
        m_renderTargetsFSE.resize( fse->GetNumInputs() );

        for( unsigned int i = 0; i < fse->GetNumInputs(); ++i )
        {
            m_renderTargetsFSE[ i ] = nullptr;
        }
    }

    RecreateValues( m_values );
}

// *********************************
//
unsigned int    NodeEffectLogic::GetNumValues                () const
{
    return (unsigned int ) m_values.size();
}

// *********************************
//
IValuePtr       NodeEffectLogic::GetValueAt                  ( unsigned int i ) const
{
    assert( i < m_values.size() );

    return m_values[ i ];
}

// *********************************
//
IValuePtr       NodeEffectLogic::GetValue                    ( const std::string & name ) const
{
    for( auto val : m_values )
    {
        if( val->GetName() == name )
        {
            return val;
        }
    }

    return nullptr;
}

// *********************************
//
void    NodeEffectLogic::RecreateValues              ( std::vector< IValuePtr > & values )
{
    //FIXME: make sure that there are no duplicate values
    std::set< IValuePtr > uniqueValues;

    values.clear();

    if( m_preFSELogic )
    {
        auto second = m_preFSELogic->GetValues();

        uniqueValues.insert( second.begin(), second.end() );
    }

    if( m_FSE )
    {
        auto second = m_FSE->GetValues();

        uniqueValues.insert( second.begin(), second.end() );
    }

    if( m_postFSELogic )
    {
        auto second = m_postFSELogic->GetValues();

        uniqueValues.insert( second.begin(), second.end() );
    }

    values.insert( values.begin(), uniqueValues.begin(), uniqueValues.end() );
}

// *********************************
//
void    NodeEffectLogic::FSEInitializedGuard         ( RenderLogicContext * ctx, std::vector< RenderTarget * > * fseInputsVec, FullscreenEffectContext * fseCtx )
{
    if( m_FSE )
    {
        assert( m_preFSELogic );
        assert( m_preFSELogic->GetPreferredNumOutputs() == fseInputsVec->size() );

        auto changed = m_preFSELogic->AllocateOutputRenderTargets( ctx, fseInputsVec );

        fseCtx->SetSyncRequired( changed );

        if( !m_FSEInitialized )
        {
            fseCtx->SetRenderer( renderer( ctx ) );
            fseCtx->SetRenderTargetAllocator( allocator( ctx ) );
            fseCtx->SetOutputRenderTarget( nullptr );
            fseCtx->SetInputRenderTargets( fseInputsVec );
            fseCtx->SetFirstRenderTargetIndex( 0 );

            m_FSEInitialized = true;
        }
    }
}

// *********************************
//
void    NodeEffectLogic::PreFSERenderLogic          ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) const
{
    if( m_preFSELogic )
    {
        m_preFSELogic->Render( node, ctx, outputs );
    }
}

// *********************************
//
void    NodeEffectLogic::FSERenderLogic             ( RenderTarget * output, FullscreenEffectContext * ctx )
{
    if( m_FSE )
    {
        bool applicable = ( m_preFSELogic && m_preFSELogic->IsFSERequired() ) || !m_preFSELogic;

        if( applicable )
        {
            ctx->SetOutputRenderTarget( output );

            // FIXME: by default we render to the currently bound render target
            m_FSE->Render( ctx );

            ctx->SetSyncRequired( false );
        }
    }
}

// *********************************
//
void    NodeEffectLogic::PostFSERenderLogic          ( SceneNode * node, RenderLogicContext * ctx ) const
{
    if( m_preFSELogic )
    {
        m_preFSELogic->FreeOutputRenderTargets( ctx, &m_renderTargetsFSE );
    }

    if( m_postFSELogic )
    {
        m_postFSELogic->Render( node, ctx );
    }
}

} //bv