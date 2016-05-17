#include "stdafx.h"
#include "ModelHelper.h"


namespace bv { namespace model {

// ***********************
//
model::SimpleFloatEvaluatorPtr      AddFloatParam       ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const float & initVal, bool addState )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );

    if( addState )
        AddState( paramModel, paramEval->Value(), paramName );

    return paramEval;
}

// ***********************
//
model::SimpleBoolEvaluatorPtr       AddBoolParam        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const bool & initVal, bool addState )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );
    
    if( addState )
        AddState( paramModel, paramEval->Value(), paramName );

    return paramEval;
}

// ***********************
//
model::SimpleVec2EvaluatorPtr       AddVec2Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec2 & initVal, bool addState )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );
    
    if( addState )
        AddState( paramModel, paramEval->Value(), paramName );

    return paramEval;
}

// ***********************
//
model::SimpleVec3EvaluatorPtr       AddVec3Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec3 & initVal, bool addState )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );
    
    if( addState )
        AddState( paramModel, paramEval->Value(), paramName );

    return paramEval;
}

// ***********************
//
model::SimpleVec4EvaluatorPtr       AddVec4Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec4 & initVal, bool addState )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );
    
    if( addState )
        AddState( paramModel, paramEval->Value(), paramName );

    return paramEval;
}

} // model
} //bv

