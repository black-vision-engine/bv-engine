#pragma once


#include "IndexedGeometry.h"

namespace bv { namespace model {


enum VertexType : char
{
	SMOOTH_VERTEX = 0,
	CREASE_VERTEX = 4,
	CORNER_VERTEX = 3,
	REGULAR_CREASE_VERTEX = 1,
	NON_REGULAR_CREASE_VERTEX = 2
};


struct VertexData
{
	VertexType		type;
	unsigned short	numNeighbours;
	float			sumWeigths;

	VertexData() { type = SMOOTH_VERTEX; numNeighbours = 0; sumWeigths = 0.0f; }
};

class HelperSmoothMesh
{
private:
	virtual float				computeCenterVertexWeight( unsigned short numNeighbours );
protected:
	void						privateSmooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& sharpEdges, unsigned int tesselation, IndexedGeometry& resultMesh );
	std::vector<INDEX_TYPE>		tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh, std::vector<INDEX_TYPE> sharpEdges );
	void						moveVerticies( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& sharpEdges, IndexedGeometry& resultMesh );
	void						oldMoveVerticies( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& sharpEdges, IndexedGeometry& resultMesh );
	bool						findVertex( const std::vector<glm::vec3>& verticies, glm::vec3 vertex, INDEX_TYPE& index, INDEX_TYPE startIndex );
	std::vector<INDEX_TYPE>		findAllNeighbours( INDEX_TYPE index, const std::vector<INDEX_TYPE>& indicies, INDEX_TYPE maxIndex );
	std::vector<float>			makeWeightTable( INDEX_TYPE index, std::vector<INDEX_TYPE>& vertexNeighbours, std::vector<INDEX_TYPE>& sharpEdges, bool edgePoint );
	std::vector<INDEX_TYPE>		findNeighboursForEdgeVertex( INDEX_TYPE index, const std::vector<INDEX_TYPE>& preIndicies, const std::vector<INDEX_TYPE>& postIndicies, INDEX_TYPE maxIndex );
	glm::vec3					computeVertexNewPosition( INDEX_TYPE index, const std::vector<INDEX_TYPE>& vertexNeighbours, const std::vector<float>& vertexWeights, const std::vector<glm::vec3>& verticies, bool ignoreIndex );
	void						addIfNotExists( INDEX_TYPE index, std::vector<INDEX_TYPE>& vertexNeighbours );
	VertexType					computeVertexType( int i, std::vector<INDEX_TYPE> sharpEdges );
	float						computeVertexWeight( int vertexIndex, int secondVertexIndex, VertexType vertexType, std::vector<INDEX_TYPE> sharpEdges );
	bool						isSharpEdge( int index1, int index2, std::vector<INDEX_TYPE> sharpEdges );
	float						computeEdgeVertexWeight( INDEX_TYPE edgeIndex1, INDEX_TYPE edgeIndex2, std::vector<VertexData>& vertexData );
public:
	HelperSmoothMesh(void);
	~HelperSmoothMesh(void);

	void						smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	IndexedGeometry				smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation );
};

} //model
} //bv