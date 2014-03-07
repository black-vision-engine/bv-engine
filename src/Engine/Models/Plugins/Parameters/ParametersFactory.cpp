#include "ParametersFactory.h"

#include "Engine/Models/Timeline/Timeline.h"


namespace bv { namespace model {

// *******************************
//
ParamMat2                          ParametersFactory::CreateParameterMat2                 ( const std::string & name, const Vec4Interpolator & interpolator, const Timeline * timeline )
{
    return ParamMat2( name, interpolator, timeline );
}

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
ParamTransform                      ParametersFactory::CreateParameter                     ( const std::string & name, const TransformF & interpolator, const Timeline * timeline )
{
    return ParamTransform( name, interpolator, timeline );
}

// *******************************
//
ParamTransformVec                   ParametersFactory::CreateParameter                     ( const std::string & name, const TransformF & interpolator, const Timeline * timeline, int dummy )
{
    return ParamTransformVec( name, interpolator, timeline );
}

// *******************************
//
ParamMat2 *                         ParametersFactory::CreateParameterMat2                 ( const std::string & name )
{
    return new ParamMat2( name, Vec4Interpolator() );
}

// *******************************
//
ParamVec3 *                         ParametersFactory::CreateParameterVec3                 ( const std::string & name )
{
    return new ParamVec3( name, Vec3Interpolator() );
}

// *******************************
//
ParamVec4 *                         ParametersFactory::CreateParameterVec4                 ( const std::string & name )
{
    return new ParamVec4( name, Vec4Interpolator() );
}

// *******************************
//
ParamFloat *                        ParametersFactory::CreateParameterFloat                ( const std::string & name )
{
    return new ParamFloat( name, FloatInterpolator() );
}

// *******************************
//
ParamTransform *                    ParametersFactory::CreateParameterTransform            ( const std::string & name )
{
    return new ParamTransform( name, TransformF() );
}

// *******************************
//
ParamTransformVec *                 ParametersFactory::CreateParameterTransformVec         ( const std::string & name, int numTransforms )
{
    ParamTransformVec * ptv = new ParamTransformVec( name );

    for( int i = 0; i < numTransforms; ++i )
    {
        ptv->AppendTransform( TransformF() );
    }

    return ptv;
}

} // model
} // bv
