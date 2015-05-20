#pragma once


#include "IndexedGeometry.h"

namespace bv { namespace model {





class HelperSmoothMesh
{
private:

protected:
	void privateSmooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& sharpEdges, unsigned int tesselation, IndexedGeometry& resultMesh );
	void tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh );
	void moveVerticies( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& sharpEdges, IndexedGeometry& resultMesh );
	bool findVertex( const std::vector<glm::vec3>& verticies, glm::vec3 vertex, INDEX_TYPE& index );
	std::vector<INDEX_TYPE> findAllNeighbours( INDEX_TYPE index, const std::vector<INDEX_TYPE>& indicies );
	std::vector<float> makeWeightTable( INDEX_TYPE index, std::vector<INDEX_TYPE>& vertexNeighbours, std::vector<INDEX_TYPE>& sharpEdges );
	glm::vec3 computeVertexNewPosition( INDEX_TYPE index, const std::vector<INDEX_TYPE>& vertexNeighbours, const std::vector<float>& vertexWeights, const std::vector<glm::vec3>& verticies, bool ignoreIndex );
	std::vector<INDEX_TYPE> findNeighboursForEdgeVertex( INDEX_TYPE index, const std::vector<INDEX_TYPE>& preIndicies, const std::vector<INDEX_TYPE>& postIndicies );
public:
	HelperSmoothMesh(void);
	~HelperSmoothMesh(void);

	void smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	IndexedGeometry smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation );
};

} //model
} //bv