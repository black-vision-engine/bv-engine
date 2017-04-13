#include "stdafx.h"

#include "ParameterBinding.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"


namespace bv { namespace SerializationHelper {


std::pair< bv::nodelogic::ParameterBinding::VectorComponent, const char* > ParameterBindingVectorComponentMapping[] = 
{   std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::X, "X" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::Y, "Y" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::Z, "Z" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::W, "W" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::Invalid, "" )      // default
};

template<> bv::nodelogic::ParameterBinding::VectorComponent     String2T        ( const std::string & s, const bv::nodelogic::ParameterBinding::VectorComponent & defaultVal )    { return String2Enum( ParameterBindingVectorComponentMapping, s, defaultVal ); }
template<> std::string                                          T2String        ( const bv::nodelogic::ParameterBinding::VectorComponent & t )                                    { return Enum2String( ParameterBindingVectorComponentMapping, t ); }
    
}   // SerializationHelper
}   // bv
