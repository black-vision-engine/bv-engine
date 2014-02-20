#include "ParamValEvaluatorFactory.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace model {

// ************************************************************** FLOAT **************************************************************

// *******************************
//
SimpleFloatEvaluator *       ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( ParamFloat * param, ValueFloat * value )
{
    return new SimpleFloatEvaluator( param, value );
}

// *******************************
//
SimpleFloatEvaluator *       ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( ParamFloat * param, const std::string & valueName )
{
    return CreateSimpleFloatEvaluator( param, ValuesFactory::CreateValueFloat( valueName ) );
}

// *******************************
//
SimpleFloatEvaluator *       ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( ParamFloat * param )
{
    return CreateSimpleFloatEvaluator( param, param->GetName() );
}

// *******************************
//
SimpleFloatEvaluator *       ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( const std::string & paramName )
{
    return CreateSimpleFloatEvaluator( ParametersFactory::CreateParameterFloat( paramName ), paramName );
}

// *******************************
//
SimpleFloatEvaluator *       ParamValEvaluatorFactory::CreateSimpleFloatEvaluator      ( const std::string & paramName, const std::string & valueName )
{
    return CreateSimpleFloatEvaluator( ParametersFactory::CreateParameterFloat( paramName ), valueName );
}


// ************************************************************** VEC4 **************************************************************

// *******************************
//
SimpleVec4Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( ParamVec4 * param, ValueVec4 * value )
{
    return new SimpleVec4Evaluator( param, value );
}

// *******************************
//
SimpleVec4Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( ParamVec4 * param, const std::string & valueName )
{
    return CreateSimpleVec4Evaluator( param, ValuesFactory::CreateValueVec4( valueName ) );
}

// *******************************
//
SimpleVec4Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( ParamVec4 * param )
{
    return CreateSimpleVec4Evaluator( param, param->GetName() );
}

// *******************************
//
SimpleVec4Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( const std::string & paramName )
{
    return CreateSimpleVec4Evaluator( ParametersFactory::CreateParameterVec4( paramName ), paramName );
}

// *******************************
//
SimpleVec4Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec4Evaluator       ( const std::string & paramName, const std::string & valueName )
{
    return CreateSimpleVec4Evaluator( ParametersFactory::CreateParameterVec4( paramName ), valueName );
}


// ************************************************************** VEC3 **************************************************************

// *******************************
//
SimpleVec3Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( ParamVec3 * param, ValueVec3 * value )
{
    return new SimpleVec3Evaluator( param, value );
}

// *******************************
//
SimpleVec3Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( ParamVec3 * param, const std::string & valueName )
{
    return CreateSimpleVec3Evaluator( param, ValuesFactory::CreateValueVec3( valueName ) );
}

// *******************************
//
SimpleVec3Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( ParamVec3 * param )
{
    return CreateSimpleVec3Evaluator( param, param->GetName() );
}

// *******************************
//
SimpleVec3Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( const std::string & paramName )
{
    return CreateSimpleVec3Evaluator( ParametersFactory::CreateParameterVec3( paramName ), paramName );
}

// *******************************
//
SimpleVec3Evaluator *        ParamValEvaluatorFactory::CreateSimpleVec3Evaluator       ( const std::string & paramName, const std::string & valueName )
{
    return CreateSimpleVec3Evaluator( ParametersFactory::CreateParameterVec3( paramName ), valueName );
}


// ************************************************************** MAT2 **************************************************************

// *******************************
//
SimpleMat2Evaluator *        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( ParamMat2 * param, ValueMat2 * value )
{
    return new SimpleMat2Evaluator( param, value );
}

// *******************************
//
SimpleMat2Evaluator *        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( ParamMat2 * param, const std::string & valueName )
{
    return CreateSimpleMat2Evaluator( param, ValuesFactory::CreateValueMat2( valueName ) );
}

// *******************************
//
SimpleMat2Evaluator *        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( ParamMat2 * param )
{
    return CreateSimpleMat2Evaluator( param, param->GetName() );
}

