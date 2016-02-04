#include "Counter.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Types/Values/ValuesFactory.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include <string>
#include <iostream>
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Serialization/BVDeserializeContext.h"

#include "EndUserAPI/EventHandlers/EventHandlerHelpers.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

namespace bv {
namespace nodelogic {

	
// *******************************
//
WidgetCounterPtr	WidgetCounter::Create				( bv::model::BasicNode * parent,bv::model:: ITimeEvaluatorPtr timeEvaluator)
{
	return std::make_shared< WidgetCounter >( parent,timeEvaluator );
	

}

// ****************************
//
bv::model::IParameterPtr        WidgetCounter::GetValueParam       ()
{
    return m_param;
}


WidgetCounter::WidgetCounter(bv::model::BasicNode * parent,bv::model:: ITimeEvaluatorPtr timeEvaluator): m_parentNode( parent )
{
	m_param = bv::model::ParametersFactory::CreateParameterFloat( "alpha", timeEvaluator );
    SetParameter( m_param, (bv::TimeType)0.0f, 0.0f );
    //SetParameter( m_param, (bv::TimeType)8.0f, 20.0f );

	m_value = ValuesFactory::CreateValueFloat("alpha" );
	m_isFinalized = true;

    //m_param->SetCurveType( CurveType::CT_COSINE_LIKE );
    m_param->SetAddedKeyCurveType( CurveType::CT_LINEAR );
}


WidgetCounter::~WidgetCounter(void)
{
}


// *******************************
//
void		WidgetCounter::Update				( TimeType T)
{
	{T;}
	 m_value->SetValue( m_param->Evaluate() );
	 //printf( "counter %f - %f - %f\r\n", T, m_param->Evaluate(),m_value->GetValue());
	 bv::model::DefaultTextPlugin* txt = dynamic_cast< bv::model::DefaultTextPlugin* >( m_parentNode->GetPlugin("text").get() );
	 std::wstring text;
	 wchar_t buffer[12];
	 swprintf(buffer,12,L"%.1f",m_value->GetValue());
	 text = std::wstring(buffer);
	 if( txt != nullptr )
	 {
         SetParameter( m_parentNode->GetPlugin("text")->GetParameter( "text" ), 0.0, text );
	 }
}


// ***********************
//
void                WidgetCounter::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "logic" );

        ser.SetAttribute( "type", "counter" );
        m_param->Serialize( ser );

        auto timeline = bv::model::TimelineManager::GetInstance()->GetTimelinePath( m_param->GetTimeEvaluator() );
        ser.SetAttribute( "timelinePath", timeline );

    ser.ExitChild();
}

// ***********************
//
WidgetCounterPtr     WidgetCounter::Create          ( const IDeserializer& deser, bv::model::BasicNode * parent )
{
    bv::model::ITimeEvaluatorPtr timeEvaluator = nullptr;

    std::string timeline = deser.GetAttribute( "timelinePath" );

    assert( timeline != "" );

    if( timeline != "" )
    {
        auto deserContext = static_cast< BVDeserializeContext * >( deser.GetDeserializeContext() );

        if( deserContext == nullptr )
        {
            return nullptr;
        }

        bv::model::ITimeEvaluatorPtr sceneTimeline = deserContext->GetSceneTimeline();
        if( sceneTimeline == nullptr )
        {
            sceneTimeline = bv::model::TimelineManager::GetInstance()->GetRootTimeline();
        }

        timeEvaluator = bv::model::TimelineHelper::GetTimeEvaluator( timeline, sceneTimeline );
        if( timeEvaluator == nullptr ) 
        {
            assert( false );
            timeEvaluator = sceneTimeline;
        }
    }

    auto newCounter = WidgetCounter::Create( parent, timeEvaluator );
    
    if( deser.EnterChild( "param" ) )
    {
        auto oldParam = std::static_pointer_cast<model::ParamFloat>( newCounter->GetValueParam() );
        auto newParam = std::static_pointer_cast<model::ParamFloat>( model::AbstractModelParameter::Create( deser ) );

        
        newParam->SetTimeEvaluator( oldParam->GetTimeEvaluator() );
        newParam->AccessInterpolator() = oldParam->AccessInterpolator();
    }

    return newCounter;
}

// ***********************
//
bool                WidgetCounter::HandleEvent     ( IDeserializer& eventSer, ISerializer& response )
{
    float value = SerializationHelper::String2T( eventSer.GetAttribute( "Value" ), 1.0f );
    float time = SerializationHelper::String2T( eventSer.GetAttribute( "Time" ), std::numeric_limits<float>::quiet_NaN() );

    std::string action = eventSer.GetAttribute( "Action" );

    if( time != time )      // Checks if time is NaN. @note comparision time == std::numeric_limits<float>::quiet_NaN() doesn't work.
    {
        response.SetAttribute( ERROR_INFO_STRING, "Not valid time." );
        return false;
    }

	auto paramPtr = this->GetValueParam();
	if( paramPtr == nullptr )
    {
        response.SetAttribute( ERROR_INFO_STRING, "Could not get parameter" );
        return false;
    }

    if( action == "SetParam" )
        SetParameter( paramPtr, (bv::TimeType)time, value );
    else if( action == "RemoveParam" )
        RemoveParameterKey( paramPtr, (bv::TimeType)time );
    else
        return false;

    return true;
}

}
}