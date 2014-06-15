#pragma once

#include <string>

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.h"
#include "Engine/Models/Plugins/ParamValModel/TransformVecParamValEvaluator.h"


namespace bv { namespace model {

class ParamValEvaluatorFactory
{
public:

    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( ParamFloat * param, ValueFloat * value );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( ParamFloat * param, const std::string & valueName );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( ParamFloat * param );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( ParamVec4 * param, ValueVec4 * value );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( ParamVec4 * param, const std::string & valueName );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( ParamVec4 * param );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( ParamVec3 * param, ValueVec3 * value );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( ParamVec3 * param, const std::string & valueName );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( ParamVec3 * param );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( ParamMat2 * param, ValueMat2 * value );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( ParamMat2 * param, const std::string & valueName );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( ParamMat2 * param );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( ParamTransform * param, ValueMat4 * value );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( ParamTransform * param, const std::string & valueName );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( ParamTransform * param );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, ValueMat4 * value );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( ParamTransformVec * param, ValueMat4PtrVec values );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( ParamTransformVec * param, const std::string & valueName );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( ParamTransformVec * param, const std::vector< std::string > & valueNames );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( ParamTransformVec * param );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, unsigned int numTransformations );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName, unsigned int numTransformations );
    static TransformVecParamValEvaluatorPtr  CreateTransformVecEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::vector< std::string > & valueNames );

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
