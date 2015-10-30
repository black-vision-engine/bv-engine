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
    //auto value = ValuesFactory::CreateValueFloat( "lineWidth" );
    //auto value = ValuesFactory::CreateValueVec4( "lineColor" );
    //auto value = ValuesFactory::CreateValueVec4( "mainColor" );
    //value->SetValue( 1.f );
    //values.push_back( value );
}

// *********************************
//
WireframeNodeEffect::~WireframeNodeEffect    ()
{
}

} // bv
