#include "RendererStateInstance.h"


namespace bv {

// *********************************
//
RendererStateInstance::RendererStateInstance   ()
    : m_alphaState( nullptr )
    , m_cullState( nullptr )
    , m_depthState( nullptr )
    , m_fillState( nullptr )
    , m_offsetState( nullptr )
    , m_stencilState( nullptr )
{
}

// *********************************
//
RendererStateInstance::RendererStateInstance   ( AlphaState * as, CullState * cs, DepthState * ds, FillState * fs, OffsetState * os, StencilState * ss )
    : m_alphaState( as )
    , m_cullState( cs )
    , m_depthState( ds )
    , m_fillState( fs )
    , m_offsetState( os )
    , m_stencilState( ss )
{
}

// *********************************
//
RendererStateInstance::~RendererStateInstance  ()
{
    delete m_alphaState;
    delete m_cullState;
    delete m_depthState;
    delete m_fillState;
    delete m_offsetState;
    delete m_stencilState;
}

// *********************************
//
const AlphaState *      RendererStateInstance::GetAlphaState   () const
{
    return m_alphaState;
}

// *********************************
//
const CullState *       RendererStateInstance::GetCullState    () const
{
    return m_cullState;
}

// *********************************
//
const DepthState *      RendererStateInstance::GetDepthState   () const
{
    return m_depthState;
}

// *********************************
//
const FillState *       RendererStateInstance::GetFillState    () const
{
    return m_fillState;
}

// *********************************
//
const OffsetState *     RendererStateInstance::GetOffsetState  () const
{
    return m_offsetState;
}

// *********************************
//
const StencilState *    RendererStateInstance::GetStencilState () const
{
    return m_stencilState;
}

// *********************************
//
void                    RendererStateInstance::SetState( AlphaState * as )
{
    m_alphaState = as;
}

// *********************************
//
void                    RendererStateInstance::SetState( CullState * cs )
{
    m_cullState = cs;
}

// *********************************
//
void                    RendererStateInstance::SetState( DepthState * ds )
{
    m_depthState = ds;
}

// *********************************
//
void                    RendererStateInstance::SetState( FillState * fs )
{
    m_fillState = fs;
}

// *********************************
//
void                    RendererStateInstance::SetState( OffsetState * os )
{
    m_offsetState = os;
}

// *********************************
//
void                    RendererStateInstance::SetState( StencilState * ss )
{
    m_stencilState = ss;
}

// *********************************
//
void                    RendererStateInstance::SetStateIgnoreEmptyEntries( const RendererStateInstance & inst )
{
    if( inst.m_alphaState )
        m_alphaState = inst.m_alphaState;

    if( inst.m_cullState )
        m_cullState = inst.m_cullState;

    if( inst.m_depthState )
        m_depthState = inst.m_depthState;

    if( inst.m_fillState )
        m_fillState = inst.m_fillState;

    if( inst.m_offsetState )
        m_offsetState = inst.m_offsetState;

    if( inst.m_stencilState )
        m_stencilState = inst.m_stencilState;
}

// *********************************
//
void                    RendererStateInstance::DeleteStates    ()
{
    delete m_alphaState;
    delete m_cullState;
    delete m_depthState;
    delete m_fillState;
    delete m_offsetState;
    delete m_stencilState;
}


// *********************************
//
AlphaState *     RenderStateAccessor::AccessAlphaState( RendererStateInstance * inst )
{
    return inst->m_alphaState;
}

// *********************************
//
CullState *      RenderStateAccessor::AccessCullState( RendererStateInstance * inst )
{
    return inst->m_cullState;
}

// *********************************
//
DepthState *     RenderStateAccessor::AccessDepthState( RendererStateInstance * inst )
{
    return inst->m_depthState;
}

// *********************************
//
FillState *      RenderStateAccessor::AccessFillState( RendererStateInstance * inst )
{
    return inst->m_fillState;
}

// *********************************
//
OffsetState *    RenderStateAccessor::AccessOffsetState( RendererStateInstance * inst )
{
    return inst->m_offsetState;
}

// *********************************
//
StencilState *   RenderStateAccessor::AccessStencilState( RendererStateInstance * inst )
{
    return inst->m_stencilState;
}

} //bv
