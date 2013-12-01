#pragma once

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/BasicInterpolator.h"
#include "System/BasicTypes.h"


namespace bv
{
    typedef bv::BasicInterpolator<TimeType, TimeType>   TimeInterpolator;

    typedef bv::BasicInterpolator<TimeType, float>      FloatInterpolator;
    typedef bv::BasicInterpolator<TimeType, glm::vec3>  Vec3Interpolator;
    typedef bv::BasicInterpolator<TimeType, glm::vec4>  Vec4Interpolator;

    typedef bv::Interpolator<float>                     BaseInterpolator;


class InterpolatorsHelper
{
public:

    static FloatInterpolator        CreateConstValue                    ( float val );
    static Vec4Interpolator         CreateConstValue                    ( const glm::vec4 & val );
    static Vec3Interpolator         CreateConstValue                    ( const glm::vec3 & val );
};

} //bv
