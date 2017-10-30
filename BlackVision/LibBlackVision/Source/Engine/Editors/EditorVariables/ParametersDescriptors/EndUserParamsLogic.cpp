#include "stdafx.h"
#include "EndUserParamsLogic.h"

#include "Engine/Events/EventManager.h"

#include "Engine/Events/InnerEvents/Plugins/PluginAddedEvent.h"
#include "Engine/Events/InnerEvents/Plugins/PluginRemovedEvent.h"
#include "Engine/Events/InnerEvents/Plugins/PluginMovedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeAddedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeMovedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeEffectAddedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeEffectRemovedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeLogicAddedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeLogicRemovedEvent.h"
#include "Engine/Events/InnerEvents/Other/CameraAddedEvent.h"
#include "Engine/Events/InnerEvents/Other/CameraRemovedEvent.h"
#include "Engine/Events/InnerEvents/Other/LightAddedEvent.h"
#include "Engine/Events/InnerEvents/Other/LightRemovedEvent.h"

#include "Engine/Models/ModelState.h"


namespace bv
{

// ***********************
//
EndUserParamsLogic::EndUserParamsLogic  ( model::SceneModel * owner )
    :   m_ownerScene( owner )
    ,   m_cameraDescs( owner )
    ,   m_effectDescs( owner )
    ,   m_lightDescs( owner )
    ,   m_logicDescs( owner )
    ,   m_pluginDescs( owner )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::EffectAdded ), NodeEffectAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::EffectRemoved ), NodeEffectRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LogicAdded ), NodeLogicAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LogicRemoved ), NodeLogicRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::NodeAdded ), NodeAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::NodeMoved ), NodeMovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::NodeRemoved ), NodeRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::PluginAdded ), PluginAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::PluginMoved ), PluginMovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::PluginRemoved ), PluginRemovedEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LightAdded ), LightAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LightRemoved ), LightRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::CameraAdded ), CameraAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::CameraRemoved ), CameraRemovedEvent::Type() );
}

// ***********************
//
EndUserParamsLogic::~EndUserParamsLogic()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::EffectAdded ), NodeEffectAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::EffectRemoved ), NodeEffectRemovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LogicAdded ), NodeLogicAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LogicRemoved ), NodeLogicRemovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::NodeAdded ), NodeAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::NodeMoved ), NodeMovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::NodeRemoved ), NodeRemovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::PluginAdded ), PluginAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::PluginMoved ), PluginMovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::PluginRemoved ), PluginRemovedEvent::Type() );

    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LightAdded ), LightAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::LightRemoved ), LightRemovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::CameraAdded ), CameraAddedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &EndUserParamsLogic::CameraRemoved ), CameraRemovedEvent::Type() );
}

// ***********************
//
bool            EndUserParamsLogic::AddDescriptor   ( ParameterAddress && param, EndUserParamDescriptor && descriptor )
{
    switch( param.ParamTargetType )
    {
        case ParameterAddress::TargetType::PluginParam:
            return m_pluginDescs.AddDescriptor( std::move( param ), std::move( descriptor ) );
        case ParameterAddress::TargetType::GlobalEffectParam:
            return m_effectDescs.AddDescriptor( std::move( param ), std::move( descriptor ) );
        case ParameterAddress::TargetType::ResourceParam:
            return m_pluginDescs.AddDescriptor( std::move( param ), std::move( descriptor ) );
        case ParameterAddress::TargetType::LightParam:
            return m_lightDescs.AddDescriptor( std::move( param ), std::move( descriptor ) );
        case ParameterAddress::TargetType::NodeLogicParam:
            return m_logicDescs.AddDescriptor( std::move( param ), std::move( descriptor ) );
        case ParameterAddress::TargetType::CameraParam:
            return m_cameraDescs.AddDescriptor( std::move( param ), std::move( descriptor ) );
        default:
            return false;
    }
}

// ***********************
//
bool            EndUserParamsLogic::RemoveDescriptor    ( const ParameterAddress & param )
{
    switch( param.ParamTargetType )
    {
        case ParameterAddress::TargetType::PluginParam:
            return m_pluginDescs.RemoveDescriptor( param );
        case ParameterAddress::TargetType::GlobalEffectParam:
            return m_effectDescs.RemoveDescriptor( param );
        case ParameterAddress::TargetType::ResourceParam:
            return m_pluginDescs.RemoveDescriptor( param );
        case ParameterAddress::TargetType::LightParam:
            return m_lightDescs.RemoveDescriptor( param );
        case ParameterAddress::TargetType::NodeLogicParam:
            return m_logicDescs.RemoveDescriptor( param );
        case ParameterAddress::TargetType::CameraParam:
            return m_cameraDescs.RemoveDescriptor( param );
        default:
            return false;
    }
}

