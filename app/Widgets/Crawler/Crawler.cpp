#include "Crawler.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "CrawlerEvents.h"


#include <algorithm>
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
	, m_interspace( 0.1f )
{}


// *******************************
//
void		Crawler::AddNext			( bv::model::BasicNodePtr node )
{
	if(! m_isFinalized )
	{
		m_parentNode->AddChild( node );
		m_nodesStates.Add( node.get() );
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
		auto copy = m_nodesStates.m_nonActives;
		for( auto n : copy )	
			SetActiveNode( n );

		LayoutNodes();
		m_isFinalized = true;
	}

	return m_isFinalized;
}

// *******************************
//
void		Crawler::SetInterspace		( Float32 interspace )
{
	m_interspace = interspace;
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
	auto length = m_nodesStates.ActiveSize();
	if( length > 0 )
	{
		Float32 currShift = m_view->xmax + m_interspace;

		m_shifts[ m_nodesStates.m_actives[ 0 ] ] = currShift;

		for( SizeType i = 1; i < length; ++i )
		{
			currShift += m_nodesStates.m_actives[ i - 1 ]->GetAABB().Width() + m_interspace;

			m_shifts[ m_nodesStates.m_actives[ i ] ] = currShift;
		}

		UpdateTransforms();
	}
}

// *******************************
//
void		Crawler::Start			()
{
	if(! m_started )
	{
		m_started = true;
		m_currTime = timeGetTime();
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
		auto t = timeGetTime();
		auto shift = m_speed * ( ( t - m_currTime ) / 1000.f );

		m_currTime = t;

		if( shift > 0.f )
		{
			for( auto elem : m_shifts )
				m_shifts[ elem.first ] -= shift;

			UpdateTransforms();
		}
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
			}
		}
	}

	auto copy = m_nodesStates.m_actives;

	for( auto n : copy )
		UpdateVisibility( n );

	if( m_nodesStates.ActiveSize() == m_nodesStates.VisibleSize() )
		NotifyNoMoreNodes();
}

// *******************************
//
void		Crawler::UpdateVisibility	( bv::model::BasicNode * n )
{
	auto currVisibility = m_nodesStates.IsVisible( n );
	auto nAABB = n->GetAABB();
	bool newVisibility = nAABB.HasNonEmptyIntersection( *m_view );

	if( currVisibility != newVisibility )
	{
		if( newVisibility )
		{
			m_nodesStates.Visible( n );
			NotifyVisibilityChanged( n, newVisibility );
		}
		else if( IsActive( n ) )
		{
			m_nodesStates.NotVisible( n );
			NotifyVisibilityChanged( n, newVisibility );
		}
	}
}

// *******************************
//
void		Crawler::NotifyVisibilityChanged( bv::model::BasicNode * n, bool visibility )
{
	auto & eventManager = GetDefaultEventManager();

	if( visibility )
		eventManager.TriggerEvent( std::make_shared< NodeAppearingCrawlerEvent >( shared_from_this(), n ) );
	else
		eventManager.TriggerEvent( std::make_shared< NodeLeavingCrawlerEvent >( shared_from_this(), n ) );

	printf( "Visibility of %p changed on %i \n", n, visibility );
	printf( "Active : %i NonActive: %i Visible %i \n", m_nodesStates.ActiveSize(), m_nodesStates.NonActiveSize(), m_nodesStates.VisibleSize() );
}

namespace 
{
	const static std::wstring examples[] = 
	{
		L"Jasiu kup kie�bas� !!",
		L"wieloj�zyczny projekt internetortej tre�ci. Funkcjonuje wykorzystuj�c",
		L"Wikipedia powsta�a 15 stycznia ert�w i nieistniej�cej ju� Nupedii. ",
		L"iostrzane. Wikipedia jest jedn�], a wiele stron uruchomi�o jej mirrory lub forki.",
		L"Wsp�za�o�yciel Wikipedii Jimmyia wieloj�zycznej",
		L"wolnej encyklopedii o najwy�szyw�asnym j�zyku�[8].",
		L"Kontrowersje budzi wiarygodno��e�ci artyku��w ",
		L"i brak weryfikacji kompetencji .",
		L"Z drugiej",
		L"strony mo�liwo�� swobodnej dyst �r�d�em informacji",
		L"Jasiu kup kie�bas� !!",
	};

	auto exampleSize = sizeof( examples ) / sizeof( std::wstring );
}

// *******************************
//
void		Crawler::NotifyNoMoreNodes( )
{
	auto & eventManager = GetDefaultEventManager();
	eventManager.TriggerEvent( std::make_shared< NoMoreNodesCrawlerEvent >( shared_from_this() ) );

	auto n = GetNonActiveNode();
	if( n )
	{
		auto i = rand() % exampleSize;
		auto textNode = n->GetChild( "Text" );
		if( textNode )
		{
			auto pl = textNode->GetPlugin( "text" );

			if( pl )
			{
				model::DefaultTextPlugin::SetText( pl, examples[ i ] );

				EnqueueNode( n );
			}
		}
	}
}

// *******************************
//
void		Crawler::SetActiveNode		( bv::model::BasicNode * n )
{
	m_nodesStates.Acivate( n );
}

// *******************************
//
bool		Crawler::IsActive			( bv::model::BasicNode * n )
{
	return m_nodesStates.IsActive( n );
}

// *******************************
//
model::BasicNode *	Crawler::GetNonActiveNode()
{
	if ( m_nodesStates.NonActiveSize() > 0 )
		return m_nodesStates.m_nonActives[ 0 ];
	else
		return nullptr;
}

// *******************************
//
void		Crawler::EnqueueNode			( model::BasicNode * n)
{
	if( m_nodesStates.IsNonActive( n ) )
	{
		auto activeSize = m_nodesStates.ActiveSize();
		if( activeSize > 0 )
		{
			auto lastActiveNode = m_nodesStates.m_actives[ activeSize - 1 ];

			auto nodeShift = m_shifts[ lastActiveNode ] + lastActiveNode->GetAABB().Width() + m_interspace;

			nodeShift = max( nodeShift, m_view->xmax );

			m_shifts[ n ] = nodeShift;
			m_nodesStates.Acivate( n );
			UpdateVisibility( n );
		}
	}
}


} // widgets
} // bv