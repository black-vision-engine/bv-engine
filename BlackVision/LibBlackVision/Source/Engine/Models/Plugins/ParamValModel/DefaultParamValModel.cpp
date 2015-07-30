#include "DefaultParamValModel.h"

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"

namespace bv { namespace model {

// *******************************
//
DefaultParamValModel::DefaultParamValModel                                          ()
{
}

// *******************************
//
DefaultParamValModel::~DefaultParamValModel                                         ()
{
}

// *******************************
//
std::vector< IParameterPtr > &              DefaultParamValModel::GetParameters     ()
{
    return m_parameters;
}

// *******************************
//
const std::vector< bv::IValueConstPtr > &   DefaultParamValModel::GetValues         () const
{
    return m_values;
}

// *******************************
//
std::vector< IParamValEvaluatorPtr > &      DefaultParamValModel::GetEvaluators     ()
{
    return m_evaluators;
}

// *******************************
//
IParameterPtr                               DefaultParamValModel::GetParameter      ( const std::string & name )
{
    for( auto param : m_parameters )
    {
        if( param->GetName() == name )
        {
            return param;
        }
    }

    return nullptr;
}

// *******************************
//
bv::IValueConstPtr                          DefaultParamValModel::GetValue          ( const std::string & name ) const
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

// *******************************
//
IStatedValuePtr                                   DefaultParamValModel::GetState        ( const std::string & name )
{
    return m_states[ name ];
}

// *******************************
//
void                                        DefaultParamValModel::Update            ()
{
    for( auto evaluator : m_evaluators )
    {
        evaluator->Evaluate();
    }

    for( auto updater : m_stateUpdaters )
    {
        updater->DoUpdate();
    }
}

void CopyParameter( IParameterPtr out, IParameterPtr in )
{
    //assert( out->GetType() == in->GetType() ); // FIXME: uncomment when parameter deserialization is finished
    
    if( out->GetType() != in->GetType() )
        return;

    if( out->GetType() == ModelParamType::MPT_VEC4 )
    {
        auto inT = QueryTypedParam< ParamVec4Ptr >( in );
        auto outT = QueryTypedParam< ParamVec4Ptr >( out );

        outT->AccessInterpolator() = inT->AccessInterpolator();
    }
    else if( out->GetType() == ModelParamType::MPT_VEC2 )
    {
        auto inT = QueryTypedParam< ParamVec2Ptr >( in );
        auto outT = QueryTypedParam< ParamVec2Ptr >( out );

        outT->AccessInterpolator() = inT->AccessInterpolator();
    } 
    else if( out->GetType() == ModelParamType::MPT_FLOAT )
    {
        auto inT = QueryTypedParam< ParamFloatPtr >( in );
        auto outT = QueryTypedParam< ParamFloatPtr >( out );

        outT->AccessInterpolator() = inT->AccessInterpolator();
    } 
    else if( out->GetType() == ModelParamType::MPT_TRANSFORM_VEC )
    {
        auto inT = QueryTypedParam< ParamTransformVecPtr >( in );
        auto outT = QueryTypedParam< ParamTransformVecPtr >( out );

        for( unsigned int i = 0; i < inT->NumTransforms(); i++ )
        {
            auto trans = inT->Transform( i );
            outT->InsertTransform( i, trans );
        }
    } 
    else
    {
        //assert( false );
        return;
    }
}

// *******************************
//
void                                        DefaultParamValModel::SetParameter      ( IParameterPtr param )
{
    std::string name = param->GetName(); // FIXME: make this an argument

    auto prevParam = GetParameter( name );
    CopyParameter( prevParam, param );
}

// *******************************
//
void                                        DefaultParamValModel::AddParameter      ( IParameterPtr param )
{
    m_parameters.push_back( IParameterPtr( param ) );
}

// *******************************
//
void                                        DefaultParamValModel::AddValue          ( bv::IValuePtr val )
{
    m_values.push_back( val );
    m_valuesNC.push_back( val );
}

// *******************************
//
void                                        DefaultParamValModel::AddValue          ( bv::IValueConstPtr val )
{
    m_values.push_back( val );
    m_valuesNC.push_back( std::const_pointer_cast< bv::IValue >( val ) );
}

// *******************************
//
void                                        DefaultParamValModel::AddEvaluator      ( IParamValEvaluatorPtr evaluator )
{
    m_evaluators.push_back( evaluator );
}

// *******************************
//
void                                        DefaultParamValModel::RegisterAll       ( IParamValEvaluatorPtr evaluator )
{
    AddEvaluator( evaluator );

    for( auto param : evaluator->GetParameters() )
    {
        AddParameter( param );
    }

    for( auto value : evaluator->GetValues() )
    {
        AddValue( value );
    }
}

// *******************************
//
void                                                DefaultParamValModel::AddState        ( const std::string & name, IStatedValuePtr state, IUpdaterPtr updater )
{
    m_states[ name ] = state;
    m_stateUpdaters.push_back( updater );
}

// *******************************
//
const std::vector< bv::IValuePtr > &         DefaultParamValModel::GetValuesNC       () const
{
    return m_valuesNC;
}

} //model
} //bv
