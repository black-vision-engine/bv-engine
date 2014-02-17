#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Models/Plugins/Parameters/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

template< typename ParamType, typename ValueType >
class SimpleParamValEvaluator : public IParamValEvaluator
{
private:

    ParamType *     m_param;
    ValueType *     m_value;

    std::vector< IParameter * >     m_paramWrapper;
    std::vector< IValue * >         m_valueWrapper;

private:

                    SimpleParamValEvaluator ( ParamType * param, ValueType * val );

public:

    virtual std::vector< IParameter * > &   GetParameters   () override;
    virtual std::vector< IValue * > &       GetValues       () override;
    virtual void                            Evaluate        ( TimeType t ) override;

    friend class ParamValEvaluatorFactory;

};

// *******************************
//
template< typename ParamType, typename ValueType >
SimpleParamValEvaluator< ParamType, ValueType >::SimpleParamValEvaluator( ParamType * param, ValueType * val )
    : m_paramWrapper( 1, param )
    , m_valueWrapper( 1, val )
{
    assert( param );
    assert( val );
}

// *******************************
//
template< typename ParamType, typename ValueType >
std::vector< IParameter * > &   SimpleParamValEvaluator< ParamType, ValueType >::GetParameters   ()
{
    return m_paramWrapper;
}

// *******************************
//
template< typename ParamType, typename ValueType >
std::vector< IValue * > &       SimpleParamValEvaluator< ParamType, ValueType >::GetValues       ()
{
    return m_valueWrapper;
}

// *******************************
//
template< typename ParamType, typename ValueType >
void    SimpleParamValEvaluator< ParamType, ValueType >::Evaluate       ( TimeType t )
{
    m_value->SetValue( m_param->Evaluate( t ) );
}

typedef SimpleParamValEvaluator< ParamFloat, ValueFloat >       SimpleFloatEvaluator;
typedef SimpleParamValEvaluator< ParamVec4, ValueVec4 >         SimpleVec4Evaluator;
typedef SimpleParamValEvaluator< ParamVec3, ValueVec3 >         SimpleVec3Evaluator;
typedef SimpleParamValEvaluator< ParamMat2, ValueMat2 >         SimpleMat2Evaluator;
typedef SimpleParamValEvaluator< ParamTransform, ValueMat4 >    SimpleTransformEvaluator;
    
} //model
} //bv
