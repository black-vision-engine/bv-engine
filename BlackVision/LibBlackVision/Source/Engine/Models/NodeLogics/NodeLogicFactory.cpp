#include "stdafx.h"

#include "NodeLogicFactory.h"

#include "Counter/Counter.h"
#include "Scroller/Scroller.h"
#include "NodeReplicator/NodeReplicator.h"
#include "MeshLoader/MeshLoader.h"
#include "SmoothValueSetter/SmoothValueSetter.h"
#include "PieChart/PieChart.h"
#include "AnalogWatch/AnalogWatch.h"
#include "Arrange/Arrange.h"
#include "Follow/Follow.h"
#include "SlideShow/SlideShow.h"
#include "MaxSize/MaxSize.h"
#include "Cloner/Cloner.h"
#include "NodeVisibilityAnimation/NodeVisibilityAnimation.h"

#include "Engine/Models/Gizmos/Logics/ShowFPS/ShowFPS.h"
#include "Engine/Models/Gizmos/Logics/BoundingBox/BoundingBoxLogic.h"
#include "Engine/Models/Gizmos/Logics/GridLines/GridLines.h"



namespace bv {


// ***********************
//
template< typename LogicType >
model::INodeLogicPtr        Create      ( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent )
{
    return LogicType::Create( deser, logicParent );
}




// ***********************
//
NodeLogicFactory::NodeLogicFactory()
{
    RegisterDefaultLogics();
}

// ***********************
//
model::INodeLogicPtr        NodeLogicFactory::CreateLogic  ( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent )
{
    const std::string & logicType = deser.GetAttribute( "type" );

    auto descIter = m_logics.find( logicType );
    if( descIter == m_logics.end() )
        return nullptr;

    return descIter->second->CreateLogic( deser, logicParent );
}

// ***********************
//
model::IGizmoLogicPtr       NodeLogicFactory::CreateGizmoLogic      ( const std::string & gizmoName, model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto descIter = m_gizmoLogics.find( gizmoName );
    if( descIter == m_gizmoLogics.end() )
        return nullptr;

    return descIter->second->CreateLogic( gizmoRoot, gizmoOwner, timeEvaluator );
}

// ***********************
//
std::string                 NodeLogicFactory::QueryGizmoName        ( const std::string & logicName, const std::string & function )
{
    auto descIter = m_logics.find( logicName );
    if( descIter == m_logics.end() )
        return "";

    auto & desc = descIter->second;
    return desc->AccessGizmoMapper().QueryGizmoLogicName( function );
}


// ========================================================================= //
// Registration
// ========================================================================= //

// ***********************
//
model::NodeLogicDesc &      NodeLogicFactory::RegisterLogic         ( const std::string & logicType, model::LogicCreateFun creator )
{
    assert( m_logics[ logicType ] == nullptr );

    model::NodeLogicDescPtr desc = std::make_shared< model::NodeLogicDesc >( creator );
    m_logics[ logicType ] = desc;
    
    return *( desc.get() );
}

// ***********************
//
model::GizmoLogicDesc &     NodeLogicFactory::RegisterGizmo         ( const std::string & logicType, model::GizmoCreateFun creator )
{
    assert( m_gizmoLogics[ logicType ] == nullptr );

    model::GizmoLogicDescPtr desc = std::make_shared< model::GizmoLogicDesc >( creator );
    m_gizmoLogics[ logicType ] = desc;

    return *( desc.get() );
}



// ***********************
// TODO: In future there should be public registration function. This code maybe should be moved
// somewhere outside in application logic.
void                        NodeLogicFactory::RegisterDefaultLogics ()
{
    RegisterLogic( nodelogic::Scroller::Type(), &Create< nodelogic::Scroller > );
    RegisterLogic( nodelogic::WidgetCounter::Type(), &Create< nodelogic::WidgetCounter > );
    RegisterLogic( nodelogic::NodeReplicator::Type(), &Create< nodelogic::NodeReplicator > );
    RegisterLogic( nodelogic::SmoothValueSetter::Type(), &Create< nodelogic::SmoothValueSetter > );
    RegisterLogic( nodelogic::MeshLoader::Type(), &Create< nodelogic::MeshLoader > );
    RegisterLogic( nodelogic::PieChart::Type(), &Create< nodelogic::PieChart > );
    RegisterLogic( nodelogic::AnalogWatch::Type(), &Create< nodelogic::AnalogWatch > );
    RegisterLogic( nodelogic::Arrange::Type(), &Create< nodelogic::Arrange > );
    RegisterLogic( nodelogic::Follow::Type(), &Create< nodelogic::Follow > );
    RegisterLogic( nodelogic::SlideShow::Type(), &Create < nodelogic::SlideShow > );
    RegisterLogic( nodelogic::MaxSize::Type(), &Create < nodelogic::MaxSize > );
    RegisterLogic( nodelogic::Cloner::Type(), &Create< nodelogic::Cloner > );
    RegisterLogic( nodelogic::NodeVisibilityAnimation::Type(), &Create< nodelogic::NodeVisibilityAnimation > );

    // Gizmos registration
    RegisterGizmo( nodelogic::ShowFPS::Type(), &nodelogic::ShowFPS::Create );
    RegisterGizmo( nodelogic::BoundingBoxLogic::Type(), &nodelogic::BoundingBoxLogic::Create );
    RegisterGizmo( nodelogic::GridLines::Type(), &nodelogic::GridLines::Create );
}

} //bv