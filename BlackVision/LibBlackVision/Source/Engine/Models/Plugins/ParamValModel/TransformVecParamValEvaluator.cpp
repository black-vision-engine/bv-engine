#include "TransformVecParamValEvaluator.h"


namespace bv { namespace model {

// ********************************
//
TransformVecParamValEvaluator::TransformVecParamValEvaluator ( ParamTransformVecPtr param, const ValueMat4PtrVec val )
    : m_param( param )
    , m_mat4Values( val )
    , m_paramWrapper( 1, param )
{
    assert( param );
    assert( param->NumTransforms() == val.size() );

    for( auto m4v : val )
    {
        m_values.push_back( m4v );
    }
}

// ********************************
//
std::vector< IParameterPtr > &              TransformVecParamValEvaluator::GetParameters   ()
{
    return m_paramWrapper;
}

// ********************************
//
const std::vector< bv::IValueConstPtr > &   TransformVecParamValEvaluator::GetValues       () const
{
    return m_values;
}

// ********************************
//
IParameterPtr                               TransformVecParamValEvaluator::GetParameter    ( const std::string & name )
{
    if( m_param->GetName() == name )
    {
        return m_param;
    }

    return nullptr;
}

// ********************************
//
bv::IValueConstPtr                         TransformVecParamValEvaluator::GetValue        ( const std::string & name ) const
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
void                                        TransformVecParamValEvaluator::Evaluate        ()
{
// \begin{FIXME}
    while( m_mat4Values.size() < m_param->NumTransforms() )
        m_mat4Values.push_back( std::make_shared< ValueMat4 >( "" ) );
// \end{FIXME}
    assert( m_param->NumTransforms() == m_mat4Values.size() );
    for( unsigned int i = 0; i < m_param->NumTransforms(); ++i )
    {
        m_mat4Values[ i ]->SetValue( m_param->Evaluate( i ) );
    }
}

// ********************************
//
ParamTransformVecPtr                        TransformVecParamValEvaluator::Parameter       ()
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
