#include "Crawler.h"

#include "Engine/Models/BasicNode.h"

namespace bv { namespace widgets { 

// *******************************
//
CrawlerPtr	Crawler::Create				( bv::model::BasicNode * parent )
{
	return std::make_shared< Crawler >( parent );
}

// *******************************
//
Crawler::Crawler						( bv::model::BasicNode * parent )
	: m_parentNode( parent )
{}


// *******************************
//
void		Crawler::AddNext			( bv::model::BasicNodePtr node )
{
	if(! m_isFinalized )
		m_parentNode->AddChild( node );
	else
		assert(!"Crawler: Cannot add node after finalization!");
}

// *******************************
//
bool		Crawler::Finalize			()
{
	if( m_isFinalized )
		assert(!"Crawler: Already finalized!");
	else
	{
		m_isFinalized = true;
	}

	return m_isFinalized;
}

} // widgets
} // bv