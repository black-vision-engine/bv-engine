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
ParamVec2                          ParametersFactory::CreateParameter                      ( const std::string & name, const Vec2Interpolator & interpolator, ITimeEvaluatorPtr timeline )
{
    return ParamVec2( name, interpolator, timeline );
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

//// *******************************
////
//ParamTransformVec                   ParametersFactory::CreateParameter                     ( const std::string & name, const TransformF & interpolator, ITimeEvaluatorPtr timeline, int dummy )
//{
//    { dummy; } // FIXME: suppress unused variable
//    return ParamTransformVec( name, interpolator, timeline );
//}

// *******************************
//
ParamMat2Ptr                         ParametersFactory::CreateParameterMat2                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamMat2 >( name, Vec4Interpolator(), timeline );
}

// *******************************
//
ParamVec2Ptr                         ParametersFactory::CreateParameterVec2                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamVec2 >( name, Vec2Interpolator(), timeline );
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
ParamBoolPtr                        ParametersFactory::CreateParameterBool                  ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamBool >( name, BoolInterpolator(), timeline );
}

// *******************************
//
ParamIntPtr                        ParametersFactory::CreateParameterInt                  ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamInt >( name, IntInterpolator(), timeline );
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

//// *******************************
////
//ParamTransformVecPtr                 ParametersFactory::CreateParameterTransformVec         ( const std::string & name, ITimeEvaluatorPtr timeline, int numTransforms )
//{
//    ParamTransformVecPtr ptv = std::make_shared< ParamTransformVec >( name, timeline );
//
//    for( int i = 0; i < numTransforms; ++i )
//    {
//        ptv->AppendTransform( TransformF() );
//    }
//
//    return ptv;
//}

// *******************************
//
ParamStringPtr                      ParametersFactory::CreateParameterString               ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamString >( name, StringInterpolator(), timeline );
}

ParamWStringPtr                     ParametersFactory::CreateParameterWString              ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return std::make_shared< ParamWString >( name, WStringInterpolator(), timeline );
}


// *******************************
//

template<>
static ParamIntPtr          ParametersFactory::CreateTypedSimpleParameter< ParamInt > (const std::string& name, ITimeEvaluatorPtr te )
{
    return CreateParameterInt( name, te );
}

template<>
static ParamFloatPtr          ParametersFactory::CreateTypedSimpleParameter< ParamFloat > (const std::string& name, ITimeEvaluatorPtr te )
{
    return CreateParameterFloat( name, te );
}

template<>
static ParamBoolPtr          ParametersFactory::CreateTypedSimpleParameter< ParamBool > (const std::string& name, ITimeEvaluatorPtr te )
{
    return CreateParameterBool( name, te );
}

template<>
static ParamStringPtr        ParametersFactory::CreateTypedSimpleParameter< ParamString > (const std::string& name, ITimeEvaluatorPtr te )
{
    return CreateParameterString( name, te );
}

template<>
static ParamWStringPtr       ParametersFactory::CreateTypedSimpleParameter< ParamWString > (const std::string& name, ITimeEvaluatorPtr te )
{
    return CreateParameterWString( name, te );
}

} // model
} // bv
