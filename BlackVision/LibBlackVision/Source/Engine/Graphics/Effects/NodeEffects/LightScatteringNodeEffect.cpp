//#include "stdafx.h"
//
//#include "LightScatteringNodeEffect.h"
//
//#include "Engine/Types/Values/ValuesFactory.h"
//
//
//namespace bv {
//
//// *********************************
////
//LightScatteringNodeEffect::LightScatteringNodeEffect     ()
//    : NodeEffect( NodeEffect::Type::T_LIGHT_SCATTERING )
//{
//    auto & values = AccessValues();
//
//    values.push_back( ValuesFactory::CreateValueFloat( "exposure" ) );
//    values.push_back( ValuesFactory::CreateValueFloat( "weight" ) );
//    values.push_back( ValuesFactory::CreateValueFloat( "decay" ) );
//    values.push_back( ValuesFactory::CreateValueFloat( "density" ) );
//    values.push_back( ValuesFactory::CreateValueVec2( "lightPositionOnScreen" ) );
//    values.push_back( ValuesFactory::CreateValueFloat( "numSamples" ) );
//}
//
//// *********************************
////
//LightScatteringNodeEffect::~LightScatteringNodeEffect    ()
//{
//}
//
//} // bv
