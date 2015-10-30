#include "NodeMaskNodeEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// *********************************
//
NodeMaskNodeEffect::NodeMaskNodeEffect     ()
    : NodeEffect( NodeEffect::Type::T_NODE_MASK )
{
    auto & values = AccessValues();

    auto valueBgIdx = ValuesFactory::CreateValueInt( "bgIdx" );
    auto valueFgIdx = ValuesFactory::CreateValueInt( "fgIdx" );
    auto valueAlpha = ValuesFactory::CreateValueFloat( "alpha" );

    valueBgIdx->SetValue( 0 );
    valueFgIdx->SetValue( 1 );
    valueAlpha->SetValue( 1.f );

    values.push_back( valueBgIdx );
    values.push_back( valueFgIdx );
    values.push_back( valueAlpha );
}

// *********************************
//
NodeMaskNodeEffect::~NodeMaskNodeEffect    ()
{
}

} // bv
