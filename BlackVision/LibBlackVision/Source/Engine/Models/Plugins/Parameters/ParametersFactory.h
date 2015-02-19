#pragma once

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

class ParametersFactory
{
public:

    static ParamMat2            CreateParameterMat2                 ( const std::string & name, const Vec4Interpolator & interpolator, ITimeEvaluatorPtr timeline );
    static ParamVec2            CreateParameter                     ( const std::string & name, const Vec2Interpolator & interpolator, ITimeEvaluatorPtr timeline );
    static ParamVec3            CreateParameter                     ( const std::string & name, const Vec3Interpolator & interpolator, ITimeEvaluatorPtr timeline );
    static ParamVec4            CreateParameter                     ( const std::string & name, const Vec4Interpolator & interpolator, ITimeEvaluatorPtr timeline );
    static ParamFloat           CreateParameter                     ( const std::string & name, const FloatInterpolator & interpolator, ITimeEvaluatorPtr timeline );
    static ParamTransform       CreateParameter                     ( const std::string & name, const TransformF & interpolator, ITimeEvaluatorPtr timeline );
    static ParamTransformVec    CreateParameter                     ( const std::string & name, const TransformF & interpolator, ITimeEvaluatorPtr timeline, int dummy );

    static ParamMat2Ptr         CreateParameterMat2                 ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamVec2Ptr         CreateParameterVec2                 ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamVec3Ptr         CreateParameterVec3                 ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamVec4Ptr         CreateParameterVec4                 ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamBoolPtr         CreateParameterBool                 ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamIntPtr          CreateParameterInt                  ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamFloatPtr        CreateParameterFloat                ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamTransformPtr    CreateParameterTransform            ( const std::string & name, ITimeEvaluatorPtr timeline );
    static ParamTransformVecPtr CreateParameterTransformVec         ( const std::string & name, ITimeEvaluatorPtr timeline, int numTransforms = 1 );

};

} // model
} // bv
