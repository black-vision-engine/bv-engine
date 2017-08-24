#include "stdafx.h"
#include "GridLines.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Models/ModelState.h"
#include "Engine/Editors/BVProjectEditor.h"

#include "Engine/Models/GridLines/GridLinesLogic.h"

#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "Assets/DataArray/DataArrayTypedRows.h"



namespace bv {
namespace nodelogic
{

const std::string       GridLines::m_type = "GridLines";

const std::string       GridLines::PARAMETERS::LINES_COLOR      = "LinesColor";
const std::string       GridLines::PARAMETERS::LINES_LENGTH     = "LinesLength";


// ***********************
//
const std::string &     GridLines::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     GridLines::GetType             () const
{
    return Type();
}

// ***********************
//
GridLines::GridLines             ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator )
    : GizmoLogicBase( gizmoRoot, gizmoOwner )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( PARAMETERS::LINES_COLOR, glm::vec4( 1.0, 0.7, 0.0, 0.8 ), true, true );
    h.AddSimpleParam( PARAMETERS::LINES_LENGTH, 20.0f, true, true );

    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );

    m_linesColor    = model::GetValueParamState< glm::vec4 >( m_paramValModel.get(), PARAMETERS::LINES_COLOR );
    m_linesLength   = model::GetValueParamState< float >( m_paramValModel.get(), PARAMETERS::LINES_LENGTH );
}

// ***********************
//
GridLines::~GridLines()
{}

// ***********************
//
model::IGizmoLogicPtr       GridLines::Create   ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator )
{
    return std::make_shared< GridLines >( gizmoRoot, gizmoOwner, timeEvaluator );
}

// ***********************
//
void        GridLines::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &GridLines::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        GridLines::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &GridLines::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        GridLines::Update			( TimeType t )
{
    GizmoLogicBase::Update( t );

    if( auto ownerNode = m_gizmoOwner.lock() )
    {
        if( m_linesColor.Changed() )
            SetColor( m_gizmoRoot.lock(), m_linesColor.GetValue() );

        if( PropagateUpdate() )
            UpdateLines();
    }
}

// ========================================================================= //
// Gizmo subtree creation
// ========================================================================= //

// ***********************
//
void                        GridLines::CreateGizmoSubtree ( BVProjectEditor * editor )
{
    if( auto gizmoOwner = m_gizmoOwner.lock() )
    {
        auto gizmoRoot = m_gizmoRoot.lock();
        auto scene = editor->GetModelScene( model::ModelState::GetInstance().QueryNodeScene( gizmoOwner.get() )->GetName() );
        auto timeEvaluator = m_linesColor.GetParameter().GetTimeEvaluator();

        m_gridLinesLogic = &scene->GetGridLinesLogic();
        m_updateID = m_gridLinesLogic->GetUpdateID();

        UInt32 idx = 1;
        editor->AddPlugin( gizmoRoot, model::PluginsManager::DefaultInstanceRef().CreatePlugin( "LINES_PLUGIN", timeEvaluator ), idx );
        editor->AddPlugin( gizmoRoot, model::PluginsManager::DefaultInstanceRef().CreatePlugin( "DEFAULT_COLOR", timeEvaluator ), ++idx );
        
        SetColor( gizmoRoot, m_linesColor.GetValue() );

        UpdateLines();
    }
}

// ***********************
//
void                        GridLines::SetColor          ( model::BasicNodePtr node, const glm::vec4 & color, TimeType time )
{
    auto colorPlugin = node->GetPlugin( "solid color" );
    auto colorParam = model::QueryTypedParam< model::ParamVec4Ptr >( colorPlugin->GetParameter( "color" ) );
    colorParam->SetVal( color, time );
}

// ***********************
//
void                        GridLines::UpdateLines      ()
{
    auto gizmoRoot = m_gizmoRoot.lock();

    auto assetDesc = m_gridLinesLogic->BuildDataArray( m_linesLength.GetValue() );
    auto linesPlugin = gizmoRoot->GetPlugin( "lines" );
    
    linesPlugin->LoadResource( std::static_pointer_cast< const AssetDesc >( assetDesc ) );
}

// ***********************
//
bool                        GridLines::PropagateUpdate  ()
{
    if( m_gridLinesLogic->GetUpdateID() > m_updateID )
        return true;
    return false;
}


// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        GridLines::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;
}

}   // nodelogic
}	// bv

