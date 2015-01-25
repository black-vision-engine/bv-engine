#include "CrawlerNodesStates.h"

#include <cassert>

namespace bv { namespace widgets { 

// *******************************
//
void CrawlerNodesStates::Add			( bv::model::BasicNode * n )
{
	m_nonActives.push_back( n );
	m_nodesCount++;
	assert( ActiveSize() + NonActiveSize() == m_nodesCount );
}

// *******************************
//
void CrawlerNodesStates::Acivate		( bv::model::BasicNode * n )
{
	assert( ActiveSize() + NonActiveSize() == m_nodesCount );
	for( SizeType i = 0; i < m_nonActives.size(); ++i )
		if( m_nonActives[ i ] == n )
		{
			m_actives.push_back( n );
			m_nonActives.erase( m_nonActives.begin() + i );
			assert( ActiveSize() + NonActiveSize() == m_nodesCount );
			return;
		}
}

//// *******************************
////
//void CrawlerNodesStates::Deacivate		( bv::model::BasicNode * n )
//{
//	for( SizeType i = 0; i < m_actives.size(); ++i )
//		if( m_actives[ i ] == n )
//		{
//			m_nonActives.push_back( n );
//			m_actives.erase( m_actives.begin() + i );
//		}
//}

// *******************************
//
void CrawlerNodesStates::Visible		( bv::model::BasicNode * n )
{
	assert( ActiveSize() + NonActiveSize() == m_nodesCount );
	for( SizeType i = 0; i < m_actives.size(); ++i )
		if( m_actives[ i ] == n )
		{
			m_visibles.push_back( n );
			assert( ActiveSize() + NonActiveSize() == m_nodesCount );
			return;
		}
}

// *******************************
//
void CrawlerNodesStates::NotVisible		( bv::model::BasicNode * n )
{
	assert( ActiveSize() + NonActiveSize() == m_nodesCount );
	for( SizeType i = 0; i < m_visibles.size(); ++i )
		if( m_visibles[ i ] == n )
		{
			m_nonActives.push_back( n );
			m_visibles.erase( m_visibles.begin() + i );
			//assert( IsActive( n ) );
			for( SizeType j = 0; j < m_actives.size(); ++j )
				if( m_actives[ j ] == n )
					m_actives.erase( m_actives.begin() + j );

			assert(! IsActive( n ) );

			assert( ActiveSize() + NonActiveSize() == m_nodesCount );
			return;
		}

	//Deacivate( n );
}

// *******************************
//
bool CrawlerNodesStates::IsVisible		( bv::model::BasicNode * n ) const
{
	assert( ActiveSize() + NonActiveSize() == m_nodesCount );
	for( SizeType i = 0; i < m_visibles.size(); ++i )
		if( m_visibles[ i ] == n )
		{
			assert( IsActive( n ) );
			return true;
		}
	return false;
}

// *******************************
//
bool CrawlerNodesStates::IsActive		( bv::model::BasicNode * n ) const
{
	assert( ActiveSize() + NonActiveSize() == m_nodesCount );
	for( SizeType i = 0; i < m_actives.size(); ++i )
		if( m_actives[ i ] == n )
			return true;
	return false;
}

// *******************************
//
bool CrawlerNodesStates::IsNonActive	( bv::model::BasicNode * n ) const
{
	assert( ActiveSize() + NonActiveSize() == m_nodesCount );
	for( SizeType i = 0; i < m_nonActives.size(); ++i )
		if( m_nonActives[ i ] == n )
		{
			assert(! IsVisible( n ) );
			return true;
		}

	assert( IsActive( n ) );

	return false;
}

// *******************************
//
SizeType CrawlerNodesStates::ActiveSize		() const
{
	return m_actives.size();
}

// *******************************
//
SizeType CrawlerNodesStates::NonActiveSize	() const
{
	return m_nonActives.size();
}

// *******************************
//
SizeType CrawlerNodesStates::VisibleSize	() const
{
	return m_visibles.size();
}

} // widgets
} // bv