#pragma once

#include "System/BasicTypes.h"

#include <vector>

// forward references
namespace bv { namespace model {

class BasicNode;

typedef std::shared_ptr< BasicNode > BasicNodePtr;

} // model
} // bv

namespace bv { namespace widgets {

class Crawler;

DEFINE_PTR_TYPE( Crawler )
DEFINE_CONST_PTR_TYPE( Crawler )

class Crawler
{
private:
	bool									m_isFinalized;
	bv::model::BasicNode *					m_parentNode;
	std::vector< bv::model::BasicNode * >	m_nodes;

	void		LayoutNodes			();

public:
	void		AddNext				( bv::model::BasicNodePtr node );
	bool		Finalize			();
	explicit	Crawler				( bv::model::BasicNode * parent );

	static		CrawlerPtr Create	( bv::model::BasicNode * parent );
};

} 
} // bv