// *******************************
//
SimpleMat2Evaluator *        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( const std::string & paramName )
{
    return CreateSimpleMat2Evaluator( ParametersFactory::CreateParameterMat2( paramName ), paramName );
}

// *******************************
//
SimpleMat2Evaluator *        ParamValEvaluatorFactory::CreateSimpleMat2Evaluator       ( const std::string & paramName, const std::string & valueName )
{
    return CreateSimpleMat2Evaluator( ParametersFactory::CreateParameterMat2( paramName ), valueName );
}


// ************************************************************** TRANSFORM **************************************************************

// *******************************
//
SimpleTransformEvaluator *   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( ParamTransform * param, ValueMat4 * value )
{
    return new SimpleTransformEvaluator( param, value );
}

// *******************************
//
SimpleTransformEvaluator *   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( ParamTransform * param, const std::string & valueName )
{
    return CreateSimpleTransformEvaluator( param, ValuesFactory::CreateValueMat4( valueName ) );
}

// *******************************
//
SimpleTransformEvaluator *   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( ParamTransform * param )
{
    return CreateSimpleTransformEvaluator( param, param->GetName() );
}

// *******************************
//
SimpleTransformEvaluator *   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( const std::string & paramName )
{
    return CreateSimpleTransformEvaluator( ParametersFactory::CreateParameterTransform( paramName ), paramName );
}

// *******************************
//
SimpleTransformEvaluator *   ParamValEvaluatorFactory::CreateSimpleTransformEvaluator  ( const std::string & paramName, const std::string & valueName )
{
    return CreateSimpleTransformEvaluator( ParametersFactory::CreateParameterTransform( paramName ), valueName );
}


// ************************************************************** TRANSFORM VEC **************************************************************

// *******************************
//
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, ValueMat4 * value )
{
    ValueMat4PtrVec values( 1, value );
    ParamTransformVec * ptv = ParametersFactory::CreateParameterTransformVec( paramName, 1 );

    return CreateTransformVecEvaluator( ptv, values );
}

// *******************************
//
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName )
{
    return CreateTransformVecEvaluator( paramName, paramName );
}

// *******************************
//
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, const std::string & valueName )
{
    return CreateTransformVecEvaluator( paramName, ValuesFactory::CreateValueMat4( valueName ) );    
}

// *******************************
//
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVec * param, ValueMat4PtrVec values )
{
    assert( values.size() > 0 );
    assert( param->NumTransforms() == values.size() );

    return new TransformVecParamValEvaluator( param, values ); 
}

// *******************************
//
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVec * param, const std::string & valueName )
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
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVec * param, const std::vector< std::string > & valueNames )
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
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( ParamTransformVec * param )
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
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, unsigned int numTransformations )
{
    return CreateTransformVecEvaluator( paramName, paramName, numTransformations );
}

// *******************************
//
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, const std::string & valueName, unsigned int numTransformations )
{
    assert( numTransformations > 0 );

    ParamTransformVec * ptv = ParametersFactory::CreateParameterTransformVec( paramName, numTransformations );

    ValueMat4PtrVec values( numTransformations );

    for( unsigned int i = 0; i < numTransformations; ++i )
    {
        values.push_back( ValuesFactory::CreateValueMat4( valueName ) );
    }

    return CreateTransformVecEvaluator( ptv, values );
}

// *******************************
//
TransformVecParamValEvaluator *   ParamValEvaluatorFactory::CreateTransformVecEvaluator( const std::string & paramName, const std::vector< std::string > & valueNames )
{
    assert( valueNames.size() > 0 );

    ParamTransformVec * ptv = ParametersFactory::CreateParameterTransformVec( paramName, valueNames.size() );

    ValueMat4PtrVec values( valueNames.size() );

    for( auto name : valueNames )
    {
        values.push_back( ValuesFactory::CreateValueMat4( name ) );
    }

    return CreateTransformVecEvaluator( ptv, values );
}

} //model
} //bv
