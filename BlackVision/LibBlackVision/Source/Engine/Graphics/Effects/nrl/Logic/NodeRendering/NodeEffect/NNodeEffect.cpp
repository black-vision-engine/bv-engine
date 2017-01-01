#include "stdafx.h"

#include "NNodeEffect.h"


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

// *********************************
//
void                NNodeEffect::Update          ()
{
    //FIXME: implement
    assert( false );
}

} //nrl
} //bv
