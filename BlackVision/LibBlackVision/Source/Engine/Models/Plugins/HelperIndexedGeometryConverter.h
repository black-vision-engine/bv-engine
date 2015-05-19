#pragma once

#include "IndexedGeometry.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {



class IndexedGeometryConverter
{
private:
	INDEX_TYPE				usedRangeIndex;			///< Points element in index table, that is the first unused element.
	int						remainnigTriangles;		///< Number of traingles to use.
	std::vector<bool>		usedTriangles;			///< Indicates which indicies(triangles) have been used.
protected:
	bool findNeighbour( INDEX_TYPE index1, INDEX_TYPE index2, INDEX_TYPE& foundIndex, std::vector<INDEX_TYPE>& indicies );
	bool findNeighbourPair( INDEX_TYPE index1, INDEX_TYPE& foundIndex1, INDEX_TYPE& foundIndex2, std::vector<INDEX_TYPE>& indicies );
	void findFirstUnusedTriangle( INDEX_TYPE& index1, std::vector<INDEX_TYPE>& indicies );
public:
	IndexedGeometryConverter(void);
	~IndexedGeometryConverter(void);

	void makeStrip( IndexedGeometry& mesh, Float3AttributeChannelPtr verts);
};


}
}
