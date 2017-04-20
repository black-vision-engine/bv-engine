#pragma once

#include "CoreDEF.h"


// forward declarations
namespace bv { namespace model {

class BasicNode;

} // model
} // bv

namespace bv { namespace nodelogic {

struct ScrollerNodesStates
{
	std::vector< bv::model::BasicNode * > m_nonActives;
	std::vector< bv::model::BasicNode * > m_actives;
	std::vector< bv::model::BasicNode * > m_visibles;

	SizeType			m_nodesCount;

	void Add			( bv::model::BasicNode * n );
    void Remove         ( bv::model::BasicNode * n );
	void Acivate		( bv::model::BasicNode * n );
    void Deacivate		( bv::model::BasicNode * n );
	void Visible		( bv::model::BasicNode * n );
	void NotVisible		( bv::model::BasicNode * n );
    void Clear          ();

	bool IsVisible		( bv::model::BasicNode * n ) const;
	bool IsActive		( bv::model::BasicNode * n ) const;
	bool IsNonActive	( bv::model::BasicNode * n ) const;

    bool Exist          ( bv::model::BasicNode * n ) const;

	SizeType ActiveSize		() const;
	SizeType NonActiveSize	() const;
	SizeType VisibleSize	() const;

	ScrollerNodesStates()
		: m_nodesCount( 0 )
	{}
};

} // nodelogic
} // bv