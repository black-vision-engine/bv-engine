#include "Engine/Events/BaseEvent.h"
#include "Scroller.h"

namespace bv { namespace nodelogic {

class ScrollerEvent : public BaseEvent
{
protected:
	ScrollerPtr				m_Scroller;
public:
	explicit 				ScrollerEvent			( ScrollerPtr Scroller );

	ScrollerConstPtr			GetScroller				() const	{ return m_Scroller; }
	ScrollerPtr				GetScroller				()			{ return m_Scroller; }
};

class NodeAppearingScrollerEvent : public ScrollerEvent
{
private:
	model::BasicNode *				m_node;

	static const EventType			m_sEventType;
    static std::string				m_sEventName;

public:
									NodeAppearingScrollerEvent	( ScrollerPtr Scroller, model::BasicNode * node )
										: ScrollerEvent( Scroller ), m_node( node )
									{}

									~NodeAppearingScrollerEvent  () {}

    virtual IEventPtr               Clone           () const	override;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

	const model::BasicNode *		GetNode			() const	{ return m_node; }
	model::BasicNode *				GetNode			()			{ return m_node; }
};

class NodeLeavingScrollerEvent : public ScrollerEvent
{
private:
	model::BasicNode *				m_node;

	static const EventType			m_sEventType;
    static std::string				m_sEventName;
public:
									NodeLeavingScrollerEvent		( ScrollerPtr Scroller, model::BasicNode * node )
										: ScrollerEvent( Scroller ), m_node( node )
									{}

									~NodeLeavingScrollerEvent	() {}

    virtual IEventPtr               Clone           () const                            override;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

	const model::BasicNode *		GetNode			() const	{ return m_node; }
	model::BasicNode *				GetNode			()			{ return m_node; }

};

class NoMoreNodesScrollerEvent : public ScrollerEvent
{
private:
	static const EventType			m_sEventType;
    static std::string				m_sEventName;
public:
									NoMoreNodesScrollerEvent		( ScrollerPtr Scroller )
										: ScrollerEvent( Scroller )
									{}

									~NoMoreNodesScrollerEvent	() {}

    virtual IEventPtr               Clone           () const                            override;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

} // widgest
} // bv
