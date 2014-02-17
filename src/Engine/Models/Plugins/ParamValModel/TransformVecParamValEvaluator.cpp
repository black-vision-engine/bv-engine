#include "TransformVecParamValEvaluator.h"


namespace bv { namespace model {

// ********************************
//
TransformVecParamValEvaluator::TransformVecParamValEvaluator ( ParamTransformVec * param, const ValueMat4PtrVec val )
    : m_param( param )
    , m_mat4Values( val )
    , m_paramWrapper( 1, param )
{
    assert( param );
    assert( param->NumTransforms() == val.size() );

    for( ValueMat4 * m4v : val )
    {
        m_values.push_back( m4v );
    }
}

// ********************************
//
std::vector< IParameter * > &   TransformVecParamValEvaluator::GetParameters   ()
{
    return m_paramWrapper;
}

// ********************************
//
std::vector< IValue * > &       TransformVecParamValEvaluator::GetValues       ()
{
    return m_values;
}

// ********************************
//
void                            TransformVecParamValEvaluator::Evaluate        ( TimeType t )
{
    for( unsigned int i = 0; i < m_param->NumTransforms(); ++i )
    {
        m_mat4Values[ i ]->SetValue( m_param->Evaluate( i, t ) );
    }
}

// ********************************
//
ParamTransformVec *             TransformVecParamValEvaluator::Param           ()
{
    return m_param;
}

// ********************************
//
ValueMat4PtrVec &               TransformVecParamValEvaluator::Value           ()
{
    return m_mat4Values;
}

} //model
} //bv
