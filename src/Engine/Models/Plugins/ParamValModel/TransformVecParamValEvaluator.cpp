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
std::vector< IParameter * > &               TransformVecParamValEvaluator::GetParameters   ()
{
    return m_paramWrapper;
}

// ********************************
//
const std::vector< const bv::IValue * > &   TransformVecParamValEvaluator::GetValues       () const
{
    return m_values;
}

// ********************************
//
IParameter *                                TransformVecParamValEvaluator::GetParameter    ( const std::string & name )
{
    if( m_param->GetName() == name )
    {
        return m_param;
    }

    return nullptr;
}

// ********************************
//
const bv::IValue *                          TransformVecParamValEvaluator::GetValue        ( const std::string & name ) const
{
    for( auto value : m_values )
    {
        if( value->GetName() == name )
        {
            return value;
        }
    }

    return nullptr;
}

// ********************************
//
void                                        TransformVecParamValEvaluator::Evaluate        ( TimeType t )
{
    for( unsigned int i = 0; i < m_param->NumTransforms(); ++i )
    {
        m_mat4Values[ i ]->SetValue( m_param->Evaluate( i, t ) );
    }
}

// ********************************
//
ParamTransformVec *                         TransformVecParamValEvaluator::Parameter       ()
{
    return m_param;
}

// ********************************
//
ValueMat4PtrVec &                           TransformVecParamValEvaluator::Value           ()
{
    return m_mat4Values;
}

} //model
} //bv
