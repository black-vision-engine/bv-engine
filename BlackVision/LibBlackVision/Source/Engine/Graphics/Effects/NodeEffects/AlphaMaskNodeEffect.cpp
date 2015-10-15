#include "AlphaMaskNodeEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// *********************************
//
AlphaMaskNodeEffect::AlphaMaskNodeEffect     ()
    : NodeEffect( NodeEffect::Type::T_ALPHA_MASK )
{
    auto values = AccessValues();

    // FIXME: string "alpha" used explicitely - suxx
    auto value = ValuesFactory::CreateValueFloat( "alpha" );
    value->SetValue( 1.f );
    values.push_back( value );
}

// *********************************
//
AlphaMaskNodeEffect::~AlphaMaskNodeEffect    ()
{
}

} // bv
