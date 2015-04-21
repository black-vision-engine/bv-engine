#include "SimpleTypedParameters.h"


namespace bv { namespace model {

// *******************************
//
VoidPtr ParamMat2::QueryParamTyped ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

} //model
} //bv
