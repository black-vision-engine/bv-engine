#include "stdafx.h"

#include "FullscreenEffectRendererState.h"


namespace bv { 

// **************************
//
FullscreenEffectRendererState::FullscreenEffectRendererState  ()
{
    SetBlendFlag( true );
    SetDepthTestFlag( false );
    SetCullFlag( false );
}

// **************************
//
bool    FullscreenEffectRendererState::GetBlendFlag                    () const
{
    return m_blendFlag;
}

// **************************
//
bool    FullscreenEffectRendererState::GetDepthTestFlag                () const
{
    return m_depthTestFlag;
}

// **************************
//
bool    FullscreenEffectRendererState::GetCullFlag                     () const
{
    return m_cullFlag;
}

// **************************
//
void    FullscreenEffectRendererState::SetBlendFlag                    ( bool flag )
{
    m_blendFlag = flag;
}

// **************************
//
void    FullscreenEffectRendererState::SetDepthTestFlag                ( bool flag )
{
    m_depthTestFlag = flag;
}

// **************************
//
void    FullscreenEffectRendererState::SetCullFlag                     ( bool flag )
{
    m_cullFlag = flag;
}



} //bv
