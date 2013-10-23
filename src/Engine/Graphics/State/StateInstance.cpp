#include "StateInstance.h"


namespace bv {

// *********************************
//
StateInstance::StateInstance   ()
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
StateInstance::StateInstance   ( const AlphaState * as, const CullState * cs, const DepthState * ds, const FillState * fs, const OffsetState * os, const StencilState * ss )
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
StateInstance::~StateInstance  ()
{
}

// *********************************
//
const AlphaState *      StateInstance::GetAlphaState   () const
{
    return m_alphaState;
}

// *********************************
//
const CullState *       StateInstance::GetCullState    () const
{
    return m_cullState;
}

// *********************************
//
const DepthState *      StateInstance::GetDepthState   () const
{
    return m_depthState;
}

// *********************************
//
const FillState *       StateInstance::GetFillState    () const
{
    return m_fillState;
}

// *********************************
//
const OffsetState *     StateInstance::GetOffsetState  () const
{
    return m_offsetState;
}

// *********************************
//
const StencilState *    StateInstance::GetStencilState () const
{
    return m_stencilState;
}

// *********************************
//
void                    StateInstance::SetState( const AlphaState * as )
{
    m_alphaState = as;
}

// *********************************
//
void                    StateInstance::SetState( const CullState * cs )
{
    m_cullState = cs;
}

// *********************************
//
void                    StateInstance::SetState( const DepthState * ds )
{
    m_depthState = ds;
}

// *********************************
//
void                    StateInstance::SetState( const FillState * fs )
{
    m_fillState = fs;
}

// *********************************
//
void                    StateInstance::SetState( const OffsetState * os )
{
    m_offsetState = os;
}

// *********************************
//
void                    StateInstance::SetState( const StencilState * ss )
{
    m_stencilState = ss;
}

// *********************************
//
void                    StateInstance::DeleteStates    ()
{
    delete m_alphaState;
    delete m_cullState;
    delete m_depthState;
    delete m_fillState;
    delete m_offsetState;
    delete m_stencilState;
}

} //bv
