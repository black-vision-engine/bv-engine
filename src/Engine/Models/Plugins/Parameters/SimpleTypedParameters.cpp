#include "SimpleTypedParameters.h"


namespace bv { namespace model {

// *******************************
//
void *  ParamMat2::QueryParamTyped ()
{
    return static_cast< void * >( this );
}


} //model
} //bv
