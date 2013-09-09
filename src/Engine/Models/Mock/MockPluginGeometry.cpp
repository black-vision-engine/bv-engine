#include "MockPluginGeometry.h"
#include <cassert>

namespace bv {

// *********************************
//
const MockPluginGeometry::TVertexVec &          MockPluginGeometry::Vertices    () const
{
    return m_vertices;
}

// *********************************
//
const MockPluginGeometry::TIndexVec &           MockPluginGeometry::Indices     () const
{
    return m_indices;

}

// *********************************
//
MockPluginGeometry::Role  MockPluginGeometry::GetRole             () const
{
    return m_role;
}

// *********************************
//
void MockPluginGeometry::SetRole             ( MockPluginGeometry::Role role )
{
    m_role = role;
}

// *********************************
//
MockPluginGeometryRect::MockPluginGeometryRect  ( float w, float h )
    : m_width( w )
    , m_height( h )
    , m_adjacency( Adjacency::A_TRIANGLE_STRIP )
    , m_ccw( true )
{
    assert( w > 0.f );
    assert( h > 0.f );

    SetRole( Role::R_SOURCE );
    TriStripRep();
}

// *********************************
//
MockPluginGeometryRect::~MockPluginGeometryRect ()
{
}

// *********************************
//
bool    MockPluginGeometryRect::IsTimeInvariant          () const
{
    return true;
}

// *********************************
//
MockPluginGeometryRect::Adjacency   MockPluginGeometryRect::AdjacencyType           () const
{
    return m_adjacency;
}

// *********************************
//
void        MockPluginGeometryRect::ConvertToTraingles      ()
{
    if ( m_adjacency != Adjacency::A_TRIANGLES )
    {
        TriangleRep();
    }
}

// *********************************
//
void        MockPluginGeometryRect::ConvertToMesh           ()
{
    if ( m_adjacency != Adjacency::A_TRIANGLE_MESH )
    {
        MeshRep();
    }
}

// *********************************
//
void                MockPluginGeometryRect::TriStripRep             ()
{
    float wh = m_width / 2.f;
    float hh = m_height / 2.f;

    m_vertices.clear();
    m_indices.clear();

    glm::vec3 v0( -wh, -hh, 0.f ); 
	glm::vec3 v1( wh, -hh, 0.f ); 
	glm::vec3 v2( -wh,  hh, 0.f );
	glm::vec3 v3( wh,  hh, 0.f );

    if( IsCCW() )
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v3 );
    }
    else
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v3 );
    }

    m_adjacency = Adjacency::A_TRIANGLE_STRIP;
}

// *********************************
//
void                MockPluginGeometryRect::TriangleRep             ()
{
    float wh = m_width / 2.f;
    float hh = m_height / 2.f;

    m_vertices.clear();
    m_indices.clear();

    glm::vec3 v0( -wh, -hh, 0.f ); 
	glm::vec3 v1( wh, -hh, 0.f ); 
	glm::vec3 v2( -wh,  hh, 0.f );
	glm::vec3 v3( wh,  hh, 0.f );

    if( IsCCW() )
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v3 );
    }
    else
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v3 );
    }

    m_adjacency = Adjacency::A_TRIANGLES;
}

// *********************************
//
void                MockPluginGeometryRect::MeshRep                 ()
{
    float wh = m_width / 2.f;
    float hh = m_height / 2.f;

    m_vertices.clear();
    m_indices.clear();

    glm::vec3 v0( -wh, -hh, 0.f ); 
	glm::vec3 v1( wh, -hh, 0.f ); 
	glm::vec3 v2( -wh,  hh, 0.f );
	glm::vec3 v3( wh,  hh, 0.f );

    m_vertices.push_back( v0 );
    m_vertices.push_back( v2 );
    m_vertices.push_back( v1 );
    m_vertices.push_back( v3 );

    if( IsCCW() )
    {
        m_indices.push_back( 0 );
        m_indices.push_back( 1 );
        m_indices.push_back( 2 );
        m_indices.push_back( 2 );
        m_indices.push_back( 1 );
        m_indices.push_back( 3 );
    }
    else
    {
        m_indices.push_back( 0 );
        m_indices.push_back( 2 );
        m_indices.push_back( 1 );
        m_indices.push_back( 1 );
        m_indices.push_back( 2 );
        m_indices.push_back( 3 );
    }

    m_adjacency = Adjacency::A_TRIANGLE_MESH;
}

// *********************************
//
bool        MockPluginGeometryRect::IsCW                    () const
{
    return !m_ccw;
}

// *********************************
//
bool        MockPluginGeometryRect::IsCCW                   () const
{
    return m_ccw;
}

// *********************************
//
void        MockPluginGeometryRect::SetCW                   ()
{

    //TODO: set flag
    //Regenerate gemoetry
}

// *********************************
//
void        MockPluginGeometryRect::SetCCW                  ()
{
    //TODO: set flag
    //Regenerate gemoetry
}

}
