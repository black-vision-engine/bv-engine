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

    valueBgIdx->SetValue( 0 );
    valueFgIdx->SetValue( 1 );

    values.push_back( valueBgIdx );
    values.push_back( valueFgIdx );
}

// *********************************
//
NodeMaskNodeEffect::~NodeMaskNodeEffect    ()
{
}

} // bv
