#include "stdafx.h"
#include "BoundingBoxLogic.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"


#include "Engine/Models/ModelState.h"
#include "Engine/Editors/BVProjectEditor.h"




namespace bv {
namespace nodelogic
{

const std::string       BoundingBoxLogic::m_type = "BoundingBoxLogic";


const std::string       BoundingBoxLogic::PARAMETERS::BOX_COLOR         = "BoxColor";
const std::string       BoundingBoxLogic::PARAMETERS::CENTER_COLOR      = "CenterColor";
const std::string       BoundingBoxLogic::PARAMETERS::CENTER_SIZE       = "CenterSize";



// ***********************
//
const std::string &     BoundingBoxLogic::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     BoundingBoxLogic::GetType             () const
{
    return Type();
}

// ***********************
//
BoundingBoxLogic::BoundingBoxLogic             ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator )
    : GizmoLogicBase( gizmoRoot, gizmoOwner )
{
    model::ModelHelper h( timeEvaluator );
    
    h.SetOrCreatePluginModel();
    h.AddSimpleParam( PARAMETERS::CENTER_COLOR, glm::vec4( 1.0, 0.0, 0.0, 1.0 ), true, true );
    h.AddSimpleParam( PARAMETERS::BOX_COLOR, glm::vec4( 1.0, 0.0, 0.0, 1.0 ), true, true );
    h.AddSimpleParam( PARAMETERS::CENTER_SIZE, 12.0f, true, true );

    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );

    m_boxColor      = model::GetValueParamState< glm::vec4 >( m_paramValModel.get(), PARAMETERS::BOX_COLOR );
    m_centerColor   = model::GetValueParamState< glm::vec4 >( m_paramValModel.get(), PARAMETERS::CENTER_COLOR );
    m_centerSize    = model::GetValueParamState< float >( m_paramValModel.get(), PARAMETERS::CENTER_SIZE );
}

// ***********************
//
BoundingBoxLogic::~BoundingBoxLogic()
{}


// ***********************
//
void        BoundingBoxLogic::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BoundingBoxLogic::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        BoundingBoxLogic::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BoundingBoxLogic::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        BoundingBoxLogic::Update			( TimeType t )
{
    GizmoLogicBase::Update( t );

    if( auto ownerNode = m_gizmoOwner.lock() )
    {


        //glm::vec3 center = ownerNode->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetCenter( 0.0f );


        //auto boundingVolume = ownerNode->GetBoundingVolume();
        //if( boundingVolume )
        //{
        //    auto box = boundingVolume->GetBoundingBox();


        //}
    }
}

// ========================================================================= //
// Gizmo subtree creation
// ========================================================================= //

// ***********************
//
void                        BoundingBoxLogic::CreateGizmoSubtree ( BVProjectEditor * editor )
{
    if( auto gizmoOwner = m_gizmoOwner.lock() )
    {
        auto gizmoRoot = m_gizmoRoot.lock();
        auto scene = editor->GetModelScene( model::ModelState::GetInstance().QueryNodeScene( gizmoOwner.get() )->GetName() );
        auto timeEvaluator = m_centerColor.GetParameter().GetTimeEvaluator();

        model::BasicNodePtr boxNode = model::BasicNode::Create( "box", timeEvaluator );
        model::BasicNodePtr centerNode = model::BasicNode::Create( "center", timeEvaluator );

        centerNode->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
        centerNode->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
        centerNode->AddPlugin( "CENTER_PLUGIN", timeEvaluator );

        SetColor( centerNode, m_centerColor.GetValue() );
        SetCenterSize( centerNode, m_centerSize.GetValue() );

        boxNode->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
        boxNode->AddPlugin( "DEFAULT_COLOR", timeEvaluator );
        //boxNode->AddPlugin( "BOUNDING_BOX_PLUGIN", timeEvaluator );

        SetColor( centerNode, m_boxColor.GetValue() );


        editor->AddChildNode( scene, gizmoRoot, centerNode, false );
        editor->AddChildNode( scene, gizmoRoot, boxNode, false );
    }
}

// ***********************
//
void                        BoundingBoxLogic::SetTranslation    ( model::BasicNodePtr node, const glm::vec3 & transform, TimeType time )
{
    auto nodeTransform = node->GetFinalizePlugin()->GetParamTransform();
    nodeTransform->SetTranslation( transform, time );
}

// ***********************
//
void                        BoundingBoxLogic::SetColor          ( model::BasicNodePtr node, const glm::vec4 & color, TimeType time )
{
    auto colorPlugin = node->GetPlugin( "solid color" );
    auto colorParam = model::QueryTypedParam< model::ParamVec4Ptr >( colorPlugin->GetParameter( "color" ) );
    colorParam->SetVal( color, time );
}

// ***********************
//
void                        BoundingBoxLogic::SetCenterSize     ( model::BasicNodePtr node, float size, TimeType time )
{
    auto centerPlugin = node->GetPlugin( "center" );
    auto sizeParam = model::QueryTypedParam< model::ParamFloatPtr >( centerPlugin->GetParameter( "size" ) );
    sizeParam->SetVal( size, time );
}


// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        BoundingBoxLogic::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;
}

}   // nodelogic
}	// bv

