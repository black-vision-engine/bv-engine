#pragma once

#include "Engine/Models/Plugins/Parameters/TypedParameters.h"


namespace bv { namespace model {

class Timeline;

class ParametersFactory
{
public:

    static ParamVec3            CreateParameter                     ( const std::string & name, const Vec3Interpolator & interpolator, const Timeline * timeline = nullptr );
    static ParamVec4            CreateParameter                     ( const std::string & name, const Vec4Interpolator & interpolator, const Timeline * timeline = nullptr );
    static ParamFloat           CreateParameter                     ( const std::string & name, const FloatInterpolator & interpolator, const Timeline * timeline = nullptr );
    static ParamTransform       CreateParameter                     ( const std::string & name, const TransformF & interpolator, const Timeline * timeline = nullptr );

};

} // model
} // bv
