#pragma once

#include "Engine/Models/Plugins/Interfaces/ICachedParameter.h"
#include "SimpleTypedParameters.h"

namespace bv { namespace model {

// *******************************************
template< typename InterpolatorType, typename ValueType, ModelParamType type >
class CachedSimpleTypedParameters : /*ICachedParameter,*/
                                    public SimpleTypedParameteres
{

};

} //model
} //bv
