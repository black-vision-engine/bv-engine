#pragma once

#include "Mathematics/glm_inc.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "CoreDEF.h"


namespace bv
{

typedef CompositeInterpolator<TimeType, TimeType>   TimeInterpolator;

typedef CompositeInterpolator<TimeType, bool>       BoolInterpolator;
typedef CompositeInterpolator<TimeType, int>        IntInterpolator;
    
typedef CompositeInterpolator<TimeType, float>      FloatInterpolator;
    
typedef CompositeInterpolator<TimeType, glm::vec2>  Vec2Interpolator;
typedef CompositeInterpolator<TimeType, glm::vec3>  Vec3Interpolator;
typedef CompositeInterpolator<TimeType, glm::vec4>  Vec4Interpolator;

//typedef BasicInterpolator<TimeType, std::string> StringInterpolator;
//typedef BasicInterpolator<TimeType, std::wstring> WStringInterpolator;
typedef CompositeInterpolator<TimeType, std::string> StringInterpolator;
typedef CompositeInterpolator<TimeType, std::wstring> WStringInterpolator;


class InterpolatorsHelper
{
public:

    static FloatInterpolator        CreateConstValue                    ( float val );
    static Vec4Interpolator         CreateConstValue                    ( const glm::vec4 & val );
    static Vec3Interpolator         CreateConstValue                    ( const glm::vec3 & val );
};

} //bv
