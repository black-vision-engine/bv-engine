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

protected:

	void        moveRangeIndex          ();
	bool        findNeighbour           ( unsigned int index1, unsigned int index2, unsigned int& foundIndex, std::vector<INDEX_TYPE>& indicies );
	bool        findNeighbourPair       ( unsigned int index1, unsigned int& foundIndex1, unsigned int& foundIndex2, std::vector<INDEX_TYPE>& indicies );
	void        findFirstUnusedTriangle ( unsigned int& index1, std::vector<INDEX_TYPE>& indicies );

public:
	IndexedGeometryConverter            ();
    IndexedGeometryConverter            ( float epsilon );
	~IndexedGeometryConverter           ();

	void                MakeStrip               ( IndexedGeometry & mesh, Float3AttributeChannelPtr verts );
    void                MakeTriangles           ( IndexedGeometry & mesh, Float3AttributeChannelPtr verts );

    IndexedGeometry     MakeIndexGeomFromStrips     ( Float3AttributeChannelPtr verts );
    IndexedGeometry     MakeIndexGeomFromTrinagles  ( Float3AttributeChannelPtr verts );
};


}
}
