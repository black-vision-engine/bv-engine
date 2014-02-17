#include "DefaultParamValModel.h"

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"


namespace bv { namespace model {

// *******************************
//
DefaultParamValModel::DefaultParamValModel                                      ()
{
}

// *******************************
//
DefaultParamValModel::~DefaultParamValModel                                     ()
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
std::vector< IParameter * > &           DefaultParamValModel::GetParameters     ()
{
    return m_parameters;
}

// *******************************
//
std::vector< IValue * > &               DefaultParamValModel::GetValues         ()
{
    return m_values;
}

// *******************************
//
std::vector< IParamValEvaluator * > &   DefaultParamValModel::GetEvaluators     ()
{
    return m_evaluators;
}

// *******************************
//
void                                    DefaultParamValModel::Update            ( TimeType t )
{
    for( IParamValEvaluator * evaluator : m_evaluators )
    {
        evaluator->Evaluate( t );
    }
}

// *******************************
//
void                                    DefaultParamValModel::AddParameter      ( IParameter * param )
{
    m_parameters.push_back( param );
}

// *******************************
//
void                                    DefaultParamValModel::AddValue          ( IValue * val )
{
    m_values.push_back( val );
}

// *******************************
//
void                                    DefaultParamValModel::AddEvaluator      ( IParamValEvaluator * evaluator )
{
    m_evaluators.push_back( evaluator );
}

// *******************************
//
void                                    DefaultParamValModel::RegisterAll     ( IParamValEvaluator * evaluator )
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

} //model
} //bv
