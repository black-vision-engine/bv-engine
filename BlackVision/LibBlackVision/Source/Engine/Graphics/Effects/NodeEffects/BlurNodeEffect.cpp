#include "BlurNodeEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// *********************************
//
BlurNodeEffect::BlurNodeEffect     ()
    : NodeEffect( NodeEffect::Type::T_BLUR )
{
    auto & values = AccessValues();

    auto valueBlurSize = ValuesFactory::CreateValueFloat( "blurSize" );

    valueBlurSize->SetValue( 0.f );

    values.push_back( valueBlurSize );
}

// *********************************
//
BlurNodeEffect::~BlurNodeEffect    ()
{
}

} // bv
