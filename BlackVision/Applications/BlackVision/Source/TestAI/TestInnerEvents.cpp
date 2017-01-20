#include "stdafxBVApp.h"

#include "TestInnerEvents.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/InnerEvents/Plugins/PluginAddedEvent.h"
#include "Engine/Events/InnerEvents/Plugins/PluginRemovedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeAddedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeMovedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeEffectAddedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeEffectRemovedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeLogicAddedEvent.h"
#include "Engine/Events/InnerEvents/Logics/NodeLogicRemovedEvent.h"


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
    //GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::PluginMoved ), PluginMovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::PluginRemoved ), PluginRemovedEvent::Type() );
}

// ***********************
//
TestInnerEvents::~TestInnerEvents()
{}


// ***********************
//
void        TestInnerEvents::NodeAdded      ( bv::IEventPtr evt )
{
    std::cout << "NodeAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::NodeRemoved    ( bv::IEventPtr evt )
{
    std::cout << "NodeRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::NodeMoved      ( bv::IEventPtr evt )
{
    std::cout << "NodeMoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::PluginAdded    ( bv::IEventPtr evt )
{
    std::cout << "PluginAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::PluginMoved    ( bv::IEventPtr evt )
{
    std::cout << "PluginMoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::PluginRemoved  ( bv::IEventPtr evt )
{
    std::cout << "PluginRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::LogicAdded     ( bv::IEventPtr evt )
{
    std::cout << "LogicAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::LogicRemoved   ( bv::IEventPtr evt )
{
    std::cout << "LogicRemoved" << std::endl;
}

// ***********************
//
void        TestInnerEvents::EffectAdded    ( bv::IEventPtr evt )
{
    std::cout << "EffectAdded" << std::endl;
}

// ***********************
//
void        TestInnerEvents::EffectRemoved  ( bv::IEventPtr evt )
{
    std::cout << "EffectRemoved" << std::endl;
}




}	// bv