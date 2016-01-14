#include "Counter.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Types/Values/ValuesFactory.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include <string>
#include <iostream>
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "EndUserAPI/EventHandlers/EventHandlerHelpers.h"

namespace bv {
namespace widgets {

	
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
	m_value = ValuesFactory::CreateValueFloat("alpha" );
	m_isFinalized = true;
    m_param->SetCurveType( CurveType::CT_COSINE_LIKE );
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
void                WidgetCounter::Serialize       ( ISerializer& /*ser*/ ) const
{

}

// ***********************
//
ISerializablePtr     WidgetCounter::Create          ( const IDeserializer& /*deser*/ )
{
    return nullptr;
}

// ***********************
//
bool                WidgetCounter::HandleEvent     ( IDeserializer& eventSer, ISerializer& response )
{
    std::string param = eventSer.GetAttribute( "Param" );
    float value = SerializationHelper::String2T( eventSer.GetAttribute( "Value" ), 1.0f );
    float time = SerializationHelper::String2T( eventSer.GetAttribute( "Time" ), std::numeric_limits<float>::quiet_NaN() );


    if( time == std::numeric_limits<float>::quiet_NaN() )
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

    SetParameter( paramPtr, (bv::TimeType)time, value );

    return true;
}

}
}