#pragma once

#include "IndexedGeometry.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {



class IndexedGeometryConverter
{
private:

    float                   m_epsilon;              ///< Epsilon for comparing positions.

    unsigned int			usedRangeIndex;			///< Points element in index table, that is the first unused element.
    unsigned int			remainnigTriangles;		///< Number of traingles to use.
    std::vector<bool>		usedTriangles;			///< Indicates which indicies(triangles) have been used.

    // These variables can hold conversion data. You can apply the same conversion to other geometry channels.
    bool                    m_rememberConversion;
    std::vector< int >      m_conversionIndicies;

protected:

    void        moveRangeIndex          ();
    bool        findNeighbour           ( unsigned int index1, unsigned int index2, unsigned int& foundIndex, std::vector< IndexType >& indicies );
    bool        findNeighbourPair       ( unsigned int index1, unsigned int& foundIndex1, unsigned int& foundIndex2, std::vector< IndexType >& indicies );
    void        findFirstUnusedTriangle ( unsigned int& index1, std::vector< IndexType >& indicies );

public:
    IndexedGeometryConverter            ();
    IndexedGeometryConverter            ( float epsilon );
    ~IndexedGeometryConverter           ();

    void                MakeStrip               ( IndexedGeometry & mesh, Float3AttributeChannelPtr verts );
    void                MakeTriangles           ( IndexedGeometry & mesh, Float3AttributeChannelPtr verts );
    void                MakeTriangles           ( const std::vector< glm::vec3 > & verticies, const std::vector< IndexType > & indicies, Float3AttributeChannelPtr verts );
    void                MakeTriangles           ( const std::vector< glm::vec2 > & srcUVs, const std::vector< IndexType > & indicies, Float2AttributeChannelPtr uvs );

    IndexedGeometry     MakeIndexGeomFromStrips     ( Float3AttributeChannelPtr verts );
    IndexedGeometry     MakeIndexGeomFromTriangles  ( Float3AttributeChannelPtr verts );

    // Converter can hold conversion data. You can apply the same conversion to other geometry channels like normals or UVs.
    // This works only with functions MakeIndexGeomFromStrips and MakeIndexGeomFromTriangles.
    // Call ConvertFromMemory function to make conversion.
    void                RememberConversionIndicies  ( bool value );
    
    std::vector< glm::vec3 >    ConvertFromMemory           ( Float3AttributeChannelPtr channel );
    std::vector< glm::vec2 >    ConvertFromMemory           ( Float2AttributeChannelPtr channel );

private:

    template< typename ChannelType, typename AttribType >
    std::vector< AttribType >   ConvertFromMemoryImpl       ( ChannelType channel );

};


}
}
