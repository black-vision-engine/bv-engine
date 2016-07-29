#include "stdafx.h"

#include "ShadowNodeEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"




#include "Memory/MemoryLeaks.h"



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
    auto valueNormalize = ValuesFactory::CreateValueInt( "normalize" );
    auto valueInner = ValuesFactory::CreateValueInt( "inner" );

    values.push_back( valueColor );
    values.push_back( valueShift );
    values.push_back( valueBlurSize );
    values.push_back( valueNormalize );
    values.push_back( valueInner );
}

// *********************************
//
ShadowNodeEffect::~ShadowNodeEffect    ()
{
}

} // bv
