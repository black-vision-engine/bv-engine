#include "CrawlerEvents.h"


namespace bv { namespace nodelogic {

const EventType NodeAppearingCrawlerEvent::m_sEventType     = 0x01000006;
std::string NodeAppearingCrawlerEvent::m_sEventName         = "Event_NodeAppearingCrawlerEvent";

const EventType NodeLeavingCrawlerEvent::m_sEventType       = 0x01000007;
std::string NodeLeavingCrawlerEvent::m_sEventName           = "Event_NodeLeavingCrawlerEvent";

const EventType NoMoreNodesCrawlerEvent::m_sEventType       = 0x01000008;
std::string NoMoreNodesCrawlerEvent::m_sEventName           = "Event_NoMoreNodesCrawlerEvent";



// *******************************
//
CrawlerEvent::CrawlerEvent( CrawlerPtr crawler )
	: m_crawler( crawler )
{}

// ************************************* NodeAppearingCrawlerEvent *************************************

// *******************************
//
IEventPtr               NodeAppearingCrawlerEvent::Clone			() const
{
	return IEventPtr( new NodeAppearingCrawlerEvent( *this ) );
}

// *************************************
//
EventType           NodeAppearingCrawlerEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeAppearingCrawlerEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeAppearingCrawlerEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeAppearingCrawlerEvent::GetEventType() const
{   return this->m_sEventType; }



// ************************************* NodeLeavingCrawlerEvent *************************************

// *******************************
//
IEventPtr               NodeLeavingCrawlerEvent::Clone				() const
{
	return IEventPtr( new NodeLeavingCrawlerEvent( *this ) );
}

// *************************************
//
EventType           NodeLeavingCrawlerEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeLeavingCrawlerEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeLeavingCrawlerEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeLeavingCrawlerEvent::GetEventType() const
{   return this->m_sEventType; }


// ************************************* NoMoreNodesCrawlerEvent *************************************

// *******************************
//
IEventPtr               NoMoreNodesCrawlerEvent::Clone				() const
{
	return IEventPtr( new NoMoreNodesCrawlerEvent( *this ) );
}

// *************************************
//
EventType           NoMoreNodesCrawlerEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NoMoreNodesCrawlerEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NoMoreNodesCrawlerEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NoMoreNodesCrawlerEvent::GetEventType() const
{   return this->m_sEventType; }

} // widgest
} // bv