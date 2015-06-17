#include "SimpleTypedParameters.h"


namespace bv { namespace model {

std::string Type2String( ModelParamType type )
{
    if( type == ModelParamType::MPT_FLOAT )
        return "float";

    //assert( false );
    return std::to_string( int ( type ) );
}

// *******************************
//
VoidPtr ParamMat2::QueryParamTyped ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

} //model
} //bv
