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
EndUserParamsLogic::EndUserParamsLogic()
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
    m_paramsDescsMap[ param ] = std::move( descriptor );
    return true;
}

// ***********************
//
bool            EndUserParamsLogic::RemoveDescriptor( const ParameterAddress & param )
{
    auto iter = m_paramsDescsMap.find( param );
    if( iter != m_paramsDescsMap.end() )
    {
        m_paramsDescsMap.erase( iter );
        return true;
    }
    return false;
}

// ***********************
//
EndUserParamDescriptor *      EndUserParamsLogic::GetDescriptor   ( const ParameterAddress & param )
{
    auto iter = m_paramsDescsMap.find( param );
    if( iter != m_paramsDescsMap.end() )
    {
        return &iter->second;
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

        for( auto & paramMapping : m_paramsDescsMap )
        {
            ser.EnterChild( "mapping" );
                ser.EnterChild( "param" );
                paramMapping.first.Serialize( ser );
                ser.ExitChild();    // param

                paramMapping.second.Serialize( ser );
            ser.ExitChild();    // mapping
        }

    ser.ExitChild();    // endUserParams
}

// ***********************
//
void            EndUserParamsLogic::Deserialize     ( const IDeserializer & deser )
{
    if( deser.EnterChild( "endUserParams" ) )
    {
        if( deser.EnterChild( "mapping" ) )
        {
            do
            {
                bool paramValid = false;
                bool descriptorValid = false;

                ParameterAddress param;
                if( deser.EnterChild( "param" ) )
                {
                    param = ParameterAddress::Create( deser );

                    deser.ExitChild();  // param
                    paramValid = true;
                }

                EndUserParamDescriptor descriptor;
                if( deser.EnterChild( "paramDescriptor" ) )
                {
                    descriptor = EndUserParamDescriptor::Create( deser );

                    deser.ExitChild();  // paramDescriptor
                    descriptorValid = true;
                }

                if( paramValid && descriptorValid )
                    AddDescriptor( std::move( param  ), std::move( descriptor ) );

            } while( deser.NextChild() );

            deser.ExitChild();  // mapping
        }
        deser.ExitChild();  // endUserParams
    }
}

}	// bv