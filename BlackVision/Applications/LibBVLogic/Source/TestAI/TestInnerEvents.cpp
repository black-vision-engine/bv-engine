#include "stdafxBVApp.h"

#include "TestInnerEvents.h"

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

#include "UseLoggerBVAppModule.h"


namespace bv
{



// ***********************
//
TestInnerEvents::TestInnerEvents()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::EffectAdded ), NodeEffectAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::EffectRemoved ), NodeEffectRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::LogicAdded ), NodeLogicAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::LogicRemoved ), NodeLogicRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::NodeAdded ), NodeAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::NodeMoved ), NodeMovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::NodeRemoved ), NodeRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::PluginAdded ), PluginAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::PluginMoved ), PluginMovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::PluginRemoved ), PluginRemovedEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::LightAdded ), LightAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::LightRemoved ), LightRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::CameraAdded ), CameraAddedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::CameraRemoved ), CameraRemovedEvent::Type() );
}

// ***********************
//
TestInnerEvents::~TestInnerEvents()
{}


// ***********************
//
void        TestInnerEvents::NodeAdded      ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "NodeAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::NodeRemoved    ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "NodeRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::NodeMoved      ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "NodeMoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::PluginAdded    ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug )  << "PluginAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::PluginMoved    ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "PluginMoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::PluginRemoved  ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "PluginRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::LogicAdded     ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "LogicAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::LogicRemoved   ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "LogicRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::EffectAdded    ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "EffectAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::EffectRemoved  ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "EffectRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::LightAdded     ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "LightAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::LightRemoved   ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "LightRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::CameraAdded    ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "CameraAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::CameraRemoved  ( bv::IEventPtr evt )
{
    LOG_MESSAGE( SeverityLevel::debug ) << "CameraRemoved" << std::endl;
}




}	// bv