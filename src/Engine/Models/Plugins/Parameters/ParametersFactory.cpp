#include "ParametersFactory.h"


namespace bv { namespace model {

// *******************************
//
ParamMat2                          ParametersFactory::CreateParameterMat2                 ( const std::string & name, const Vec4Interpolator & interpolator, ITimeEvaluatorPtr timeline )
{
    return ParamMat2( name, interpolator, timeline );
}

// *******************************
//
ParamVec3                          ParametersFactory::CreateParameter                      ( const std::string & name, const Vec3Interpolator & interpolator, ITimeEvaluatorPtr timeline )
{
    return ParamVec3( name, interpolator, timeline );
}

// *******************************
//
ParamVec4                          ParametersFactory::CreateParameter                      ( const std::string & name, const Vec4Interpolator & interpolator, ITimeEvaluatorPtr timeline )
{
    return ParamVec4( name, interpolator, timeline );
}

// *******************************
//
ParamFloat                          ParametersFactory::CreateParameter                     ( const std::string & name, const FloatInterpolator & interpolator, ITimeEvaluatorPtr timeline )
{
    return ParamFloat( name, interpolator, timeline );
}

// *******************************
//
ParamTransform                      ParametersFactory::CreateParameter                     ( const std::string & name, const TransformF & interpolator, ITimeEvaluatorPtr timeline )
{
    return ParamTransform( name, interpolator, timeline );
}

// *******************************
//
ParamTransformVec                   ParametersFactory::CreateParameter                     ( const std::string & name, const TransformF & interpolator, ITimeEvaluatorPtr timeline, int dummy )
{
    return ParamTransformVec( name, interpolator, timeline );
}

// *******************************
//
ParamMat2Ptr                         ParametersFactory::CreateParameterMat2                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamMat2 >( name, Vec4Interpolator(), timeline );
}

// *******************************
//
ParamVec3Ptr                         ParametersFactory::CreateParameterVec3                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamVec3 >( name, Vec3Interpolator(), timeline );
}

// *******************************
//
ParamVec4Ptr                         ParametersFactory::CreateParameterVec4                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamVec4 >( name, Vec4Interpolator(), timeline );
}

// *******************************
//
ParamFloatPtr                        ParametersFactory::CreateParameterFloat                ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamFloat >( name, FloatInterpolator(), timeline );
}

// *******************************
//
ParamTransformPtr                    ParametersFactory::CreateParameterTransform            ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamTransform >( name, TransformF(), timeline );
}

// *******************************
//
ParamTransformVecPtr                 ParametersFactory::CreateParameterTransformVec         ( const std::string & name, ITimeEvaluatorPtr timeline, int numTransforms )
{
    ParamTransformVecPtr ptv = std::make_shared< ParamTransformVec >( name, timeline );

    for( int i = 0; i < numTransforms; ++i )
    {
        ptv->AppendTransform( TransformF() );
    }

    return ptv;
}

} // model
} // bv
