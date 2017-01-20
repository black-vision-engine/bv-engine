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
    //GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &TestInnerEvents::EffectAdded ), NodeEffectAddedEvent:: )
}

// ***********************
//
TestInnerEvents::~TestInnerEvents()
{}


// ***********************
//
void        TestInnerEvents::NodeAdded      ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::NodeRemoved    ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::NodeMoved      ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::PluginAdded    ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::PluginMoved    ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::PluginRemoved  ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::LogicAdded     ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::LogicRemoved   ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::EffectAdded    ( bv::IEventPtr evt )
{}

// ***********************
//
void        TestInnerEvents::EffectRemoved  ( bv::IEventPtr evt )
{}




}	// bv