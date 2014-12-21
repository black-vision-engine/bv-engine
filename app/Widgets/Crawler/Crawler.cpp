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
		LayoutNodes();
		m_isFinalized = true;
		for( auto n : m_nodes )
			SetActiveNode( n, true );
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
		auto trPlugin = elem.first->GetPlugin( "transform" );
		if( trPlugin )
		{
			auto trParam = trPlugin->GetParameter( "simple_transform" );
			model::SetParameterTranslation( trParam, 0, 0.0f, glm::vec3( elem.second, 0.0f, 0.0f ) );
			UpdateVisibility( elem.first );
		}
	}
}

// *******************************
//
void		Crawler::UpdateVisibility	( bv::model::BasicNode * n )
{
	auto & currVisibility = m_visibilities[ n ];
	bool newVisibility = n->GetAABB().HasNonEmptyIntersection( *m_view );

	if( currVisibility != newVisibility )
	{
		currVisibility = newVisibility;
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
void		Crawler::SetActiveNode		( bv::model::BasicNode * n, bool isActive )
{
	m_activeNodes[ n ] = isActive;
}

} // widgets
} // bv