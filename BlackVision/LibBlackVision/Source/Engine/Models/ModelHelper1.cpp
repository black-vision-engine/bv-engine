#include "stdafx.h"
#include "ModelHelper.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Engine/Models/Timeline/TimelineManager.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"


#include "Memory/MemoryLeaks.h"



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
model::SimpleIntEvaluatorPtr      AddIntParam       ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const int & initVal, bool addState )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleIntEvaluator( paramName, timeEvaluator );
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



namespace SerializationHelper
{

// ***********************
//
model::ITimeEvaluatorPtr            GetDefaultTimeline      ( const IDeserializer & deser )
{
    bv::model::ITimeEvaluatorPtr timeEvaluator = nullptr;

    auto deserContext = Context( deser );

    if( deserContext == nullptr )
    {
        assert( false );
        return nullptr;
    }

    //bv::model::ITimeEvaluatorPtr sceneTimeline = deserContext->GetSceneTimeline();
    //if( sceneTimeline == nullptr )
    //{
    //    sceneTimeline = deserContext->GetRootTimeline();
    //}

    timeEvaluator = deserContext->GetTimeline( "default" );
    //if( timeEvaluator == nullptr ) 
    //{
    //    assert( false );
    //    timeEvaluator = sceneTimeline;
    //}

    return timeEvaluator;
}


} //SerializationHelper

} //bv

