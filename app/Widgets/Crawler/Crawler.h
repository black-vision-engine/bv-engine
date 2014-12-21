#pragma once

#include "System/BasicTypes.h"

#include "Engine/Models/Interfaces/INodeLogic.h"

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
	typedef std::map< bv::model::BasicNode *, Float32 > ShiftsMap;

private:
	bool									m_isFinalized;
	bv::model::BasicNode *					m_parentNode;
	std::vector< bv::model::BasicNode * >	m_nodes;
	ShiftsMap								m_shifts;
	mathematics::RectConstPtr				m_view;
	UInt64									m_currTime;
	bool									m_started;
	Float32									m_speed;

	void		LayoutNodes			();
	void		UpdateTransforms	();

public:

	explicit	Crawler				( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view );
				~Crawler			() {}

	void		AddNext				( bv::model::BasicNodePtr node );
	bool		Finalize			();
	
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