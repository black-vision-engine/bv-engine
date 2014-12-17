#include "Crawler.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

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
		LayoutNodes();
		m_isFinalized = true;
	}

	return m_isFinalized;
}

// *******************************
//
void		Crawler::LayoutNodes		()
{
	auto length = m_nodes.size();
	if( length > 1 )
	{
		Float32 currShift = 0.f;
		for( SizeType i = 1; i < length; ++i )
		{
			currShift += m_nodes[ i -1 ]->GetAABB().Width();

			auto trPlugin = m_nodes[ i ]->GetPlugin( "transform" );
			if( trPlugin )
			{
				auto trParam = trPlugin->GetParameter( "simple_transform" );
				model::SetParameterTranslation( trParam, 0, 0.0f, glm::vec3( currShift, 0.0f, 0.0f ) );
			}
		}
	}
}

} // widgets
} // bv