#pragma once

#include "CoreDEF.h"

#include <vector>

// forward declarations
namespace bv { namespace model {

class BasicNode;
typedef std::shared_ptr< BasicNode > BasicNodePtr;

} // model
} // bv

namespace bv { namespace nodelogic {

struct CrawlerNodesStates
{
	std::vector< bv::model::BasicNode * > m_nonActives;
	std::vector< bv::model::BasicNode * > m_actives;
	std::vector< bv::model::BasicNode * > m_visibles;

	SizeType			m_nodesCount;

	void Add			( bv::model::BasicNode * n );
	void Acivate		( bv::model::BasicNode * n );
	void Visible		( bv::model::BasicNode * n );
	void NotVisible		( bv::model::BasicNode * n );

	bool IsVisible		( bv::model::BasicNode * n ) const;
	bool IsActive		( bv::model::BasicNode * n ) const;
	bool IsNonActive	( bv::model::BasicNode * n ) const;

    bool Exist          ( bv::model::BasicNode * n ) const;

	SizeType ActiveSize		() const;
	SizeType NonActiveSize	() const;
	SizeType VisibleSize	() const;

	CrawlerNodesStates()
		: m_nodesCount( 0 )
	{}
};

} // nodelogic
} // bv