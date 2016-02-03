#include "stdafx.h"

#include "TransformParamValEvaluator.h"


namespace bv { namespace model {

// ********************************
//
TransformParamValEvaluator::TransformParamValEvaluator ( ParamTransformPtr param, const ValueMat4Ptr val )
    : m_param( param )
    , m_mat4Values( val )
    , m_paramWrapper( 1, param )
{
    assert( param );

    m_values.push_back( val );
}

// ********************************
//
std::vector< IParameterPtr > &              TransformParamValEvaluator::GetParameters   ()
{
    return m_paramWrapper;
}

// ********************************
//
const std::vector< bv::IValueConstPtr > &   TransformParamValEvaluator::GetValues       () const
{
    return m_values;
}

// ********************************
//
IParameterPtr                               TransformParamValEvaluator::GetParameter    ( const std::string & name )
{
    if( m_param->GetName() == name )
    {
        return m_param;
    }

    return nullptr;
}

// ********************************
//
bv::IValueConstPtr                         TransformParamValEvaluator::GetValue        ( const std::string & name ) const
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
void                                        TransformParamValEvaluator::Evaluate        ()
{
// \begin{FIXME}
    //while( m_mat4Values.size() < m_param->NumTransforms() )
    //    m_mat4Values.push_back( std::make_shared< ValueMat4 >( "" ) );
// \end{FIXME}
    m_mat4Values->SetValue( m_param->Evaluate() );
}

// ********************************
//
ParamTransformPtr                           TransformParamValEvaluator::Parameter       ()
{
    return m_param;
}

// ********************************
//
ValueMat4Ptr &                              TransformParamValEvaluator::Value           ()
{
    return m_mat4Values;
}

} //model
} //bv
