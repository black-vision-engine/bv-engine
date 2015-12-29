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

    valueColor->SetValue( glm::vec4( 1.f, 1.f, 1.f, 1.f ) );
    valueShift->SetValue( glm::vec2( 1.f, 1.f ) );
    valueBlurSize->SetValue( 0.01f );

    values.push_back( valueColor );
    values.push_back( valueShift );
    values.push_back( valueBlurSize );
}

// *********************************
//
ShadowNodeEffect::~ShadowNodeEffect    ()
{
}

} // bv
