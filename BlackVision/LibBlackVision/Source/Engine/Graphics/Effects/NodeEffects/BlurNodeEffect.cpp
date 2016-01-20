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
    auto valueNormalize = ValuesFactory::CreateValueInt( "normalize" );
    auto valueBlurKernelType = ValuesFactory::CreateValueInt( "blurKernelType" );

    values.push_back( valueBlurSize );
    values.push_back( valueNormalize );
    values.push_back( valueBlurKernelType );
}

// *********************************
//
BlurNodeEffect::~BlurNodeEffect    ()
{
}

} // bv
