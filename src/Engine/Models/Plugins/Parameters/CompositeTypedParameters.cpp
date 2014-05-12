#include "CompositeTypedParameters.h"

namespace bv { namespace model {

// ******************************************************************************************

// *******************************
//
ParamTransform::ParamTransform  ( const std::string & name, const TransformF & transform, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, ModelParamType::MPT_TRANSFORM, evaluator )
    , m_transformModel( transform )
{
}

// *******************************
//
void *     ParamTransform::QueryParamTyped  ()
{
    return static_cast< void * >( this );
}


// ******************************************************************************************

// *******************************
//
ParamTransformVec::ParamTransformVec                ( const std::string & name, const TransformF & transform, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, ModelParamType::MPT_TRANSFORM_VEC, evaluator )
{
    AppendTransform( transform );
}

// *******************************
//
ParamTransformVec::ParamTransformVec                ( const std::string & name, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, ModelParamType::MPT_TRANSFORM_VEC, evaluator )
{
}

// *******************************
//
void    ParamTransformVec::AppendTransform          ( const TransformF & transform )
{
    m_transformModelVec.push_back( transform );
}

// *******************************
//
void *          ParamTransformVec::QueryParamTyped  ()
{
    return static_cast< void * >( this );
}

} //model
} //bv
