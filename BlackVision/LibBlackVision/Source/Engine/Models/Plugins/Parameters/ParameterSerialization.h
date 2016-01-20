#pragma once

#include "Engine/Types/Enums.h"
#include "Serialization/SerializationHelper.h"


namespace bv
{
namespace SerializationHelper
{

template<>
std::string         T2String< bv::ModelParamType>       ( const bv::ModelParamType& t );



template<>
std::string         T2String< bv::ParamType>            ( const bv::ParamType& t );

template<>
bv::ParamType         String2T< bv::ParamType>            ( const std::string& s, const bv::ParamType& defaultVal );

} // SerializationHelper
} // bv
