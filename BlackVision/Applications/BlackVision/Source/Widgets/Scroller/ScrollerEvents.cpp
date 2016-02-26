#include "ScrollerEvents.h"


namespace bv { namespace nodelogic {

const EventType NodeAppearingScrollerEvent::m_sEventType     = 0x01000006;
std::string NodeAppearingScrollerEvent::m_sEventName         = "Event_NodeAppearingScrollerEvent";

const EventType NodeLeavingScrollerEvent::m_sEventType       = 0x01000007;
std::string NodeLeavingScrollerEvent::m_sEventName           = "Event_NodeLeavingScrollerEvent";

const EventType NoMoreNodesScrollerEvent::m_sEventType       = 0x01000008;
std::string NoMoreNodesScrollerEvent::m_sEventName           = "Event_NoMoreNodesScrollerEvent";



// *******************************
//
ScrollerEvent::ScrollerEvent( ScrollerPtr Scroller )
	: m_Scroller( Scroller )
{}

// ************************************* NodeAppearingScrollerEvent *************************************

// *******************************
//
IEventPtr               NodeAppearingScrollerEvent::Clone			() const
{
	return IEventPtr( new NodeAppearingScrollerEvent( *this ) );
}

// *************************************
//
EventType           NodeAppearingScrollerEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeAppearingScrollerEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeAppearingScrollerEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeAppearingScrollerEvent::GetEventType() const
{   return this->m_sEventType; }



// ************************************* NodeLeavingScrollerEvent *************************************

// *******************************
//
IEventPtr               NodeLeavingScrollerEvent::Clone				() const
{
	return IEventPtr( new NodeLeavingScrollerEvent( *this ) );
}

// *************************************
//
EventType           NodeLeavingScrollerEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeLeavingScrollerEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeLeavingScrollerEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeLeavingScrollerEvent::GetEventType() const
{   return this->m_sEventType; }


// ************************************* NoMoreNodesScrollerEvent *************************************

// *******************************
//
IEventPtr               NoMoreNodesScrollerEvent::Clone				() const
{
	return IEventPtr( new NoMoreNodesScrollerEvent( *this ) );
}

// *************************************
//
EventType           NoMoreNodesScrollerEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NoMoreNodesScrollerEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NoMoreNodesScrollerEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NoMoreNodesScrollerEvent::GetEventType() const
{   return this->m_sEventType; }

} // widgest
} // bv