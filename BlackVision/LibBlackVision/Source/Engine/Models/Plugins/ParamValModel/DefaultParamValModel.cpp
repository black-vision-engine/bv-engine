#include "DefaultParamValModel.h"

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"


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
IStatePtr                                   DefaultParamValModel::GetState        ( const std::string & name )
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
void                                                DefaultParamValModel::AddState        ( const std::string & name, IStatePtr state, IUpdaterPtr updater )
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
