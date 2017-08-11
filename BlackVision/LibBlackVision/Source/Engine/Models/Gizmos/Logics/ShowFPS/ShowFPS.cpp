#include "stdafx.h"
#include "ShowFPS.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Models/ModelState.h"
#include "Engine/Editors/BVProjectEditor.h"


#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"



namespace bv {
namespace nodelogic
{

const std::string       ShowFPS::m_type = "ShowFPS";


const std::string       ShowFPS::PARAMETERS::REFRESH_FREQUENCY = "RefreshFrequency";


// ***********************
//
const std::string &     ShowFPS::Type            ()
{
    return m_type;
}


// ***********************
//
const std::string &     ShowFPS::GetType             () const
{
    return Type();
}

// ***********************
//
ShowFPS::ShowFPS             ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator )
    :   GizmoLogicBase( gizmoRoot, gizmoOwner )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( PARAMETERS::REFRESH_FREQUENCY, (Int32)100, true, true );

    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );

    m_refreshFrequency = model::GetValueParamState< Int32 >( m_paramValModel.get(), PARAMETERS::REFRESH_FREQUENCY );
}

// ***********************
//
ShowFPS::~ShowFPS()
{}


// ***********************
//
void        ShowFPS::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &ShowFPS::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        ShowFPS::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &ShowFPS::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        ShowFPS::Update			( TimeType t )
{
    GizmoLogicBase::Update( t );
}

// ========================================================================= //
// Gizmo subtree creation
// ========================================================================= //

// ***********************
//
void                        ShowFPS::CreateGizmoSubtree ( BVProjectEditor * editor )
{
    if( auto gizmoOwner = m_gizmoOwner.lock() )
    {
        auto gizmoRoot = m_gizmoRoot.lock();
        auto scene = editor->GetModelScene( model::ModelState::GetInstance().QueryNodeScene( gizmoOwner.get() )->GetName() );
        auto timeEvaluator = m_refreshFrequency.GetParameter().GetTimeEvaluator();

        model::BasicNodePtr fpsLabel = model::BasicNode::Create( "fpsLabel", timeEvaluator );
        model::BasicNodePtr fpsValue = model::BasicNode::Create( "fpsValue", timeEvaluator );

        fpsLabel->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
        fpsLabel->AddPlugin( "DEFAULT_TEXT", timeEvaluator );

        SetTranslation( fpsLabel, glm::vec3( -0.5, 0.0, 0.0 ) );
        SetText( fpsLabel, L"FPS" );


        fpsValue->AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );
        fpsValue->AddPlugin( "DEFAULT_TEXT", timeEvaluator );

        SetTranslation( fpsValue, glm::vec3( 0.0, 0.0, 0.0 ) );
        SetText( fpsValue, L"0.0" );

        editor->AddChildNode( scene, gizmoRoot, fpsLabel, false );
        editor->AddChildNode( scene, gizmoRoot, fpsValue, false );

        m_fpsNode = fpsValue;
    }
}


// ***********************
//
void                        ShowFPS::SetTranslation     ( model::BasicNodePtr node, const glm::vec3 & transform, TimeType time )
{
    auto nodeTransform = node->GetFinalizePlugin()->GetParamTransform();
    nodeTransform->SetTranslation( transform, time );
}

// ***********************
//
void                        ShowFPS::SetText            ( model::BasicNodePtr node, const std::wstring & text, TimeType time )
{
    auto nodeTextPlugin = node->GetPlugin( "text" );
    auto textParam = model::QueryTypedParam< model::ParamWStringPtr >( nodeTextPlugin->GetParameter( "text" ) );
    textParam->SetVal( text, time );
}


// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        ShowFPS::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    //if( auto parentNode = m_parentNode.lock() )
    //{

    //}
}


}   // nodelogic
}	// bv

