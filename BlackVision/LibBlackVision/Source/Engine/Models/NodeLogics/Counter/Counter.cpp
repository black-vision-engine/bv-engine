#include "stdafx.h"

#include "Counter.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/NodeLogics/NodeLogicHelper.h"


namespace bv { namespace nodelogic {


const std::string   WidgetCounter::m_type = "counter";

const std::string   WidgetCounter::PARAMETERS::VALUE        = "value";
const std::string   WidgetCounter::PARAMETERS::PRECISION    = "precision";

	
// *******************************
//
WidgetCounterPtr	WidgetCounter::Create				( model::BasicNodeWeakPtr parent, model:: ITimeEvaluatorPtr timeEvaluator )
{
	return std::make_shared< WidgetCounter >( parent, timeEvaluator );
}

// *******************************
//
WidgetCounter::WidgetCounter( model::BasicNodeWeakPtr parent, model:: ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
{
    m_precision = AddIntParam( m_paramValModel, timeEvaluator, PARAMETERS::PRECISION, 3 )->Value();
    auto valueEval = AddFloatParam( m_paramValModel, timeEvaluator, PARAMETERS::VALUE, 0.0 );

    m_value = valueEval->Value();
    valueEval->Parameter()->SetGlobalCurveType( CurveType::CT_LINEAR );
}

// *******************************
//
WidgetCounter::~WidgetCounter()
{}

// *******************************
//
void		WidgetCounter::Update				( TimeType T )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        NodeLogicBase::Update( T );

        auto textPlugin = parentNode->GetPlugin( "text" );
        if( textPlugin )
        {
            float value = m_value->GetValue();
            int precision = m_precision->GetValue();
            if( precision < 0 )
                precision = 0;

            std::wstringstream converter;
            converter.precision( precision );
            converter << std::fixed << value;

            SetParameter( textPlugin->GetParameter( "text" ), 0.0, converter.str() );
        }
    }
}


// ***********************
//
void                WidgetCounter::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );

        ser.SetAttribute( "type", m_type );

        if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
        {
            NodeLogicBase::Serialize( ser );
        }

    ser.ExitChild();
}

// ***********************
//
WidgetCounterPtr     WidgetCounter::Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parent )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );

    auto newCounter = WidgetCounter::Create( parent, timeline );
    newCounter->Deserialize( deser );
    
    return newCounter;
}

// ***********************
//
bool                WidgetCounter::HandleEvent     ( IDeserializer& /*eventSer*/, ISerializer& response, BVProjectEditor * /*editor*/ )
{
    response.SetAttribute( ERROR_INFO_STRING, "This logic supports no commands." );
    return false;
}

// ***********************
//
const std::string &  WidgetCounter::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string & WidgetCounter::GetType             () const
{
    return Type();
}

} //nodelogic
} //bv