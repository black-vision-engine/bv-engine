#include "WireframeNodeEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// *********************************
//
WireframeNodeEffect::WireframeNodeEffect     ()
    : NodeEffect( NodeEffect::Type::T_WIREFRAME )
{
    auto & values = AccessValues();

    { values; }
    // FIXME: string "alpha" used explicitely - suxx
    //auto value = ValuesFactory::CreateValueFloat( "alpha" );
    //value->SetValue( 1.f );
    //values.push_back( value );
}

// *********************************
//
WireframeNodeEffect::~WireframeNodeEffect    ()
{
}

} // bv
