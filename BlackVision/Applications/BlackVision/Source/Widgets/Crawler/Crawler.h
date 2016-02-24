#pragma once

#include "CoreDEF.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "CrawlerNodesStates.h"
#include <string>
#include <vector>
#include <map>

// forward references
namespace bv {

namespace mathematics {

struct Rect;
typedef std::shared_ptr< const Rect > RectConstPtr;

}

namespace model {

class BasicNode;
typedef std::shared_ptr< BasicNode > BasicNodePtr;

} // model
} // bv

namespace bv { namespace nodelogic {

class Crawler;

DEFINE_PTR_TYPE( Crawler )
DEFINE_CONST_PTR_TYPE( Crawler )

class Crawler : public model::INodeLogic, public std::enable_shared_from_this< Crawler >
{
private:
    static const std::string        m_type;


	typedef std::map< bv::model::BasicNode *, Float32 > NodeFloatMap;
	typedef std::map< bv::model::BasicNode *, bool >	NodeBoolMap;

private:
	bool									m_isFinalized;
	bv::model::BasicNode *					m_parentNode;
	CrawlerNodesStates						m_nodesStates;
	NodeFloatMap							m_shifts;	
	UInt64									m_currTime;

    mathematics::RectConstPtr				m_view;
	Float32									m_speed;
	Float32									m_interspace;
	bool									m_started;
    bool                                    m_paused;
    bool                                    m_enableEvents;

	//pawe³ek
	std::vector<std::wstring>				m_messages_new;
	std::vector<std::wstring>				m_messages_displayed;
	std::wstring							m_promo_msg;
	int										m_promo_freq;
	int										m_displayed_index;
	int										m_total_displayed_msgs;




	void		LayoutNodes			();
	void		UpdateTransforms	();
	void		UpdateVisibility	( bv::model::BasicNode * );
	void		SetActiveNode		( bv::model::BasicNode * );
	bool		IsActive			( bv::model::BasicNode * );
	void		NotifyVisibilityChanged( bv::model::BasicNode *, bool );
	void		NotifyNoMoreNodes	();
	void		HackNoMoreNodes		();
    
    bool        AddNode             ( bv::model::BasicNodePtr node );

public:

	void		AddMessage			(std::wstring msg);
	void		SetPromoMessage		(std::wstring msg);
	void		SetPromoFrequency	(int freq);
    void		Clear				();

	explicit	Crawler				( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view );
				~Crawler			() {}

	void		AddNext				( bv::model::BasicNodePtr node );
    bool		AddNext				( Int32 nodeIdx );
    bool		AddNext				( const std::string& childNodeName );
	bool		Finalize			();
    bool        Unfinalize          ();

	model::BasicNode *	GetNonActiveNode();
	void		EnqueueNode			( model::BasicNode * n);

	void		SetSpeed			( Float32 speed );
	void		SetInterspace		( Float32 interspace );

	virtual void	Initialize		()				override {}
	virtual void	Update			( TimeType t )	override;
	virtual void	Deinitialize	()				override {}


	static		CrawlerPtr Create	( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view );

    virtual const std::string   GetType         () const override;

    virtual void                Serialize       ( ISerializer& ser ) const override;
    static CrawlerPtr           Create          ( const IDeserializer & deser, bv::model::BasicNode * parentNode );

    virtual bool                HandleEvent     ( IDeserializer& eventSer, ISerializer& response, BVProjectEditor * editor ) override;

public:
    // Event handler funtions
	void			Start			    ();
	void			Stop			    ();
	void		    Reset				();
    void            Pause               ();

    bool            SmoothStart         ();
    bool            SmoothPause         ();

    bool            AddPreset           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );
    bool            AddPresetAndMessages( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );

    bool            GetStatus           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );

private:
    void            AddTexts            ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor, model::BasicNodePtr node );
    void            AddImages           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor, model::BasicNodePtr node );
};

} 
} // bv
