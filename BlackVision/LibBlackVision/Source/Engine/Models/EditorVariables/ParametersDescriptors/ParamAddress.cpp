#include "stdafx.h"
#include "ParamAddress.h"




namespace bv
{

// ***********************
//
void                    ParameterAddress::Serialize       ( ISerializer & /*ser*/ ) const
{

}

// ***********************
//
ParameterAddress        ParameterAddress::Create          ( const IDeserializer & /*deser*/ )
{
    return ParameterAddress();
}

}	// bv