#pragma once

#include "SerializationHelper.h"
#include "ConversionHelper.h"



namespace bv {
namespace SerializationHelper
{


// ***********************
//
template< typename T >
void                SerializeAttribute      ( ISerializer& ser, const T& t, std::string name )
{
    ser.SetAttribute( name, ::bv::Convert::T2String( t ) );
}

}   // SerializationHelper
}   // bv
