#include "ParamValEvaluatorFactory.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace model {

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


// ************************************************************** TRANSFORM VEC **************************************************************

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, ValueMat4Ptr value )
{
    ValueMat4PtrVec values( 1, value );
    ParamTransformVecPtr ptv = ParametersFactory::CreateParameterTransformVec( paramName, timeEvaluator, 1 );

    return CreateTransformVecEvaluator( ptv, values );
}

// *******************************
//
TransformVecParamValEvaluatorPtr  ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator )
{
    return CreateTransformVecEvaluator( paramName, timeEvaluator, paramName );
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName )
{
    return CreateTransformVecEvaluator( paramName, timeEvaluator, ValuesFactory::CreateValueMat4( valueName ) );    
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVecPtr param, ValueMat4PtrVec values )
{
    assert( values.size() > 0 );
    assert( param->NumTransforms() == values.size() );

    return std::make_shared< TransformVecParamValEvaluator >( param, values ); 
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVecPtr param, const std::string & valueName )
{
    assert( param->NumTransforms() > 0 );

    ValueMat4PtrVec values( param->NumTransforms() );

    for( unsigned int i = 0; i < param->NumTransforms(); ++i )
    {
        values.push_back( ValuesFactory::CreateValueMat4( valueName ) );
    }

    return CreateTransformVecEvaluator( param, values );
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVecPtr param, const std::vector< std::string > & valueNames )
{
    assert( param->NumTransforms() > 0 );
    assert( param->NumTransforms() == valueNames.size() );

    ValueMat4PtrVec values( valueNames.size() );

    for( auto name : valueNames )
    {
        values.push_back( ValuesFactory::CreateValueMat4( name ) );
    }

    return CreateTransformVecEvaluator( param, values );
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVecPtr param )
{
    assert( param->NumTransforms() > 0 );

    ValueMat4PtrVec values( param->NumTransforms() );

    for( unsigned int i = 0; i < param->NumTransforms(); ++i )
    {
        values.push_back( ValuesFactory::CreateValueMat4( param->GetName() ) );
    }

    return CreateTransformVecEvaluator( param, values );
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, unsigned int numTransformations )
{
    return CreateTransformVecEvaluator( paramName, timeEvaluator, paramName, numTransformations );
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName, unsigned int numTransformations )
{
    assert( numTransformations > 0 );

    ParamTransformVecPtr ptv = ParametersFactory::CreateParameterTransformVec( paramName, timeEvaluator, numTransformations );

    ValueMat4PtrVec values( numTransformations );

    for( unsigned int i = 0; i < numTransformations; ++i )
    {
        values.push_back( ValuesFactory::CreateValueMat4( valueName ) );
    }

    return CreateTransformVecEvaluator( ptv, values );
}

// *******************************
//
TransformVecParamValEvaluatorPtr   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::vector< std::string > & valueNames )
{
    assert( valueNames.size() > 0 );

    ParamTransformVecPtr ptv = ParametersFactory::CreateParameterTransformVec( paramName, timeEvaluator, valueNames.size() );

    ValueMat4PtrVec values( valueNames.size() );

    for( auto name : valueNames )
    {
        values.push_back( ValuesFactory::CreateValueMat4( name ) );
    }

    return CreateTransformVecEvaluator( ptv, values );
}

} //model
} //bv
