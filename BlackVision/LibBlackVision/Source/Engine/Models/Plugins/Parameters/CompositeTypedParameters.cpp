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
// FIXME: reimplement with SQT paramter model
void                ParamTransform::SetCurveType    ( CurveType type ) 
{ 
    m_transformModel.SetCurveType( type ); 
}

// *******************************
//
VoidPtr    ParamTransform::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
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
void ParamTransformVec::SetCurveType        ( CurveType type ) 
{ 
    for( auto& trans : m_transformModelVec ) 
        trans.SetCurveType( type ); 
}

// *******************************
//
void    ParamTransformVec::AppendTransform          ( const TransformF & transform )
{
    m_transformModelVec.push_back( transform );
}

// *******************************
//
VoidPtr         ParamTransformVec::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

} //model
} //bv
