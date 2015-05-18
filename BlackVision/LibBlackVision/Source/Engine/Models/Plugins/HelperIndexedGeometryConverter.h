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
	bool findNeighbour( unsigned short index1, unsigned short index2, unsigned short& foundIndex, std::vector<unsigned short>& indicies, std::vector<glm::vec3>& verticies );
	bool findNeighbour( unsigned short index1, unsigned short& foundIndex, std::vector<unsigned short>& indicies, std::vector<glm::vec3>& verticies );
	void addFirstUnusedPoints( unsigned short& index1, unsigned short& index2, std::vector<unsigned short>& indicies, std::vector<glm::vec3>& verticies, Float3AttributeChannelPtr verts );
public:
	IndexedGeometryConverter(void);
	~IndexedGeometryConverter(void);

	void makeStrip( IndexedGeometry& mesh, Float3AttributeChannelPtr verts);
};


}
}
