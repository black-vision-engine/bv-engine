#include "ShadowNodeEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// *********************************
//
ShadowNodeEffect::ShadowNodeEffect     ()
    : NodeEffect( NodeEffect::Type::T_SHADOW )
{
    auto & values = AccessValues();

    auto valueColor = ValuesFactory::CreateValueVec4( "color" );
    auto valueShift = ValuesFactory::CreateValueVec2( "shift" );
    auto valueBlurSize = ValuesFactory::CreateValueFloat( "blurSize" );
    auto valueNormalize = ValuesFactory::CreateValueFloat( "normalize" );

    values.push_back( valueColor );
    values.push_back( valueShift );
    values.push_back( valueBlurSize );
    values.push_back( valueNormalize );
}

// *********************************
//
ShadowNodeEffect::~ShadowNodeEffect    ()
{
}

} // bv
