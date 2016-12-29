#include "stdafx.h"

#include "NFullscreenEffectRendererState.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffectRendererState::NFullscreenEffectRendererState  ()
{
    SetBlendFlag( true );
    SetDepthTestFlag( false );
    SetCullFlag( false );
}

// **************************
//
bool    NFullscreenEffectRendererState::GetBlendFlag                    () const
{
    return m_blendFlag;
}

// **************************
//
bool    NFullscreenEffectRendererState::GetDepthTestFlag                () const
{
    return m_depthTestFlag;
}

// **************************
//
bool    NFullscreenEffectRendererState::GetCullFlag                     () const
{
    return m_cullFlag;
}

// **************************
//
void    NFullscreenEffectRendererState::SetBlendFlag                    ( bool flag )
{
    m_blendFlag = flag;
}

// **************************
//
void    NFullscreenEffectRendererState::SetDepthTestFlag                ( bool flag )
{
    m_depthTestFlag = flag;
}

// **************************
//
void    NFullscreenEffectRendererState::SetCullFlag                     ( bool flag )
{
    m_cullFlag = flag;
}


} //nrl
} //bv
