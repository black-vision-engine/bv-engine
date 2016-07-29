#include "stdafx.h"

#include "ParamValEvaluatorFactory.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Types/Values/ValuesFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

    //IParamValEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleEvaluator           ( IParameterPtr param )
    //{
    //    //auto value = nullptr;
    //    //return std::make_shared< SimpleParamValEvaluator< ParamIntPtr, ValueIntPtr > >( param, value );

    //    if( param->GetType() == ModelParamType::MPT_INT )
    //    {
    //        auto iParam = QueryTypedParam< ParamIntPtr >( param );
    //        return CreateSimpleIntEvaluator( iParam );
    //    }
    //    else
    //    {
    //        assert( false );
    //        return nullptr;
    //    }
    //}

// ************************************************************** BOOL **************************************************************

// ***********************
//
SimpleBoolEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleBoolEvaluator       ( ParamBoolPtr param, ValueBoolPtr value )
{
    return std::make_shared< SimpleBoolEvaluator >( param, value );
}

// ***********************
//
SimpleBoolEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleBoolEvaluator       ( ParamBoolPtr param, const std::string & valueName )
{
    return CreateSimpleBoolEvaluator( param, ValuesFactory::CreateValueBool( valueName ) );
}

// ***********************
//
SimpleBoolEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleBoolEvaluator       ( ParamBoolPtr param )
{
    return CreateSimpleBoolEvaluator( param, param->GetName() );
}

// ***********************
//
SimpleBoolEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleBoolEvaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleBoolEvaluator( ParametersFactory::CreateParameterBool( paramName, timeEvaluator ), paramName );
}

// ***********************
//
SimpleBoolEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleBoolEvaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleBoolEvaluator( ParametersFactory::CreateParameterBool( paramName, timeEvaluator ), valueName );
}


// ************************************************************** INT **************************************************************

// *******************************
//
SimpleIntEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleIntEvaluator      ( ParamIntPtr param, ValueIntPtr value )
{
    return std::make_shared< SimpleIntEvaluator >( param, value );
}

// *******************************
//
SimpleIntEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleIntEvaluator      ( ParamIntPtr param, const std::string & valueName )
{
    return CreateSimpleIntEvaluator( param, ValuesFactory::CreateValueInt( valueName ) );
}

// *******************************
//
SimpleIntEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleIntEvaluator      ( ParamIntPtr param )
{
    return CreateSimpleIntEvaluator( param, param->GetName() );
}

