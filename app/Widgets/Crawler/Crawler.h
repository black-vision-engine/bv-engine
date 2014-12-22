#pragma once

#include "System/BasicTypes.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "CrawlerNodesStates.h"

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

namespace bv { namespace widgets {

class Crawler;

DEFINE_PTR_TYPE( Crawler )
DEFINE_CONST_PTR_TYPE( Crawler )

class Crawler : public model::INodeLogic, public std::enable_shared_from_this< Crawler >
{
	typedef std::map< bv::model::BasicNode *, Float32 > NodeFloatMap;
	typedef std::map< bv::model::BasicNode *, bool >	NodeBoolMap;

private:
	bool									m_isFinalized;
	bv::model::BasicNode *					m_parentNode;
	CrawlerNodesStates						m_nodesStates;
	NodeFloatMap							m_shifts;
	mathematics::RectConstPtr				m_view;
	UInt64									m_currTime;
	bool									m_started;
	Float32									m_speed;

	void		LayoutNodes			();
	void		UpdateTransforms	();
	void		UpdateVisibility	( bv::model::BasicNode * );
	void		SetActiveNode		( bv::model::BasicNode * );
	bool		IsActive			( bv::model::BasicNode * );
	void		NotifyVisibilityChanged( const bv::model::BasicNode *, bool ) const;
	void		NotifyNoMoreNodes	();

public:

	explicit	Crawler				( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view );
				~Crawler			() {}

	void		AddNext				( bv::model::BasicNodePtr node );
	bool		Finalize			();

	model::BasicNode *	GetNonActiveNode();
	void		EnqueueNode			( model::BasicNode * n);

	void		SetSpeed			( Float32 speed );

	virtual void	Initialize		()				override {}
	virtual void	Update			( TimeType t )	override;
	virtual void	Deinitialize	()				override {}

	void			Start			();
	void			Stop			();

	static		CrawlerPtr Create	( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view );
};

} 
} // bv