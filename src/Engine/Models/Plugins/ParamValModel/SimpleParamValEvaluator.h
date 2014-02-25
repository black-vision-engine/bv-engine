#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

template< typename ParamType, typename ValueType >
class SimpleParamValEvaluator : public IParamValEvaluator
{
private:

    ParamType *     m_param;
    ValueType *     m_value;

    std::vector< IParameter * >         m_paramWrapper;
    std::vector< const bv::IValue * >   m_valueWrapper;

private:

                    SimpleParamValEvaluator ( ParamType * param, ValueType * val );

public:

    virtual std::vector< IParameter * > &               GetParameters   () override;
    virtual const std::vector< const bv::IValue * > &   GetValues       () const override;
    
    virtual IParameter *                                GetParameter    ( const std::string & name ) override;
    virtual const bv::IValue *                          GetValue        ( const std::string & name ) const override;

    virtual void                                        Evaluate        ( TimeType t ) override;

    ParamType *                                         Parameter       ();
    ValueType *                                         Value           ();

    friend class ParamValEvaluatorFactory;

};

// *******************************
//
template< typename ParamType, typename ValueType >
SimpleParamValEvaluator< ParamType, ValueType >::SimpleParamValEvaluator( ParamType * param, ValueType * val )
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
template< typename ParamType, typename ValueType >
std::vector< IParameter * > &               SimpleParamValEvaluator< ParamType, ValueType >::GetParameters   ()
{
    return m_paramWrapper;
}

// *******************************
//
template< typename ParamType, typename ValueType >
const std::vector< const bv::IValue * > &   SimpleParamValEvaluator< ParamType, ValueType >::GetValues () const
{
    return m_valueWrapper;
}

// *******************************
//
template< typename ParamType, typename ValueType >
IParameter *                                SimpleParamValEvaluator< ParamType, ValueType >::GetParameter    ( const std::string & name )
{
    if( m_param->GetName() == name )
    {
        return m_param;
    }

    return nullptr;
}

// *******************************
//
template< typename ParamType, typename ValueType >
const bv::IValue *                          SimpleParamValEvaluator< ParamType, ValueType >::GetValue        ( const std::string & name ) const
{
    if( m_value->GetName() == name )
    {
        return m_value;
    }

    return nullptr;
}

// *******************************
//
template< typename ParamType, typename ValueType >
void                                        SimpleParamValEvaluator< ParamType, ValueType >::Evaluate       ( TimeType t )
{
    m_value->SetValue( m_param->Evaluate( t ) );
}

// *******************************
//
template< typename ParamType, typename ValueType >
ParamType *                                 SimpleParamValEvaluator< ParamType, ValueType >::Parameter       ()
{
    return m_param;
}

// *******************************
//
template< typename ParamType, typename ValueType >
ValueType *                                 SimpleParamValEvaluator< ParamType, ValueType >::Value           ()
{
    return m_value;
}

typedef SimpleParamValEvaluator< ParamFloat, ValueFloat >       SimpleFloatEvaluator;
typedef SimpleParamValEvaluator< ParamVec4, ValueVec4 >         SimpleVec4Evaluator;
typedef SimpleParamValEvaluator< ParamVec3, ValueVec3 >         SimpleVec3Evaluator;
typedef SimpleParamValEvaluator< ParamMat2, ValueMat2 >         SimpleMat2Evaluator;
typedef SimpleParamValEvaluator< ParamTransform, ValueMat4 >    SimpleTransformEvaluator;
    
} //model
} //bv
