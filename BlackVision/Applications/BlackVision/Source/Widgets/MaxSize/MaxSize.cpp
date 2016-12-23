#include "stdafxBVApp.h"
#include "MaxSize.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/NodeRemovedEvent.h"
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
void                        MaxSize::Update			( TimeType t )
{
    NodeLogicBase::Update( t );

    if( auto node = m_parentNode.lock() )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();

        auto bb = node->GetBoundingVolume()->GetBoundingBox();
        
        float width = bb->Width();
        float height = bb->Height();
        float depth = bb->Depth();

        glm::vec3 rescale = transformParam->Transform().GetScale( 0.0f );

        bool needScaleX = width > m_maxWidth->GetValue() && m_maxWidth->GetValue() != 0.0f ? true : false;
        bool needScaleY = height > m_maxHeight->GetValue() && m_maxHeight->GetValue() != 0.0f ? true : false;
        bool needScaleZ = depth > m_maxDepth->GetValue() && m_maxDepth->GetValue() != 0.0f ? true : false;

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
            float scaleFactor = std::max( rescale.x, rescale.y );
            scaleFactor = std::max( scaleFactor, rescale.z );

            if( m_maxWidth->GetValue() != 0.0f )
                rescale.x = scaleFactor;
            if( m_maxHeight->GetValue() != 0.0f )
                rescale.y = scaleFactor;
            if( m_maxDepth->GetValue() != 0.0f )
                rescale.z = scaleFactor;
        }
        
        transformParam->SetScale( rescale, 0.0f );
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
