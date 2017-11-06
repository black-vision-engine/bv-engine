#include "stdafx.h"

#include "ParameterBinding.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"


namespace bv {
namespace Convert
{

// ***********************
//
std::pair< bv::nodelogic::ParameterBinding::VectorComponent, const char* > ParameterBindingVectorComponentMapping[] = 
{   std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::X, "X" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::Y, "Y" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::Z, "Z" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::W, "W" )
    , std::make_pair( bv::nodelogic::ParameterBinding::VectorComponent::Invalid, "" )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( bv::nodelogic::ParameterBinding::VectorComponent, ParameterBindingVectorComponentMapping )

    
}   // Convert
}   // bv
