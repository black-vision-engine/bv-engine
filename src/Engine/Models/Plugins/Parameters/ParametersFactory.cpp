#include "ParametersFactory.h"
#include "Engine/Models/Timeline/Timeline.h"

namespace bv { namespace model {

// *******************************
//
ParamVec3                          ParametersFactory::CreateParameter                      ( const std::string & name, const Vec3Interpolator & interpolator, const Timeline * timeline )
{
    return ParamVec3( name, interpolator, timeline );
}

// *******************************
//
ParamVec4                          ParametersFactory::CreateParameter                      ( const std::string & name, const Vec4Interpolator & interpolator, const Timeline * timeline )
{
    return ParamVec4( name, interpolator, timeline );
}

// *******************************
//
ParamFloat                          ParametersFactory::CreateParameter                     ( const std::string & name, const FloatInterpolator & interpolator, const Timeline * timeline )
{
    return ParamFloat( name, interpolator, timeline );
}

// *******************************
//
ParamTransform                      ParametersFactory::CreateParameter                    ( const std::string & name, const TransformF & interpolator, const Timeline * timeline )
{
    return ParamTransform( name, interpolator, timeline );
}

} // model
} // bv