#include "Mesh.h"



Mesh::Mesh()
{
	m_current = -1;
	m_singleSegment = false;
}

Mesh::Mesh( bool singleSegment )
{
	m_singleSegment = singleSegment;

	if( m_singleSegment )
	{
		m_current = 0;
		m_segments.push_back( MeshSegment() );
	}
}

void		Mesh::AddPoint( float x, float y, float z )
{
	m_segments[ m_current ].push_back( glm::vec3( x, y, z ) );
}

void		Mesh::AddPoint( glm::vec3 & point )
{
	m_segments[ m_current ].push_back( point );
}

void		Mesh::Begin()
{
	if( !m_singleSegment )
	{
		m_segments.push_back( MeshSegment() );
		m_current++;
	}
}

void		Mesh::End()
{}
