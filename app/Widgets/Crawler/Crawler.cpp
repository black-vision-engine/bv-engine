#include "Crawler.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include <Windows.h>

namespace bv { namespace widgets { 

// *******************************
//
CrawlerPtr	Crawler::Create				( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view )
{
	return std::make_shared< Crawler >( parent, view );
}

// *******************************
//
Crawler::Crawler						( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view )
	: m_parentNode( parent )
	, m_isFinalized( false )
	, m_view( view )
	, m_started( false )
	, m_currTime( 0 )
	, m_speed( 0.f )
{}


// *******************************
//
void		Crawler::AddNext			( bv::model::BasicNodePtr node )
{
	if(! m_isFinalized )
	{
		m_parentNode->AddChild( node );
		m_nodes.push_back( node.get() );
	}
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
		for( auto n : m_nodes )
		{
			m_nodesStates.Add( n );
			SetActiveNode( n, true );
		}
		LayoutNodes();
		m_isFinalized = true;
	}

	return m_isFinalized;
}

// *******************************
//
void		Crawler::SetSpeed			( Float32 speed )
{
	m_speed = speed;
}

// *******************************
//
void		Crawler::LayoutNodes		()
{
	auto length = m_nodes.size();
	if( length > 1 )
	{
		Float32 currShift = 0.f;

		m_shifts[ m_nodes[ 0 ] ] = currShift;

		for( SizeType i = 1; i < length; ++i )
		{
			currShift += m_nodes[ i - 1 ]->GetAABB().Width();

			m_shifts[ m_nodes[ i ] ] = currShift;

			auto trPlugin = m_nodes[ i ]->GetPlugin( "transform" );
			if( trPlugin )
			{
				auto trParam = trPlugin->GetParameter( "simple_transform" );
				model::SetParameterTranslation( trParam, 0, 0.0f, glm::vec3( currShift, 0.0f, 0.0f ) );
				UpdateVisibility( m_nodes[ i ] );
			}
		}
	}
}

// *******************************
//
void		Crawler::Start			()
{
	if(! m_started )
	{
		m_started = true;
		m_currTime = GetTickCount();
	}
}

// *******************************
//
void		Crawler::Stop			()
{
	m_started = false;
}

// *******************************
//
void		Crawler::Update				( TimeType )
{
	if( m_started )
	{
		auto t = GetTickCount();
		auto shift = m_speed * ( ( t - m_currTime ) / 1000.f );

		m_currTime = t;

		for( auto elem : m_shifts )
			m_shifts[ elem.first ] -= shift;

		UpdateTransforms();
	}
}

// *******************************
//
void		Crawler::UpdateTransforms	()
{
	for( auto elem : m_shifts )
	{
		if( IsActive( elem.first ) )
		{
			auto trPlugin = elem.first->GetPlugin( "transform" );
			if( trPlugin )
			{
				auto trParam = trPlugin->GetParameter( "simple_transform" );
				model::SetParameterTranslation( trParam, 0, 0.0f, glm::vec3( elem.second, 0.0f, 0.0f ) );
				UpdateVisibility( elem.first );

				if( m_nodesStates.ActiveSize() == m_nodesStates.VisibleSize() )
					NotifyNoMoreNodes();
			}
		}
	}
}

// *******************************
//
void		Crawler::UpdateVisibility	( bv::model::BasicNode * n )
{
	auto currVisibility = m_nodesStates.IsVisible( n );
	bool newVisibility = n->GetAABB().HasNonEmptyIntersection( *m_view );

	if( currVisibility != newVisibility )
	{
		if( newVisibility )
			m_nodesStates.Visible( n );
		else
			m_nodesStates.NotVisible( n );

		NotifyVisibilityChanged( n, newVisibility );
	}
}

// *******************************
//
void		Crawler::NotifyVisibilityChanged( const bv::model::BasicNode * n, bool visibility ) const
{
	printf( "Visibility of %p changed on %i \n", n, visibility );
}

// *******************************
//
void		Crawler::NotifyNoMoreNodes( ) const
{
	printf( "No more nodes \n" );
}

// *******************************
//
void		Crawler::SetActiveNode		( bv::model::BasicNode * n, bool isActive )
{
	if( isActive )
		m_nodesStates.Acivate( n );
	else
		m_nodesStates.Deacivate( n );
}

// *******************************
//
bool		Crawler::IsActive			( bv::model::BasicNode * n )
{
	return m_nodesStates.IsActive( n );
}

// *******************************
//
void CrawlerNodesStates::Add			( bv::model::BasicNode * n )
{
	m_nonActives.push_back( n );
}

// *******************************
//
void CrawlerNodesStates::Acivate		( bv::model::BasicNode * n )
{
	for( SizeType i = 0; i < m_nonActives.size(); ++i )
		if( m_nonActives[ i ] == n )
		{
			m_actives.push_back( n );
			m_nonActives.erase( m_nonActives.begin() + i );
		}
}

// *******************************
//
void CrawlerNodesStates::Deacivate		( bv::model::BasicNode * n )
{
	for( SizeType i = 0; i < m_actives.size(); ++i )
		if( m_actives[ i ] == n )
		{
			m_nonActives.push_back( n );
			m_actives.erase( m_actives.begin() + i );
		}

	NotVisible( n );
}

// *******************************
//
void CrawlerNodesStates::Visible		( bv::model::BasicNode * n )
{
	for( SizeType i = 0; i < m_actives.size(); ++i )
		if( m_actives[ i ] == n )
			m_visibles.push_back( n );
}

// *******************************
//
void CrawlerNodesStates::NotVisible		( bv::model::BasicNode * n )
{
	for( SizeType i = 0; i < m_visibles.size(); ++i )
		if( m_visibles[ i ] == n )
		{
			m_nonActives.push_back( n );
			m_visibles.erase( m_visibles.begin() + i );
		}
}

// *******************************
//
bool CrawlerNodesStates::IsVisible		( bv::model::BasicNode * n ) const
{
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
	for( SizeType i = 0; i < m_actives.size(); ++i )
		if( m_actives[ i ] == n )
			return true;
	return false;
}

// *******************************
//
bool CrawlerNodesStates::IsNonActive	( bv::model::BasicNode * n ) const
{
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