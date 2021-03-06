#include "stdafx.h"

#include "NodeEffect.h"

// FIXME: Delete after solving problems with IsBlendable_DIRTY_DESIGN_HACK and GetDepth_DIRTY_DESIGN_HACK
#include "NodeEffectImpl.h"


namespace bv {

// *********************************
//
NodeEffect::NodeEffect     ( NodeEffectType type )
    : m_type( type )
{
}

// *********************************
//
NodeEffect::~NodeEffect    ()
{
}

// *********************************
//
NodeEffectType     NodeEffect::GetType         () const
{
    return m_type;
}

} //bv
