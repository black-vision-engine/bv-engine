#include "Engine/Events/BaseEvent.h"
#include "Crawler.h"

namespace bv { namespace widgets {

class CrawlerEvent : public BaseEvent
{
protected:
	CrawlerPtr				m_crawler;
public:
	explicit 				CrawlerEvent			( CrawlerPtr crawler );

	CrawlerConstPtr			GetCrawler				() const	{ return m_crawler; }
	CrawlerPtr				GetCrawler				()			{ return m_crawler; }
};

class NodeAppearingCrawlerEvent : public CrawlerEvent
{
private:
	model::BasicNode *				m_node;

	static const EventType			m_sEventType;
    static std::string				m_sEventName;

public:
									NodeAppearingCrawlerEvent	( CrawlerPtr crawler, model::BasicNode * node )
										: CrawlerEvent( crawler ), m_node( node )
									{}

									~NodeAppearingCrawlerEvent  () {}

    virtual IEventPtr               Clone           () const	override;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

	const model::BasicNode *		GetNode			() const	{ return m_node; }
	model::BasicNode *				GetNode			()			{ return m_node; }
};

class NodeLeavingCrawlerEvent : public CrawlerEvent
{
private:
	model::BasicNode *				m_node;

	static const EventType			m_sEventType;
    static std::string				m_sEventName;
public:
									NodeLeavingCrawlerEvent		( CrawlerPtr crawler, model::BasicNode * node )
										: CrawlerEvent( crawler ), m_node( node )
									{}

									~NodeLeavingCrawlerEvent	() {}

    virtual IEventPtr               Clone           () const                            override;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

	const model::BasicNode *		GetNode			() const	{ return m_node; }
	model::BasicNode *				GetNode			()			{ return m_node; }

};

class NoMoreNodesCrawlerEvent : public CrawlerEvent
{
private:
	static const EventType			m_sEventType;
    static std::string				m_sEventName;
public:
									NoMoreNodesCrawlerEvent		( CrawlerPtr crawler )
										: CrawlerEvent( crawler )
									{}

									~NoMoreNodesCrawlerEvent	() {}

    virtual IEventPtr               Clone           () const                            override;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

} // widgest
} // bv
