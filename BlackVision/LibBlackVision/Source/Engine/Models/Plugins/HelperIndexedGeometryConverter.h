#pragma once

#include "IndexedGeometry.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {



class IndexedGeometryConverter
{
private:
	unsigned int			usedRangeIndex;		///< Points element in index table, that is the first unused element.
	unsigned int			remainnigTriangles;	///< Number of traingles to use.
	std::vector<bool>		usedTriangles;		///< Indicates which indicies(triangles) have been used.
protected:
	bool findNeighbour( unsigned short index1, unsigned short index2, unsigned short& foundIndex, std::vector<unsigned short>& indicies );
	bool findNeighbourPair( unsigned short index1, unsigned short& foundIndex1, unsigned short& foundIndex2, std::vector<unsigned short>& indicies );
	void findFirstUnusedTriangle( unsigned short& index1, std::vector<unsigned short>& indicies );
public:
	IndexedGeometryConverter(void);
	~IndexedGeometryConverter(void);

	void makeStrip( IndexedGeometry& mesh, Float3AttributeChannelPtr verts);
};


}
}
