#pragma once

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/BasicInterpolator.h"
#include "System/BasicTypes.h"


namespace bv
{
    typedef bv::BasicInterpolator<TimeType, TimeType>   TimeInterpolator;

    typedef bv::BasicInterpolator<float, float>         FloatInterpolator;
    typedef bv::BasicInterpolator<float, glm::vec3>     Vec3Interpolator;
    typedef bv::BasicInterpolator<float, glm::vec4>     Vec4Interpolator;

    typedef bv::Interpolator<float>                     BaseInterpolator;
}
