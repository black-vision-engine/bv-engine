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

} //nrl
} //bv
