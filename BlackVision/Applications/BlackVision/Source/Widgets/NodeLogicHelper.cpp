#include "NodeLogicHelper.h"
#include "Engine/Types/Values/ValuesFactory.h"
#include "Engine/Models/Plugins/Parameters/ParameterSerialization.h"


#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Engine/Models/Timeline/TimelineManager.h"

#include "Tools/StringHeplers.h"

namespace bv
{
namespace SerializationHelper
{


// ========================================================================= //
// IValue serialization
// ========================================================================= //
// In general values shouldn't be serialize. There're few cases, in which 
// serialization is needed ( NodeReplicator NodeLogic )
// Thats the main reason, why it wasn't done by inheiting from ISerializable.


// ***********************
//
IValuePtr CreateValue( const IDeserializer & deser, const std::string & name )
{
    auto valType = String2T( deser.GetAttribute( "type" ), ParamType::PT_FLOAT1 );

    switch( valType )
    {
    case ParamType::PT_FLOAT1:
    {
        auto iValuePtr = ValuesFactory::CreateValueFloat( name );
        float value = SerializationHelper::String2T( deser.GetAttribute( "value" ), 0.f );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_FLOAT2:
    {
        auto iValuePtr = ValuesFactory::CreateValueVec2( name );
        glm::vec2 value = SerializationHelper::String2T( deser.GetAttribute( "value" ), glm::vec2( 0.f ) );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_FLOAT3:
    {
        auto iValuePtr = ValuesFactory::CreateValueVec3( name );
        glm::vec3 value = SerializationHelper::String2T( deser.GetAttribute( "value" ), glm::vec3( 0.f ) );
        iValuePtr->SetValue( value );
        return iValuePtr;

    }
    case ParamType::PT_FLOAT4:
    {
        auto iValuePtr = ValuesFactory::CreateValueVec4( name );
        glm::vec4 value = SerializationHelper::String2T( deser.GetAttribute( "value" ), glm::vec4( 0.f ) );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_INT:
    {
        auto iValuePtr = ValuesFactory::CreateValueInt( name );
        int value = SerializationHelper::String2T( deser.GetAttribute( "value" ), 0 );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_MAT2:
    {
        assert( !"Implement" );
        auto iValuePtr = ValuesFactory::CreateValueMat2( name );
        return iValuePtr;
    }
    case ParamType::PT_MAT4:
    {
        assert( !"Implement" );
        auto iValuePtr = ValuesFactory::CreateValueMat4( name );
        return iValuePtr;
    }
    case ParamType::PT_STRING:
    {
        auto iValuePtr = ValuesFactory::CreateValueString( name );
        std::string value = deser.GetAttribute( "value" );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_WSTRING:
    {
        auto iValuePtr = ValuesFactory::CreateValueWString( name );
        std::wstring value = StringToWString( deser.GetAttribute( "value" ) );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    default:
        assert( !"Implement" );
        return nullptr;
    }
}


// ***********************
//
void                SerializeValue                      ( ISerializer & ser, IValueConstPtr val )
{
    auto valType = val->GetType();
    ser.SetAttribute( "type", SerializationHelper::T2String( valType ) );

    switch( valType )
    {
    case ParamType::PT_FLOAT1:
    {
        auto iValuePtr = QueryTypedValue< ValueFloatPtr> ( val );
        float value = iValuePtr->GetValue();
        ser.SetAttribute( "value", SerializationHelper::T2String( value ) );
        break;
    }
    case ParamType::PT_FLOAT2:
    {
        auto iValuePtr = QueryTypedValue< ValueVec2Ptr> ( val );
        glm::vec2 value = iValuePtr->GetValue();
        ser.SetAttribute( "value", SerializationHelper::T2String( value ) );
        break;
    }
    case ParamType::PT_FLOAT3:
    {
        auto iValuePtr = QueryTypedValue< ValueVec3Ptr> ( val );
        glm::vec3 value = iValuePtr->GetValue();
        ser.SetAttribute( "value", SerializationHelper::T2String( value ) );
        break;
    }
    case ParamType::PT_FLOAT4:
    {
        auto iValuePtr = QueryTypedValue< ValueVec4Ptr> ( val );
        glm::vec4 value = iValuePtr->GetValue();
        ser.SetAttribute( "value", SerializationHelper::T2String( value ) );
        break;
    }
    case ParamType::PT_INT:
    {
        auto iValuePtr = QueryTypedValue< ValueIntPtr> ( val );
        int value = iValuePtr->GetValue();
        ser.SetAttribute( "value", SerializationHelper::T2String( value ) );
        break;
    }
    case ParamType::PT_MAT2:
    {
        assert( !"Implement" );
        break;
    }
    case ParamType::PT_MAT4:
    {
        assert( !"Implement" );
        break;
    }
    case ParamType::PT_STRING:
    {
        auto iValuePtr = QueryTypedValue< ValueStringPtr> ( val );
        std::string value = iValuePtr->GetValue();
        ser.SetAttribute( "value", value );
        break;
    }
    case ParamType::PT_WSTRING:
    {
        auto iValuePtr = QueryTypedValue< ValueWStringPtr> ( val );
        std::wstring value = iValuePtr->GetValue();
        ser.SetAttribute( "value", SerializationHelper::T2String( value ) );
        break;
    }
    default:
        assert( !"Implement" );
    }

}

// ***********************
//
model::ITimeEvaluatorPtr            GetDefaultTimeline      ( const IDeserializer & deser )
{
    bv::model::ITimeEvaluatorPtr timeEvaluator = nullptr;


    auto deserContext = static_cast< BVDeserializeContext * >( deser.GetDeserializeContext() );

    if( deserContext == nullptr )
    {
        assert( false );
        return nullptr;
    }

    bv::model::ITimeEvaluatorPtr sceneTimeline = deserContext->GetSceneTimeline();
    if( sceneTimeline == nullptr )
    {
        sceneTimeline = bv::model::TimelineManager::GetInstance()->GetRootTimeline();
    }

    timeEvaluator = bv::model::TimelineHelper::GetTimeEvaluator( "default", sceneTimeline );
    if( timeEvaluator == nullptr ) 
    {
        assert( false );
        timeEvaluator = sceneTimeline;
    }

    return timeEvaluator;
}

} // SerializationHelper

// ***********************
//
model::SimpleFloatEvaluatorPtr      AddFloatParam       ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const float & initVal )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );

    return paramEval;
}

// ***********************
//
model::SimpleBoolEvaluatorPtr       AddBoolParam        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const bool & initVal )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );

    return paramEval;
}

// ***********************
//
model::SimpleVec2EvaluatorPtr       AddVec2Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec2 & initVal )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );

    return paramEval;
}

// ***********************
//
model::SimpleVec3EvaluatorPtr       AddVec3Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec3 & initVal )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );

    return paramEval;
}

// ***********************
//
model::SimpleVec4EvaluatorPtr       AddVec4Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec4 & initVal )
{
    auto paramEval = model::ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( paramName, timeEvaluator );
    paramEval->Parameter()->SetVal( initVal, TimeType( 0.0f ) );
    paramModel->RegisterAll( paramEval );

    return paramEval;
}

} // bv

