#include "stdafx.h"

#include "DefaultParamValModel.h"

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.inl"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"




#include "Memory/MemoryLeaks.h"



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
void                                        DefaultParamValModel::Serialize       ( ISerializer& ser ) const
{
ser.EnterArray( "params" );
    for( auto param : m_parameters )
        param->Serialize( ser );
ser.ExitChild();
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

// ***********************
//
const std::map< std::string, IStatedValuePtr > &    DefaultParamValModel::GetStates       ()
{
    return m_states;
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

    for( auto & updater : m_stateUpdaters )
    {
        updater->DoUpdate();
    }
}

void CopyParameter( IParameterPtr out, IParameterPtr in )
{
	if( out->GetType() != in->GetType() )
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Params types are not equal. '" << ( int ) out->GetType() << " != " << ( int ) in->GetType() << "'. Check a type of param '" << in->GetName() << "'.";
	}
	else
	{
		out->SetTimeEvaluator( in->GetTimeEvaluator() );

		if( out->GetType() == ModelParamType::MPT_VEC4 )
		{
			auto inT = QueryTypedParam< ParamVec4Ptr >( in );
			auto outT = QueryTypedParam< ParamVec4Ptr >( out );

			outT->AccessInterpolator() = inT->AccessInterpolator();
		}
		else if( out->GetType() == ModelParamType::MPT_VEC3 )
		{
			auto inT = QueryTypedParam< ParamVec3Ptr >( in );
			auto outT = QueryTypedParam< ParamVec3Ptr >( out );

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
		else if( out->GetType() == ModelParamType::MPT_INT )
		{
			auto inT = QueryTypedParam< ParamIntPtr >( in );
			auto outT = QueryTypedParam< ParamIntPtr >( out );

			outT->AccessInterpolator() = inT->AccessInterpolator();
		}
		else if( out->GetType() == ModelParamType::MPT_BOOL )
		{
			auto inT = QueryTypedParam< ParamBoolPtr >( in );
			auto outT = QueryTypedParam< ParamBoolPtr >( out );

			outT->AccessInterpolator() = inT->AccessInterpolator();
		}
		else if( out->GetType() == ModelParamType::MPT_TRANSFORM )
		{
			auto inT = QueryTypedParam< ParamTransformPtr >( in );
			auto outT = QueryTypedParam< ParamTransformPtr >( out );

			outT->Transform() = inT->Transform();
		}
		else if( out->GetType() == ModelParamType::MPT_ENUM )
		{
			auto inTypedParam = QueryTypedParam< std::shared_ptr< ParamEnum< GenericEnumType > > >( in );
			auto outTypedParam = QueryTypedParam< std::shared_ptr< ParamEnum< GenericEnumType > > >( out );

			outTypedParam->AccessInterpolator() = inTypedParam->AccessInterpolator();
		}
		else if( out->GetType() == ModelParamType::MPT_STRING )
		{
			auto inT = QueryTypedParam< ParamStringPtr >( in );
			auto outT = QueryTypedParam< ParamStringPtr >( out );

			outT->AccessInterpolator() = inT->AccessInterpolator();
		}
		else if( out->GetType() == ModelParamType::MPT_WSTRING )
		{
			auto inT = QueryTypedParam< ParamWStringPtr >( in );
			auto outT = QueryTypedParam< ParamWStringPtr >( out );

			outT->AccessInterpolator() = inT->AccessInterpolator();
		}
		else
		{
			LOG_MESSAGE( SeverityLevel::error ) << "Unknown param type '" << ( int ) out->GetType() << "'. Check a type of param '" << out->GetName() << "'.";
			return;
		}
	}
}

// ***********************
//
void                                        DefaultParamValModel::RemoveParamVal  ( const std::string & name )
{
    // Remove parameters
    for( int i = 0; i < ( Int32 )m_parameters.size(); ++i )
    {
        if( m_parameters[ i ]->GetName() == name )
            m_parameters.erase( m_parameters.begin() + i );
    }

    // Remove values
    for( int i = 0; i < ( Int32 )m_values.size(); ++i )
    {
        if( m_values[ i ]->GetName() == name )
            m_values.erase( m_values.begin() + i );
    }

    for( int i = 0; i < ( Int32 )m_valuesNC.size(); ++i )
    {
        if( m_valuesNC[ i ]->GetName() == name )
            m_valuesNC.erase( m_valuesNC.begin() + i );
    }

    //Remove evaluators
    for( int i = 0; i < ( Int32 )m_evaluators.size(); ++i )
    {
        auto & evalVec = m_evaluators[ i ]->GetParameters();
        if( evalVec[ 0 ]->GetName() == name )
            m_evaluators.erase( m_evaluators.begin() + i );
    }

    // Remove states
    m_states.erase( name );

    // Remove updaters
    for( int i = 0; i < ( Int32 )m_stateUpdaters.size(); ++i )
    {
        if( m_stateUpdaters[ i ]->GetName() == name )
            m_stateUpdaters.erase( m_stateUpdaters.begin() + i );
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
void                                                DefaultParamValModel::AddState        ( const std::string & name, IStatedValuePtr state, IStateUpdaterPtr updater )
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