// ***********************
//
EndUserParamDescriptor *      EndUserParamsLogic::GetDescriptor   ( const ParameterAddress & param )
{
    if( param.SceneName != m_ownerScene->GetName() )
        return nullptr;

    switch( param.ParamTargetType )
    {
        case ParameterAddress::TargetType::PluginParam:
            return m_pluginDescs.GetDescriptor( param );
        case ParameterAddress::TargetType::GlobalEffectParam:
            return m_effectDescs.GetDescriptor( param );
        case ParameterAddress::TargetType::ResourceParam:
            return m_pluginDescs.GetDescriptor( param );
        case ParameterAddress::TargetType::LightParam:
            return m_lightDescs.GetDescriptor( param );
        case ParameterAddress::TargetType::NodeLogicParam:
            return m_logicDescs.GetDescriptor( param );
        case ParameterAddress::TargetType::CameraParam:
            return m_cameraDescs.GetDescriptor( param );
    }

    return nullptr;
}


// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void            EndUserParamsLogic::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "endUserParams" );

    m_cameraDescs.Serialize( ser );
    m_effectDescs.Serialize( ser );
    m_logicDescs.Serialize( ser );
    m_pluginDescs.Serialize( ser );
    m_lightDescs.Serialize( ser );


    ser.ExitChild();    // endUserParams
}

// ***********************
//
void            EndUserParamsLogic::Deserialize     ( const IDeserializer & deser )
{
    if( deser.EnterChild( "endUserParams" ) )
    {
        m_cameraDescs.Deserialize( deser );
        m_effectDescs.Deserialize( deser );
        m_logicDescs.Deserialize( deser );
        m_pluginDescs.Deserialize( deser );
        m_lightDescs.Deserialize( deser );

        deser.ExitChild();  // endUserParams
    }
}

// ========================================================================= //
// Parameters descriptors update.
// ========================================================================= //


// ***********************
//
template< typename EventType >
bool            FilterEvent     ( bv::IEventPtr evt, model::SceneModel * ownerScene )
{
    if( evt->GetEventType() != EventType::Type() )
        return true;

    typedef std::shared_ptr< EventType > EventTypePtr;
    EventTypePtr event = std::static_pointer_cast< EventType >( evt );

    auto scene = model::ModelState::GetInstance().QueryNodeScene( event->ParentNode.get() );
    if( scene != ownerScene )
        return true;

    return false;
}



