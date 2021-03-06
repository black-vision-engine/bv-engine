#pragma once

#include <string>

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.h"


namespace bv { namespace model {

class ParamValEvaluatorFactory
{
public:
    template< typename InterpolatorType, typename ValueType, ModelParamType MPT, ParamType PT, typename ParamImpl >
    static IParamValEvaluatorPtr        CreateSimpleEvaluator           ( std::shared_ptr< ParamImpl > param )
    {
        auto value = std::make_shared< ValueImpl< ValueType, PT > >( param->GetName() );
        return std::make_shared< SimpleParamValEvaluator< 
            std::shared_ptr< ParamImpl >, 
            std::shared_ptr< ValueImpl< ValueType, PT > >
        > >( param, value );
    }

    static SimpleBoolEvaluatorPtr       CreateSimpleBoolEvaluator       ( ParamBoolPtr param, ValueBoolPtr value );
    static SimpleBoolEvaluatorPtr       CreateSimpleBoolEvaluator       ( ParamBoolPtr param, const std::string & valueName );
    static SimpleBoolEvaluatorPtr       CreateSimpleBoolEvaluator       ( ParamBoolPtr param );
    static SimpleBoolEvaluatorPtr       CreateSimpleBoolEvaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleBoolEvaluatorPtr       CreateSimpleBoolEvaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleIntEvaluatorPtr        CreateSimpleIntEvaluator        ( ParamIntPtr param, ValueIntPtr value );
    static SimpleIntEvaluatorPtr        CreateSimpleIntEvaluator        ( ParamIntPtr param, const std::string & valueName );
    static SimpleIntEvaluatorPtr        CreateSimpleIntEvaluator        ( ParamIntPtr param );
    static SimpleIntEvaluatorPtr        CreateSimpleIntEvaluator        ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleIntEvaluatorPtr        CreateSimpleIntEvaluator        ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( ParamFloatPtr param, ValueFloatPtr value );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( ParamFloatPtr param, const std::string & valueName );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( ParamFloatPtr param );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleFloatEvaluatorPtr      CreateSimpleFloatEvaluator      ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( ParamVec4Ptr param, ValueVec4Ptr value );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( ParamVec4Ptr param, const std::string & valueName );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( ParamVec4Ptr param );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleVec4EvaluatorPtr       CreateSimpleVec4Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( ParamVec3Ptr param, ValueVec3Ptr value );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( ParamVec3Ptr param, const std::string & valueName );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( ParamVec3Ptr param );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleVec3EvaluatorPtr       CreateSimpleVec3Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleVec2EvaluatorPtr       CreateSimpleVec2Evaluator       ( ParamVec2Ptr param, ValueVec2Ptr value );
    static SimpleVec2EvaluatorPtr       CreateSimpleVec2Evaluator       ( ParamVec2Ptr param, const std::string & valueName );
    static SimpleVec2EvaluatorPtr       CreateSimpleVec2Evaluator       ( ParamVec2Ptr param );
    static SimpleVec2EvaluatorPtr       CreateSimpleVec2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleVec2EvaluatorPtr       CreateSimpleVec2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( ParamMat2Ptr param, ValueMat2Ptr value );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( ParamMat2Ptr param, const std::string & valueName );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( ParamMat2Ptr param );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleMat2EvaluatorPtr       CreateSimpleMat2Evaluator       ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( ParamTransformPtr param, ValueMat4Ptr value );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( ParamTransformPtr param, const std::string & valueName );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( ParamTransformPtr param );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );
    static SimpleTransformEvaluatorPtr  CreateSimpleTransformEvaluator  ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, ValueMat4Ptr value );

    static SimpleStringEvaluatorPtr         CreateSimpleStringEvaluator ( ParamStringPtr param, ValueStringPtr value );
    static SimpleStringEvaluatorPtr         CreateSimpleStringEvaluator ( ParamStringPtr param, const std::string & valueName );
    static SimpleStringEvaluatorPtr         CreateSimpleStringEvaluator ( ParamStringPtr param );
    static SimpleStringEvaluatorPtr         CreateSimpleStringEvaluator ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleStringEvaluatorPtr         CreateSimpleStringEvaluator ( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );

    static SimpleWStringEvaluatorPtr        CreateSimpleWStringEvaluator( ParamWStringPtr param, ValueWStringPtr value );
    static SimpleWStringEvaluatorPtr        CreateSimpleWStringEvaluator( ParamWStringPtr param, const std::string & valueName );
    static SimpleWStringEvaluatorPtr        CreateSimpleWStringEvaluator( ParamWStringPtr param );
    static SimpleWStringEvaluatorPtr        CreateSimpleWStringEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator );
    static SimpleWStringEvaluatorPtr        CreateSimpleWStringEvaluator( const std::string & paramName, ITimeEvaluatorPtr timeEvaluator, const std::string & valueName );
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
