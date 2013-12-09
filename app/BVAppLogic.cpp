#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"


namespace bv
{

namespace
{
IEventManager * GEventManager = nullptr;
}

// *********************************
//
BVAppLogic::BVAppLogic              ()
    : m_startTime( 0 )
{
}

// *********************************
//
BVAppLogic::~BVAppLogic             ()
{
}

// *********************************
//
void BVAppLogic::OnInitialize       ()
{
    GEventManager = &bv::GetDefaultEventManager();
}

// *********************************
//
void BVAppLogic::SetStartTime       ( unsigned long millis )
{
    m_startTime = millis;
}

// *********************************
//
void BVAppLogic::OnUpdate           ( unsigned long millis )
{
    
}

// *********************************
//
void BVAppLogic::FrameRendered      ( IEventPtr evt )
{
}

} //bv
