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
public:

	static const EventType			m_sEventType;
    static std::string				m_sEventName;

	model::BasicNodePtr				m_node;

public:
									NodeAppearingCrawlerEvent	( CrawlerPtr crawler, model::BasicNodePtr node )
										: CrawlerEvent( crawler ), m_node( node )
									{}

									~NodeAppearingCrawlerEvent  () {}

    virtual IEventPtr               Clone           () const                            override;

    virtual EventType               GetEventType    () const                            override;
	virtual const std::string &     GetName         () const                            override;

    //virtual void                    Serialize       ( std::ostringstream & out ) const  override;
    //virtual void                    Deserialize     ( std::istringstream & in )         override;

   
};

class NodeLeavingCrawlerEvent : public CrawlerEvent
{
public:

	static const EventType			m_sEventType;
    static std::string				m_sEventName;

	model::BasicNodePtr				m_node;

public:
									NodeLeavingCrawlerEvent		( CrawlerPtr crawler, model::BasicNodePtr node )
										: CrawlerEvent( crawler ), m_node( node )
									{}

									~NodeLeavingCrawlerEvent	() {}

    virtual IEventPtr               Clone           () const                            override;

    virtual EventType               GetEventType    () const                            override;
	virtual const std::string &     GetName         () const                            override;

    //virtual void                    Serialize       ( std::ostringstream & out ) const  override;
    //virtual void                    Deserialize     ( std::istringstream & in )         override;
};

class NoMoreNodesCrawlerEvent : public CrawlerEvent
{
public:

	static const EventType			m_sEventType;
    static std::string				m_sEventName;

public:
									NoMoreNodesCrawlerEvent		( CrawlerPtr crawler )
										: CrawlerEvent( crawler )
									{}

									~NoMoreNodesCrawlerEvent	() {}

    virtual IEventPtr               Clone           () const                            override;

    virtual EventType               GetEventType    () const                            override;
	virtual const std::string &     GetName         () const                            override;

    //virtual void                    Serialize       ( std::ostringstream & out ) const  override;
    //virtual void                    Deserialize     ( std::istringstream & in )         override;
};

} // widgest
} // bv
