#pragma once

#include <string>

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.h"
#include "Engine/Models/Plugins/ParamValModel/TransformVecParamValEvaluator.h"


namespace bv { namespace model {

class ParamValEvaluatorFactory
{
public:

    static SimpleFloatEvaluator *       CreateSimpleFloatEvaluator      ( ParamFloat * param, ValueFloat * value );
    static SimpleFloatEvaluator *       CreateSimpleFloatEvaluator      ( ParamFloat * param, const std::string & valueName );
    static SimpleFloatEvaluator *       CreateSimpleFloatEvaluator      ( ParamFloat * param );
    static SimpleFloatEvaluator *       CreateSimpleFloatEvaluator      ( const std::string & paramName );
    static SimpleFloatEvaluator *       CreateSimpleFloatEvaluator      ( const std::string & paramName, const std::string & valueName );

    static SimpleVec4Evaluator *        CreateSimpleVec4Evaluator       ( ParamVec4 * param, ValueVec4 * value );
    static SimpleVec4Evaluator *        CreateSimpleVec4Evaluator       ( ParamVec4 * param, const std::string & valueName );
    static SimpleVec4Evaluator *        CreateSimpleVec4Evaluator       ( ParamVec4 * param );
    static SimpleVec4Evaluator *        CreateSimpleVec4Evaluator       ( const std::string & paramName );
    static SimpleVec4Evaluator *        CreateSimpleVec4Evaluator       ( const std::string & paramName, const std::string & valueName );

    static SimpleVec3Evaluator *        CreateSimpleVec3Evaluator       ( ParamVec3 * param, ValueVec3 * value );
    static SimpleVec3Evaluator *        CreateSimpleVec3Evaluator       ( ParamVec3 * param, const std::string & valueName );
    static SimpleVec3Evaluator *        CreateSimpleVec3Evaluator       ( ParamVec3 * param );
    static SimpleVec3Evaluator *        CreateSimpleVec3Evaluator       ( const std::string & paramName );
    static SimpleVec3Evaluator *        CreateSimpleVec3Evaluator       ( const std::string & paramName, const std::string & valueName );

    static SimpleMat2Evaluator *        CreateSimpleMat2Evaluator       ( ParamMat2 * param, ValueMat2 * value );
    static SimpleMat2Evaluator *        CreateSimpleMat2Evaluator       ( ParamMat2 * param, const std::string & valueName );
    static SimpleMat2Evaluator *        CreateSimpleMat2Evaluator       ( ParamMat2 * param );
    static SimpleMat2Evaluator *        CreateSimpleMat2Evaluator       ( const std::string & paramName );
    static SimpleMat2Evaluator *        CreateSimpleMat2Evaluator       ( const std::string & paramName, const std::string & valueName );

    static SimpleTransformEvaluator *   CreateSimpleTransformEvaluator  ( ParamTransform * param, ValueMat4 * value );
    static SimpleTransformEvaluator *   CreateSimpleTransformEvaluator  ( ParamTransform * param, const std::string & valueName );
    static SimpleTransformEvaluator *   CreateSimpleTransformEvaluator  ( ParamTransform * param );
    static SimpleTransformEvaluator *   CreateSimpleTransformEvaluator  ( const std::string & paramName );
    static SimpleTransformEvaluator *   CreateSimpleTransformEvaluator  ( const std::string & paramName, const std::string & valueName );

    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( const std::string & paramName, ValueMat4 * value );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( const std::string & paramName );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( const std::string & paramName, const std::string & valueName );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( ParamTransformVec * param, ValueMat4PtrVec values );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( ParamTransformVec * param, const std::string & valueName );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( ParamTransformVec * param, const std::vector< std::string > & valueNames );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( ParamTransformVec * param );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( const std::string & paramName, unsigned int numTransformations );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( const std::string & paramName, const std::string & valueName, unsigned int numTransformations );
    static TransformVecParamValEvaluator *   CreateTransformVecEvaluator( const std::string & paramName, const std::vector< std::string > & valueNames );

};

/*
typedef SimpleParamValEvaluator< ParamFloat, ValueFloat >       SimpleFloatEvaluator;
typedef SimpleParamValEvaluator< ParamVec4, ValueVec4 >         SimpleVec4Evaluator;
typedef SimpleParamValEvaluator< ParamVec3, ValueVec3 >         SimpleVec2Evaluator;
typedef SimpleParamValEvaluator< ParamMat2, ValueMat2 >         SimpleMat2Evaluator;
typedef SimpleParamValEvaluator< ParamTransform, ValueMat4 >    SimpleTransformEvaluator;
*/

} //model
} //bv
