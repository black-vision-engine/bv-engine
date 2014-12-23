#include "CrawlerEvents.h"


namespace bv { namespace widgets {

const EventType NodeAppearingCrawlerEvent::m_sEventType     = 0x00000006;
std::string NodeAppearingCrawlerEvent::m_sEventName         = "Event_NodeAppearingCrawlerEvent";

const EventType NodeLeavingCrawlerEvent::m_sEventType       = 0x00000007;
std::string NodeLeavingCrawlerEvent::m_sEventName           = "Event_NodeLeavingCrawlerEvent";

const EventType NoMoreNodesCrawlerEvent::m_sEventType       = 0x00000008;
std::string NoMoreNodesCrawlerEvent::m_sEventName           = "Event_NoMoreNodesCrawlerEvent";


// *******************************
//
CrawlerEvent::CrawlerEvent( CrawlerPtr crawler )
	: m_crawler( crawler )
{}

// *******************************
//
IEventPtr               NodeAppearingCrawlerEvent::Clone			() const
{
	return IEventPtr( new NodeAppearingCrawlerEvent( *this ) );
}

// *******************************
//
EventType				NodeAppearingCrawlerEvent::GetEventType		() const
{
	return m_sEventType;
}

// *******************************
//
const std::string &     NodeAppearingCrawlerEvent::GetName			() const
{
	return m_sEventName;
}

// *******************************
//
IEventPtr               NodeLeavingCrawlerEvent::Clone				() const
{
	return IEventPtr( new NodeLeavingCrawlerEvent( *this ) );
}

// *******************************
//
EventType				NodeLeavingCrawlerEvent::GetEventType		() const
{
	return m_sEventType;
}

// *******************************
//
const std::string &     NodeLeavingCrawlerEvent::GetName			() const
{
	return m_sEventName;
}

// *******************************
//
IEventPtr               NoMoreNodesCrawlerEvent::Clone				() const
{
	return IEventPtr( new NoMoreNodesCrawlerEvent( *this ) );
}

// *******************************
//
EventType				NoMoreNodesCrawlerEvent::GetEventType		() const
{
	return m_sEventType;
}

// *******************************
//
const std::string &     NoMoreNodesCrawlerEvent::GetName			() const
{
	return m_sEventName;
}

} // widgest
} // bv