#pragma once

#include "Mathematics/glm_inc.h"

#include "Mathematics/Interpolators/BasicInterpolator.h"
#include "Mathematics/Interpolators/CompositeBezierInterpolator.h"
#include "CoreDEF.h"


namespace bv
{
    typedef CompositeBezierInterpolator<TimeType, TimeType>   TimeInterpolator;

    //typedef CompositeBezierInterpolator<TimeType, bool>       BoolInterpolator;
    typedef CompositeBezierInterpolator<TimeType, int>        IntInterpolator;
    
    typedef CompositeBezierInterpolator<TimeType, float>      FloatInterpolator;
    
    typedef CompositeBezierInterpolator<TimeType, glm::vec2>  Vec2Interpolator;
    typedef CompositeBezierInterpolator<TimeType, glm::vec3>  Vec3Interpolator;
    typedef CompositeBezierInterpolator<TimeType, glm::vec4>  Vec4Interpolator;


    //typedef BasicInterpolator<TimeType, TimeType>   TimeInterpolator;

    typedef BasicInterpolator<TimeType, bool>       BoolInterpolator;
    //typedef BasicInterpolator<TimeType, int>        IntInterpolator;
    //
    //typedef BasicInterpolator<TimeType, float>      FloatInterpolator;
    //
    //typedef BasicInterpolator<TimeType, glm::vec2>  Vec2Interpolator;
    //typedef BasicInterpolator<TimeType, glm::vec3>  Vec3Interpolator;
    //typedef BasicInterpolator<TimeType, glm::vec4>  Vec4Interpolator;

//    typedef Interpolator<float>                     BaseInterpolator;


class InterpolatorsHelper
{
public:

    static FloatInterpolator        CreateConstValue                    ( float val );
    static Vec4Interpolator         CreateConstValue                    ( const glm::vec4 & val );
    static Vec3Interpolator         CreateConstValue                    ( const glm::vec3 & val );
};

} //bv
