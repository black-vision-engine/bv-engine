#include "stdafxBVApp.h"
#include "MaxSize.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/BoundingVolume.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Widgets/NodeLogicHelper.h"


namespace bv {
namespace nodelogic
{

const std::string       MaxSize::m_type = "MaxSize";

//const std::string       MaxSize::ACTION::ACTION_NAME        = "ActionName";

const std::string       MaxSize::PARAMETERS::MAX_HEIGHT = "MaxHeight";
const std::string       MaxSize::PARAMETERS::MAX_WIDTH  = "MaxWidth";
const std::string       MaxSize::PARAMETERS::MAX_DEPTH  = "MaxDepth";

const std::string       MaxSize::PARAMETERS::IS_PROPORTIONAL = "IsProportional";



// ***********************
//
const std::string &     MaxSize::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     MaxSize::GetType             () const
{
    return Type();
}

// ***********************
//
MaxSize::MaxSize             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( PARAMETERS::MAX_HEIGHT, 5.0f, true, false );
    h.AddSimpleParam( PARAMETERS::MAX_WIDTH, 5.0f, true, false );
    h.AddSimpleParam( PARAMETERS::MAX_DEPTH, 5.0f, true, false );

    h.AddSimpleParam( PARAMETERS::IS_PROPORTIONAL, false, true, false );

    m_paramValModel = std::static_pointer_cast<model::DefaultParamValModel>( h.GetModel()->GetPluginModel() );

    m_maxWidth  = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::MAX_WIDTH ) );
    m_maxHeight = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::MAX_HEIGHT ) );
    m_maxDepth  = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::MAX_DEPTH ) );

    m_isProportional = QueryTypedValue< ValueBoolPtr >( m_paramValModel->GetValue( PARAMETERS::IS_PROPORTIONAL ) );
}

// ***********************
//
MaxSize::~MaxSize()
{}


// ***********************
//
void                        MaxSize::PostChildrenUpdate ( TimeType t )
{
    if( auto node = m_parentNode.lock() )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();

		// If there isn't transform param in previuos plugin do nothing.
		if( transformParam )
		{
			auto bb = *node->GetBoundingVolume()->GetChildrenBox();

			float width = bb.Width();
			float height = bb.Height();
			float depth = bb.Depth();

			bool activeX = m_maxWidth->GetValue() != 0.0f ? true : false;
			bool activeY = m_maxHeight->GetValue() != 0.0f ? true : false;
			bool activeZ = m_maxDepth->GetValue() != 0.0f ? true : false;

			glm::vec3 curScale = transformParam->Transform().GetScale( 0.0f );
			glm::vec3 rescale = glm::vec3( activeX ? 1.0f : curScale.x, activeY ? 1.0f : curScale.y, activeZ ? 1.0f : curScale.z );

			bool needScaleX = width > m_maxWidth->GetValue() && activeX ? true : false;
			bool needScaleY = height > m_maxHeight->GetValue() && activeY ? true : false;
			bool needScaleZ = depth > m_maxDepth->GetValue() && activeZ ? true : false;

			if( needScaleX )
			{
				rescale.x = m_maxWidth->GetValue() / width;
			}

			if( needScaleY )
			{
				rescale.y = m_maxHeight->GetValue() / height;
			}

			if( needScaleZ )
			{
				rescale.z = m_maxDepth->GetValue() / depth;
			}

			if( m_isProportional->GetValue() )
			{
				float scaleFactor = std::min( rescale.x, rescale.y );
				scaleFactor = std::min( scaleFactor, rescale.z );

				if( activeX )
					rescale.x = scaleFactor;
				if( activeY )
					rescale.y = scaleFactor;
				if( activeZ )
					rescale.z = scaleFactor;
			}

			transformParam->SetScale( rescale, 0.0f );

            // Write new transformation data into values. This seems like hack. Calling update for the second time
            // in the same frame could cause something bad and depend on implementation of TransformPlugin.
            node->GetPlugin( "transform" )->Update( t );
		}
    }
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        MaxSize::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
    ser.SetAttribute( "type", m_type );

    if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
    {
        NodeLogicBase::Serialize( ser );
    }

    ser.ExitChild();    // logic
}

// ***********************
//
MaxSizePtr              MaxSize::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< MaxSize >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        MaxSize::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

//    if( action == MaxSize::ACTION::ACTION_NAME )
//    {
//        return false
//    }

    return false;
}

}   // nodelogic
}	// bv
