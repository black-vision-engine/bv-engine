#include "stdafx.h"
#include "Cloner.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Models/ModelState.h"
#include "Engine/Models/BVProject.h"
#include "Engine/Models/BVProjectEditor.h"

namespace bv
{
namespace nodelogic
{

const std::string       Cloner::m_type = "Cloner";

//const std::string       Cloner::ACTION::ACTION_NAME        = "ActionName";

const std::string       Cloner::PARAMETERS::N_ROWS  = "numRows";
const std::string       Cloner::PARAMETERS::N_COLS  = "numCols";
const std::string       Cloner::PARAMETERS::DELTA   = "delta";

// ***********************
//
const std::string &     Cloner::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     Cloner::GetType             () const
{
    return Type();
}

// ***********************
//
Cloner::Cloner             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( PARAMETERS::N_ROWS, 1, true, true );
    h.AddSimpleParam( PARAMETERS::N_COLS, 1, true, true );
    h.AddSimpleParam( PARAMETERS::DELTA, glm::vec3( 0.f, 0.f, 0.f ), true, true );


    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );

    m_numRows = model::GetValueParamState< Int32 >( m_paramValModel.get(), PARAMETERS::N_ROWS );
    m_numCols = model::GetValueParamState< Int32 >( m_paramValModel.get(), PARAMETERS::N_COLS );
    m_delta = model::GetValueParamState< glm::vec3 >( m_paramValModel.get(), PARAMETERS::DELTA );
}

// ***********************
//
Cloner::~Cloner()
{}


// ***********************
//
void        Cloner::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &Cloner::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        Cloner::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &Cloner::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        Cloner::Update			( TimeType t )
{
    NodeLogicBase::Update( t );

    UpdateClones();
    UpdatePositions();
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        Cloner::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext* >( ser.GetSerializeContext() );
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
ClonerPtr              Cloner::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Cloner >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Cloner::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    //    if( action == Cloner::ACTION::ACTION_NAME )
    //    {
    //        return false
    //    }

    return false;
}

// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        Cloner::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    if( auto parentNode = m_parentNode.lock() )
    {
        
    }
}

// ***********************
//
void                        Cloner::UpdateClones        ()
{
    if( ParameterChanged( PARAMETERS::N_ROWS ) ||
        ParameterChanged( PARAMETERS::N_COLS ) )
    {
        if( auto parentNode = m_parentNode.lock() )
        {
            auto missingNum = m_numCols.GetValue() * m_numRows.GetValue() - ( UInt32 )parentNode->GetNumChildren();
            
            if( missingNum > 0 )
                CloneNode( missingNum );
        }
    }
}

// ***********************
//
void                        Cloner::UpdatePositions     ()
{
    if( ParameterChanged( PARAMETERS::DELTA ) )
    {
        if( auto parentNode = m_parentNode.lock() )
        {
            if( parentNode->GetNumChildren() > 0 )
            {
                auto firstChild = parentNode->GetChild( 0 );

                auto paramTransformFirst = firstChild->GetFinalizePlugin()->GetParamTransform();

                glm::vec3 translation = glm::vec3( 0.f );

                if( paramTransformFirst )
                    translation = paramTransformFirst->GetTransform().GetTranslation( 0.f );

                auto numCols = m_numCols.GetValue();
                auto numRows = m_numRows.GetValue();
                auto delta = m_delta.GetValue();

                for( UInt32 i = 1; i < parentNode->GetNumChildren(); ++i )
                {
                    auto paramTransform = parentNode->GetChild( i )->GetFinalizePlugin()->GetParamTransform();

                    auto r = i / numRows;
                    auto c = i / numCols;

                    if( paramTransform )
                        paramTransform->SetTranslation( translation + glm::vec3( delta.x * c, delta.y * r, delta.z * c ), 0.f );
                }
            }
        }
    }
}

// ***********************
//
void                        Cloner::CloneNode           ( UInt32 clonesNum ) const
{
    if( auto parentNode = m_parentNode.lock() )
    {
        auto & modelState = model::ModelState::GetInstance();

        auto sceneName = modelState.QueryNodeScene( parentNode.get() )->GetName();

        auto scene = modelState.GetBVProject()->GetModelScene( sceneName );

        auto projectEditor = modelState.GetBVProject()->GetProjectEditor();

        if( parentNode->GetNumChildren() > 0 )
        {
            auto firstChild = parentNode->GetChild( 0 );

            for( UInt32 i = 0; i < clonesNum; ++i )
                projectEditor->AddNodeCopy( scene, parentNode, scene, firstChild, false );
        }
    }
}

}   // nodelogic
}	// bv
