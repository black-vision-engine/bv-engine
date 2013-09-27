#pragma once

#include <vector>
#include "glm/glm.hpp"


namespace bv {


class MockPluginGeometry;

typedef std::vector< MockPluginGeometry * > TMockGeometryPluginVec;

class MockPluginGeometry
{
public:

    typedef std::vector< glm::vec3 >    TVertexVec;
    typedef std::vector< int >          TIndexVec;

    enum class Adjacency : int
    {
        A_TRIANGLES = 0,
        A_TRIANGLE_STRIP,
        A_TRIANGLE_MESH,

        A_TOTAL
    };

    enum class Role : int
    {
        R_SOURCE = 0,
        R_PROCESSOR,

        R_TOTAL
    };

protected:

    TVertexVec  m_vertices;
    TIndexVec   m_indices;

    Role        m_role;

public:

    const TVertexVec &          Vertices            () const;
    const TIndexVec &           Indices             () const;

    Role                        GetRole             () const;
    void                        SetRole             ( Role role );

    virtual bool                IsTimeInvariant     () const = 0;
    virtual Adjacency           AdjacencyType       () const = 0;

    virtual void                ConvertToTraingles  () = 0;
    virtual void                ConvertToMesh       () = 0;

    virtual bool                IsCW                () const = 0;
    virtual bool                IsCCW               () const = 0;

    virtual void                SetCW               () = 0;
    virtual void                SetCCW              () = 0;

    virtual ~MockPluginGeometry() = 0;
};

class MockPluginGeometryRect : public MockPluginGeometry
{
private:

    typedef MockPluginGeometry::Adjacency Adjacency;

private:

    float       m_width;
    float       m_height;

    bool        m_ccw;
    Adjacency   m_adjacency;

public:

                        MockPluginGeometryRect  ( float w = 1.0f, float h = 1.0f );
    virtual             ~MockPluginGeometryRect ();

    virtual bool        IsTimeInvariant         () const;
    virtual Adjacency   AdjacencyType           () const;

    virtual void        ConvertToTraingles      ();
    virtual void        ConvertToMesh           ();

    virtual bool        IsCW                    () const;
    virtual bool        IsCCW                   () const;

    virtual void        SetCW                   ();
    virtual void        SetCCW                  ();

private:

    void                TriStripRep             ();
    void                TriangleRep             ();
    void                MeshRep                 ();

};

}