// ***********************
//
void            EndUserParamsLogic::NodeAdded( bv::IEventPtr evt )
{
    if( !FilterEvent< NodeAddedEvent >( evt, m_ownerScene ) )
    {
        NodeAddedEventPtr event = std::static_pointer_cast< NodeAddedEvent >( evt );

        m_effectDescs.NodeAdded( event->ParentNode, event->AddedNode );
        m_logicDescs.NodeAdded( event->ParentNode, event->AddedNode );
        m_pluginDescs.NodeAdded( event->ParentNode, event->AddedNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::NodeRemoved( bv::IEventPtr evt )
{
    if( !FilterEvent< NodeRemovedEvent >( evt, m_ownerScene ) )
    {
        NodeRemovedEventPtr event = std::static_pointer_cast< NodeRemovedEvent >( evt );

        m_effectDescs.NodeRemoved( event->ParentNode, event->RemovedNode );
        m_logicDescs.NodeRemoved( event->ParentNode, event->RemovedNode );
        m_pluginDescs.NodeRemoved( event->ParentNode, event->RemovedNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::NodeMoved( bv::IEventPtr evt )
{
    if( evt->GetEventType() != NodeMovedEvent::Type() )
        return;

    NodeMovedEventPtr event = std::static_pointer_cast< NodeMovedEvent >( evt );
    
    auto scene = model::ModelState::GetInstance().QueryNodeScene( event->SrcParentNode.get() );
    auto newScene = model::ModelState::GetInstance().QueryNodeScene( event->DstParentNode.get() );
    
    if( scene != m_ownerScene )
        return;

    // Descriptors are per scene.
    if( scene != newScene )
    {
        // Remove Descriptor.
        // FIXME: We should move descriptor to new scene.
        m_effectDescs.NodeRemoved( event->SrcParentNode, event->Node );
        m_logicDescs.NodeRemoved( event->SrcParentNode, event->Node );
        m_pluginDescs.NodeRemoved( event->SrcParentNode, event->Node );
    }
    else
    {
        m_effectDescs.NodeMoved( event->SrcParentNode, event->DstParentNode, event->Node );
        m_logicDescs.NodeMoved( event->SrcParentNode, event->DstParentNode, event->Node );
        m_pluginDescs.NodeMoved( event->SrcParentNode, event->DstParentNode, event->Node );
    }
}


// ***********************
//
void            EndUserParamsLogic::PluginAdded( bv::IEventPtr evt )
{
    if( !FilterEvent< PluginAddedEvent >( evt, m_ownerScene ) )
    {
        PluginAddedEventPtr event = std::static_pointer_cast< PluginAddedEvent >( evt );
        m_pluginDescs.ContainerAdded( event->Plugin, event->ParentNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::PluginMoved( bv::IEventPtr evt )
{
    if( evt->GetEventType() != PluginMovedEvent::Type() )
        return;

    PluginMovedEventPtr event = std::static_pointer_cast< PluginMovedEvent >( evt );

    auto scene = model::ModelState::GetInstance().QueryNodeScene( event->SrcParentNode.get() );
    auto newScene = model::ModelState::GetInstance().QueryNodeScene( event->DstParentNode.get() );

    if( scene != m_ownerScene )
        return;

    if( scene != newScene )
    {
        // Remove Descriptor.
        // FIXME: We should move descriptor to new scene.
        m_pluginDescs.ContainerRemoved( event->Plugin, event->SrcParentNode );
        return;
    }

    m_pluginDescs.ContainerMoved( event->Plugin, event->SrcParentNode, event->DstParentNode );
}

// ***********************
//
void            EndUserParamsLogic::PluginRemoved( bv::IEventPtr evt )
{
    if( !FilterEvent< PluginAddedEvent >( evt, m_ownerScene ) )
    {
        PluginRemovedEventPtr event = std::static_pointer_cast< PluginRemovedEvent >( evt );
        m_pluginDescs.ContainerRemoved( event->Plugin, event->ParentNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::LogicAdded( bv::IEventPtr evt )
{
    if( !FilterEvent< PluginAddedEvent >( evt, m_ownerScene ) )
    {
        NodeLogicAddedEventPtr event = std::static_pointer_cast< NodeLogicAddedEvent >( evt );
        m_logicDescs.ContainerAdded( event->Logic, event->ParentNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::LogicRemoved( bv::IEventPtr evt )
{
    if( !FilterEvent< PluginAddedEvent >( evt, m_ownerScene ) )
    {
        NodeLogicRemovedEventPtr event = std::static_pointer_cast< NodeLogicRemovedEvent >( evt );
        m_logicDescs.ContainerRemoved( event->Logic, event->ParentNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::EffectAdded( bv::IEventPtr evt )
{
    if( !FilterEvent< PluginAddedEvent >( evt, m_ownerScene ) )
    {
        NodeEffectAddedEventPtr event = std::static_pointer_cast< NodeEffectAddedEvent >( evt );
        m_effectDescs.ContainerAdded( event->Effect, event->ParentNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::EffectRemoved( bv::IEventPtr evt )
{
    if( !FilterEvent< PluginAddedEvent >( evt, m_ownerScene ) )
    {
        NodeEffectRemovedEventPtr event = std::static_pointer_cast< NodeEffectRemovedEvent >( evt );
        m_effectDescs.ContainerRemoved( event->Effect, event->ParentNode );
    }
}

// ***********************
//
void            EndUserParamsLogic::LightAdded( bv::IEventPtr evt )
{
    if( evt->GetEventType() != LightAddedEvent::Type() )
        return;

    LightAddedEventPtr event = std::static_pointer_cast< LightAddedEvent >( evt );

    model::BasicNodePtr emptyPtr;
    m_lightDescs.ContainerAdded( event->Light, emptyPtr );
}

// ***********************
//
void            EndUserParamsLogic::LightRemoved( bv::IEventPtr evt )
{
    if( evt->GetEventType() != LightRemovedEvent::Type() )
        return;

    LightRemovedEventPtr event = std::static_pointer_cast< LightRemovedEvent >( evt );

    model::BasicNodePtr emptyPtr;
    m_lightDescs.ContainerRemoved( event->Light, emptyPtr );
}

// ***********************
//
void            EndUserParamsLogic::CameraAdded( bv::IEventPtr evt )
{
    if( evt->GetEventType() != CameraAddedEvent::Type() )
        return;

    CameraAddedEventPtr event = std::static_pointer_cast< CameraAddedEvent >( evt );

    model::BasicNodePtr emptyPtr;
    m_cameraDescs.ContainerAdded( event->Camera, emptyPtr );
}

// ***********************
//
void            EndUserParamsLogic::CameraRemoved( bv::IEventPtr evt )
{
    if( evt->GetEventType() != CameraRemovedEvent::Type() )
        return;

    CameraRemovedEventPtr event = std::static_pointer_cast< CameraRemovedEvent >( evt );

    model::BasicNodePtr emptyPtr;
    m_cameraDescs.ContainerRemoved( event->Camera, emptyPtr );
}



}	// bv