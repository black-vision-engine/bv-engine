#include "PluginGeometryRect.h"
#include <cassert>

#if 0

namespace bv {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string PluginGeometryRectPD::pluginName( "geometry.rect" );

// *************************************
//
PluginGeometryRectPD::PluginGeometryRectPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *********************************
//
PluginGeometryRect::PluginGeometryRect  ( float w, float h )
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
PluginGeometryRect::~PluginGeometryRect ()
{
}

// *********************************
//
bool    PluginGeometryRect::IsTimeInvariant          () const
{
    return true;
}

// *********************************
//
PluginGeometryRect::Adjacency   PluginGeometryRect::AdjacencyType           () const
{
    return m_adjacency;
}

// *********************************
//
void        PluginGeometryRect::ConvertToTraingles      ()
{
    if ( m_adjacency != Adjacency::A_TRIANGLES )
    {
        TriangleRep();
    }
}

// *********************************
//
void        PluginGeometryRect::ConvertToMesh           ()
{
    if ( m_adjacency != Adjacency::A_TRIANGLE_MESH )
    {
        MeshRep();
    }
}

// *********************************
//
void                PluginGeometryRect::TriStripRep             ()
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
void                PluginGeometryRect::TriangleRep             ()
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
void                PluginGeometryRect::MeshRep                 ()
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
    m_vertices.push_back( v1 );
    m_vertices.push_back( v2 );
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
bool        PluginGeometryRect::IsCW                    () const
{
    return !m_ccw;
}

// *********************************
//
bool        PluginGeometryRect::IsCCW                   () const
{
    return m_ccw;
}

// *********************************
//
void        PluginGeometryRect::SetCW                   ()
{
    float wh = m_width / 2.f;
    float hh = m_height / 2.f;

    m_vertices.clear();
    m_indices.clear();

    glm::vec3 v0( -wh, -hh, 0.f ); 
    glm::vec3 v1( wh, -hh, 0.f ); 
    glm::vec3 v2( -wh,  hh, 0.f );
    glm::vec3 v3( wh,  hh, 0.f );

    if(m_adjacency == Adjacency::A_TRIANGLES)
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v3 );
    }
    else if (m_adjacency == Adjacency::A_TRIANGLE_STRIP)
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v3 );
    }
    else if (m_adjacency == Adjacency::A_TRIANGLE_MESH)
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v3 );

        m_indices.push_back( 0 );
        m_indices.push_back( 2 );
        m_indices.push_back( 1 );
        m_indices.push_back( 1 );
        m_indices.push_back( 2 );
        m_indices.push_back( 3 );
    }
    else
    {
        assert(!"Impossible");
    }

    m_ccw = false;
}

// *********************************
//
void        PluginGeometryRect::SetCCW                  ()
{   
    float wh = m_width / 2.f;
    float hh = m_height / 2.f;

    m_vertices.clear();
    m_indices.clear();

    glm::vec3 v0( -wh, -hh, 0.f ); 
    glm::vec3 v1( wh, -hh, 0.f ); 
    glm::vec3 v2( -wh,  hh, 0.f );
    glm::vec3 v3( wh,  hh, 0.f );

    if(m_adjacency == Adjacency::A_TRIANGLES)
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v3 );
    }
    else if (m_adjacency == Adjacency::A_TRIANGLE_STRIP)
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v3 );
    }
    else if (m_adjacency == Adjacency::A_TRIANGLE_MESH)
    {
        m_vertices.push_back( v0 );
        m_vertices.push_back( v1 );
        m_vertices.push_back( v2 );
        m_vertices.push_back( v3 );

        m_indices.push_back( 0 );
        m_indices.push_back( 1 );
        m_indices.push_back( 2 );
        m_indices.push_back( 2 );
        m_indices.push_back( 1 );
        m_indices.push_back( 3 );
    }
    else
    {
        assert(!"Impossible");
    }

    m_ccw = true;
}

void    PluginGeometryRect::Update(float t)
{
    if(!IsTimeInvariant())
    {
        assert(!"This plugins should be time invariant");
    }
}

}

#endif