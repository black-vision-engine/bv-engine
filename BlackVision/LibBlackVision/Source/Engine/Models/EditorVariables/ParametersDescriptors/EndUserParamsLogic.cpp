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

// ***********************
//
void EndUserParamsLogic::NodeAdded( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::NodeRemoved( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::NodeMoved( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::PluginAdded( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::PluginMoved( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::PluginRemoved( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::LogicAdded( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::LogicRemoved( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::EffectAdded( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::EffectRemoved( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::LightAdded( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::LightRemoved( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::CameraAdded( bv::IEventPtr evt )
{}

// ***********************
//
void EndUserParamsLogic::CameraRemoved( bv::IEventPtr evt )
{}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void            EndUserParamsLogic::Serialize       ( ISerializer & ser ) const
{
    ser.EnterArray( "endUserParams" );

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

}	// bv