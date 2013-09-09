#pragma once

#include "BasicInterpolator.h"
#include <glm/glm.hpp>

namespace bv
{
    typedef bv::BasicInterpolator<float, float>     FloatInterpolator;
    typedef bv::BasicInterpolator<float, glm::vec3> Vec3Interpolator;
    typedef bv::BasicInterpolator<float, glm::vec4> Vec4Interpolator;

    typedef bv::Interpolator<float> BaseInterpolator;
}