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
    for( auto param : m_parameters )
    {
        delete param;
    }

    for( auto value : m_values )
    {
        delete value;
    }

    for( auto evaluator : m_evaluators )
    {
        delete evaluator;
    }
}

// *******************************
//
std::vector< IParameter * > &               DefaultParamValModel::GetParameters     ()
{
    return m_parameters;
}

// *******************************
//
const std::vector< const bv::IValue * > &   DefaultParamValModel::GetValues         () const
{
    return m_values;
}

// *******************************
//
std::vector< IParamValEvaluator * > &       DefaultParamValModel::GetEvaluators     ()
{
    return m_evaluators;
}

// *******************************
//
IParameter *                                DefaultParamValModel::GetParameter      ( const std::string & name )
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
const bv::IValue *                          DefaultParamValModel::GetValue          ( const std::string & name ) const
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
void                                        DefaultParamValModel::Update            ()
{
    for( IParamValEvaluator * evaluator : m_evaluators )
    {
        evaluator->Evaluate();
    }
}

// *******************************
//
void                                        DefaultParamValModel::AddParameter      ( IParameter * param )
{
    m_parameters.push_back( param );
}

// *******************************
//
void                                        DefaultParamValModel::AddValue          ( bv::IValue * val )
{
    m_values.push_back( val );
    m_valuesNC.push_back( val );
}

// *******************************
//
void                                        DefaultParamValModel::AddValue          ( const bv::IValue * val )
{
    m_values.push_back( val );
    m_valuesNC.push_back( const_cast< bv::IValue * >( val ) );
}

// *******************************
//
void                                        DefaultParamValModel::AddEvaluator      ( IParamValEvaluator * evaluator )
{
    m_evaluators.push_back( evaluator );
}

// *******************************
//
void                                        DefaultParamValModel::RegisterAll       ( IParamValEvaluator * evaluator )
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
const std::vector< bv::IValue * > &         DefaultParamValModel::GetValuesNC       () const
{
    return m_valuesNC;
}

} //model
} //bv
