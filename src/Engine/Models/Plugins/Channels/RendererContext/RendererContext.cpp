#include "RendererContext.h"

namespace bv { namespace model {

// *****************************
//
RendererContext::RendererContext     ()
    : m_stateChanged( false )
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
}

// *****************************
//
bool    RendererContext::StateChanged       () const
{
    return m_stateChanged;
}

// *****************************
//
void    RendererContext::SetStateChanged    ( bool stateChanged ) const
{
    m_stateChanged = stateChanged;
}

} //model
} //bv
