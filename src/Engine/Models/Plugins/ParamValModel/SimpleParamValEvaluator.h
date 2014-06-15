#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

template< typename ParamTypePtr, typename ValueTypePtr >
class SimpleParamValEvaluator : public IParamValEvaluator
{
private:

    ParamTypePtr    m_param;
    ValueTypePtr    m_value;

    std::vector< IParameterPtr >        m_paramWrapper;
    std::vector< bv::IValueConstPtr >   m_valueWrapper;

public:

                    SimpleParamValEvaluator ( ParamTypePtr param, ValueTypePtr val );

public:

    virtual std::vector< IParameterPtr > &              GetParameters   () override;
    virtual const std::vector< bv::IValueConstPtr > &   GetValues       () const override;
    
    virtual IParameterPtr                               GetParameter    ( const std::string & name ) override;
    virtual bv::IValueConstPtr                          GetValue        ( const std::string & name ) const override;

    virtual void                                        Evaluate        () override;

    ParamTypePtr                                        Parameter       ();
    ValueTypePtr                                        Value           ();

    friend class ParamValEvaluatorFactory;

};

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

typedef SimpleParamValEvaluator< ParamFloatPtr, ValueFloatPtr >       SimpleFloatEvaluator;
typedef SimpleParamValEvaluator< ParamVec4Ptr, ValueVec4Ptr >         SimpleVec4Evaluator;
typedef SimpleParamValEvaluator< ParamVec3Ptr, ValueVec3Ptr >         SimpleVec3Evaluator;
typedef SimpleParamValEvaluator< ParamMat2Ptr, ValueMat2Ptr >         SimpleMat2Evaluator;
typedef SimpleParamValEvaluator< ParamTransformPtr, ValueMat4Ptr >    SimpleTransformEvaluator;

typedef std::shared_ptr< SimpleFloatEvaluator >                 SimpleFloatEvaluatorPtr;
typedef std::shared_ptr< SimpleVec4Evaluator >                  SimpleVec4EvaluatorPtr;
typedef std::shared_ptr< SimpleVec3Evaluator >                  SimpleVec3EvaluatorPtr;
typedef std::shared_ptr< SimpleMat2Evaluator >                  SimpleMat2EvaluatorPtr;
typedef std::shared_ptr< SimpleTransformEvaluator >             SimpleTransformEvaluatorPtr;

} //model
} //bv
