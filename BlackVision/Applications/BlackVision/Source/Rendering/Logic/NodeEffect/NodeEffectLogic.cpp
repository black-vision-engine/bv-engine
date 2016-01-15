#include "NodeEffectLogic.h"

#include "Rendering/Utils/RenderLogicContext.h"


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
void    NodeEffectLogic::SetFullscreenEffect         ( FullscreenEffectTr * fse )
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
void    NodeEffectLogic::RecreateValues              ( std::vector< IValuePtr > & values )
{
    values.clear();


    //FIXME: implement
}

// *********************************
//
void    NodeEffectLogic::FSEInitializedGuard         ( RenderLogicContext * ctx, std::vector< RenderTarget * > * fseInputsVec, FullscreenEffectContext * fseCtx )
{
    if( m_FSE && !m_FSEInitialized )
    {
        //Initialize output vector
        for( unsigned int i = 0; i < fseInputsVec->size(); ++i )
        {
            (*fseInputsVec)[ i ] = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        }

        fseCtx->SetRenderer( renderer( ctx ) );
        fseCtx->SetRenderTargetAllocator( allocator( ctx ) );
        fseCtx->SetOutputRenderTarget( nullptr );
        fseCtx->SetInputRenderTargets( fseInputsVec );
        fseCtx->SetFirstRenderTargetIndex( 0 );

        m_FSEInitialized = true;
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
        ctx->SetOutputRenderTarget( output );
        m_FSE->Render( ctx );
    }
}

// *********************************
//
void    NodeEffectLogic::PostFSERenderLogic          ( SceneNode * node, RenderLogicContext * ctx ) const
{
    if( m_postFSELogic )
    {
        m_postFSELogic->Render( node, ctx );
    }
}

} //bv
