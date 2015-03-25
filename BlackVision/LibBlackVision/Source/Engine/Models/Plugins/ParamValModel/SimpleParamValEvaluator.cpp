#include "SimpleParamValEvaluator.h"


namespace bv { namespace model {

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::SimpleParamValEvaluator( ParamTypePtr param, ValueTypePtr val )
    : m_paramWrapper( 1, param )
    , m_valueWrapper( 1, val )
    , m_param( param )
    , m_value( val )
{
    assert( param );
    assert( val );
}

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
std::vector< IParameterPtr > &              SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::GetParameters   ()
{
    return m_paramWrapper;
}

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
const std::vector< bv::IValueConstPtr > &   SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::GetValues () const
{
    return m_valueWrapper;
}

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
IParameterPtr                               SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::GetParameter    ( const std::string & name )
{
    if( m_param->GetName() == name )
    {
        return m_param;
    }

    return nullptr;
}

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
bv::IValueConstPtr                         SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::GetValue        ( const std::string & name ) const
{
    if( m_value->GetName() == name )
    {
        return m_value;
    }

    return nullptr;
}

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
void                                        SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::Evaluate       ()
{
    m_value->SetValue( m_param->Evaluate() );
}

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
ParamTypePtr                                 SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::Parameter       ()
{
    return m_param;
}

// *******************************
//
template< typename ParamTypePtr, typename ValueTypePtr >
ValueTypePtr                                 SimpleParamValEvaluator< ParamTypePtr, ValueTypePtr >::Value           ()
{
    return m_value;
}

} // model
} // bv

//Explicit instantiation - this way class' implementation can be stored in cpp file (like here)
template class bv::model::SimpleParamValEvaluator<bv::model::ParamIntPtr, bv::ValueIntPtr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamFloatPtr, bv::ValueFloatPtr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamVec4Ptr, bv::ValueVec4Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamVec3Ptr, bv::ValueVec3Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamVec2Ptr, bv::ValueVec2Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamMat2Ptr, bv::ValueMat2Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamTransformPtr, bv::ValueMat4Ptr>;