// *******************************
//
SimpleIntEvaluatorPtr       ParamValEvaluatorFactory::CreateSimpleIntEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleIntEvaluator( ParametersFactory::CreateParameterInt( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleIntEvaluatorPtr       ParamValEvaluatorFactory::CreateSimpleIntEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleIntEvaluator( ParametersFactory::CreateParameterInt( paramName, timeEvaluator ), valueName );
}


// ************************************************************** FLOAT **************************************************************

// *******************************
//
SimpleFloatEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( ParamFloatPtr param, ValueFloatPtr value )
{
    return std::make_shared< SimpleFloatEvaluator >( param, value );
}

// *******************************
//
SimpleFloatEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( ParamFloatPtr param, const std::string & valueName )
{
    return CreateSimpleFloatEvaluator( param, ValuesFactory::CreateValueFloat( valueName ) );
}

// *******************************
//
SimpleFloatEvaluatorPtr      ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( ParamFloatPtr param )
{
    return CreateSimpleFloatEvaluator( param, param->GetName() );
}

// *******************************
//
SimpleFloatEvaluatorPtr       ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleFloatEvaluator( ParametersFactory::CreateParameterFloat( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleFloatEvaluatorPtr       ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleFloatEvaluator( ParametersFactory::CreateParameterFloat( paramName, timeEvaluator ), valueName );
}


// ************************************************************** VEC4 **************************************************************

// *******************************
//
SimpleVec4EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( ParamVec4Ptr param, ValueVec4Ptr value )
{
    return std::make_shared< SimpleVec4Evaluator >( param, value );
}

// *******************************
//
SimpleVec4EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( ParamVec4Ptr param, const std::string & valueName )
{
    return CreateSimpleVec4Evaluator( param, ValuesFactory::CreateValueVec4( valueName ) );
}

// *******************************
//
SimpleVec4EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( ParamVec4Ptr param )
{
    return CreateSimpleVec4Evaluator( param, param->GetName() );
}

// *******************************
//
SimpleVec4EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleVec4Evaluator( ParametersFactory::CreateParameterVec4( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleVec4EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleVec4Evaluator( ParametersFactory::CreateParameterVec4( paramName, timeEvaluator ), valueName );
}


// ************************************************************** VEC3 **************************************************************

// *******************************
//
SimpleVec3EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( ParamVec3Ptr param, ValueVec3Ptr value )
{
    return std::make_shared< SimpleVec3Evaluator >( param, value );
}

// *******************************
//
SimpleVec3EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( ParamVec3Ptr param, const std::string & valueName )
{
    return CreateSimpleVec3Evaluator( param, ValuesFactory::CreateValueVec3( valueName ) );
}

// *******************************
//
SimpleVec3EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( ParamVec3Ptr param )
{
    return CreateSimpleVec3Evaluator( param, param->GetName() );
}

// *******************************
//
SimpleVec3EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleVec3Evaluator( ParametersFactory::CreateParameterVec3( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleVec3EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleVec3Evaluator( ParametersFactory::CreateParameterVec3( paramName, timeEvaluator ), valueName );
}


// ************************************************************** VEC2 **************************************************************

// *******************************
//
SimpleVec2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec2Evaluator       ( ParamVec2Ptr param, ValueVec2Ptr value )
{
    return std::make_shared< SimpleVec2Evaluator >( param, value );
}

// *******************************
//
SimpleVec2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec2Evaluator       ( ParamVec2Ptr param, const std::string & valueName )
{
    return CreateSimpleVec2Evaluator( param, ValuesFactory::CreateValueVec2( valueName ) );
}

// *******************************
//
SimpleVec2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec2Evaluator       ( ParamVec2Ptr param )
{
    return CreateSimpleVec2Evaluator( param, param->GetName() );
}

// *******************************
//
SimpleVec2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleVec2Evaluator( ParametersFactory::CreateParameterVec2( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleVec2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleVec2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleVec2Evaluator( ParametersFactory::CreateParameterVec2( paramName, timeEvaluator ), valueName );
}

// ************************************************************** MAT2 **************************************************************

// *******************************
//
SimpleMat2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( ParamMat2Ptr param, ValueMat2Ptr value )
{
    return std::make_shared< SimpleMat2Evaluator >( param, value );
}

// *******************************
//
SimpleMat2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( ParamMat2Ptr param, const std::string & valueName )
{
    return CreateSimpleMat2Evaluator( param, ValuesFactory::CreateValueMat2( valueName ) );
}

// *******************************
//
SimpleMat2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( ParamMat2Ptr param )
{
    return CreateSimpleMat2Evaluator( param, param->GetName() );
}

// *******************************
//
SimpleMat2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleMat2Evaluator( ParametersFactory::CreateParameterMat2( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleMat2EvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleMat2Evaluator( ParametersFactory::CreateParameterMat2( paramName, timeEvaluator ), valueName );
}


// ************************************************************** TRANSFORM **************************************************************

// *******************************
//
SimpleTransformEvaluatorPtr   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( ParamTransformPtr param, ValueMat4Ptr value )
{
    return std::make_shared< SimpleTransformEvaluator >( param, value );
}

// *******************************
//
SimpleTransformEvaluatorPtr   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( ParamTransformPtr param, const std::string & valueName )
{
    return CreateSimpleTransformEvaluator( param, ValuesFactory::CreateValueMat4( valueName ) );
}

// *******************************
//
SimpleTransformEvaluatorPtr   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( ParamTransformPtr param )
{
    return CreateSimpleTransformEvaluator( param, param->GetName() );
}

// *******************************
//
SimpleTransformEvaluatorPtr   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleTransformEvaluator( ParametersFactory::CreateParameterTransform( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleTransformEvaluatorPtr   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleTransformEvaluator( ParametersFactory::CreateParameterTransform( paramName, timeEvaluator ), valueName );
}

// *******************************
//
SimpleTransformEvaluatorPtr   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, ValueMat4Ptr value )
{
    return CreateSimpleTransformEvaluator( ParametersFactory::CreateParameterTransform( paramName, timeEvaluator ), value );
}


// *******************************
//
SimpleStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleStringEvaluator( ParamStringPtr param, ValueStringPtr value )
{
    return std::make_shared< SimpleStringEvaluator >( param, value );
}

// *******************************
//
SimpleStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleStringEvaluator     ( ParamStringPtr param, const std::string & valueName )
{
    return CreateSimpleStringEvaluator( param, ValuesFactory::CreateValueString( valueName ) );
}

// *******************************
//
SimpleStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleStringEvaluator     ( ParamStringPtr param )
{
    return CreateSimpleStringEvaluator( param, param->GetName() );
}

// *******************************
//
SimpleStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleStringEvaluator     ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleStringEvaluator( ParametersFactory::CreateTypedSimpleParameter< ParamString >( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleStringEvaluator     ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleStringEvaluator( ParametersFactory::CreateTypedSimpleParameter< ParamString >( paramName, timeEvaluator ), valueName );
}

// *******************************
//
SimpleWStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleWStringEvaluator    ( ParamWStringPtr param, ValueWStringPtr value )
{
    return std::make_shared< SimpleWStringEvaluator >( param, value );
}

// *******************************
//
SimpleWStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleWStringEvaluator    ( ParamWStringPtr param, const std::string & valueName )
{
    return CreateSimpleWStringEvaluator( param, ValuesFactory::CreateValueWString( valueName ) );
}

// *******************************
//
SimpleWStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleWStringEvaluator    ( ParamWStringPtr param )
{
    return CreateSimpleWStringEvaluator( param, param->GetName() );
}

// *******************************
//
SimpleWStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleWStringEvaluator    ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateSimpleWStringEvaluator( ParametersFactory::CreateTypedSimpleParameter< ParamWString >( paramName, timeEvaluator ), paramName );
}

// *******************************
//
SimpleWStringEvaluatorPtr        ParamValEvaluatorFactory::CreateSimpleWStringEvaluator    ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateSimpleWStringEvaluator( ParametersFactory::CreateTypedSimpleParameter< ParamWString >( paramName, timeEvaluator ), valueName );
}

} //model
} //bv
