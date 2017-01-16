#include "stdafx.h"

#include "NNodeEffect.h"

// FIXME: Delete after solving problems with IsBlendable_DIRTY_DESIGN_HACK and GetDepth_DIRTY_DESIGN_HACK
#include "NNodeEffectImpl.h"


namespace bv { namespace nrl  {

// *********************************
//
NNodeEffect::NNodeEffect     ( NNodeEffectType type )
    : m_type( type )
{
}

// *********************************
//
NNodeEffect::~NNodeEffect    ()
{
}

// *********************************
//
NNodeEffectType     NNodeEffect::GetType         () const
{
    return m_type;
}

} //nrl
} //bv
