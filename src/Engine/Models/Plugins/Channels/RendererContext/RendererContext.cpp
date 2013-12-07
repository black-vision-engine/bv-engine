#include "RendererContext.h"


namespace bv { namespace model {

// *****************************
//
RendererContext::RendererContext     ()
    : m_stateChanged( true )
    , alphaCtx( nullptr )
    , cullCtx( nullptr )
    , depthCtx( nullptr )
    , fillCtx( nullptr )
{
}

// *****************************
//
RendererContext::~RendererContext    ()
{
    delete alphaCtx;
    delete cullCtx;
    delete depthCtx;
    delete fillCtx;
}

// *****************************
//
bool                RendererContext::StateChanged       () const
{
    return m_stateChanged;
}

// *****************************
//
void                RendererContext::SetStateChanged    ( bool stateChanged ) const
{
    m_stateChanged = stateChanged;
}

// *****************************
//
RendererContext*    RendererContext::Create       ()
{
    return new model::RendererContext();
}

// *****************************
//
RendererContext*    RendererContext::CreateDefault       ()
{
    auto ctx        = Create();
    ctx->alphaCtx   = new model::AlphaContext();
    ctx->cullCtx    = new model::CullContext();
    ctx->depthCtx   = new model::DepthContext();
    ctx->fillCtx    = new model::FillContext();

    return ctx;
}

} //model
} //bv
