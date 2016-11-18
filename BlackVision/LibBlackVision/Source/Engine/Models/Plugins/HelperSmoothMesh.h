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
	void						privateSmooth       ( IndexedGeometry& mesh, std::vector<IndexType>& sharpEdges, unsigned int tesselation, IndexedGeometry& resultMesh );
	std::vector<IndexType>		tesselate           ( IndexedGeometry& mesh, IndexedGeometry& resultMesh, std::vector<IndexType> sharpEdges );
	void						moveVerticies       ( IndexedGeometry& mesh, std::vector<IndexType>& sharpEdges, IndexedGeometry& resultMesh );
	void						oldMoveVerticies    ( IndexedGeometry& mesh, std::vector<IndexType>& sharpEdges, IndexedGeometry& resultMesh );
	bool						findVertex          ( const std::vector<glm::vec3>& verticies, glm::vec3 vertex, IndexType& index, IndexType startIndex );
	std::vector<IndexType>		findAllNeighbours   ( IndexType index, const std::vector<IndexType>& indicies, IndexType maxIndex );
	std::vector<float>			makeWeightTable     ( IndexType index, std::vector<IndexType>& vertexNeighbours, std::vector<IndexType>& sharpEdges, bool edgePoint );
	
    std::vector<IndexType>		findNeighboursForEdgeVertex     ( IndexType index, const std::vector<IndexType>& preIndicies, const std::vector<IndexType>& postIndicies, IndexType maxIndex );
	glm::vec3					computeVertexNewPosition        ( IndexType index, const std::vector<IndexType>& vertexNeighbours, const std::vector<float>& vertexWeights, const std::vector<glm::vec3>& verticies, bool ignoreIndex );
	void						addIfNotExists                  ( IndexType index, std::vector<IndexType>& vertexNeighbours );
	VertexType					computeVertexType               ( int i, std::vector<IndexType> sharpEdges );
	float						computeVertexWeight             ( int vertexIndex, int secondVertexIndex, VertexType vertexType, std::vector<IndexType> sharpEdges );
	bool						isSharpEdge                     ( int index1, int index2, std::vector<IndexType> sharpEdges );
	float						computeEdgeVertexWeight         ( IndexType edgeIndex1, IndexType edgeIndex2, std::vector<VertexData>& vertexData );
public:
	HelperSmoothMesh(void);
	~HelperSmoothMesh(void);

	void						smooth( IndexedGeometry& mesh, std::vector<IndexType>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	IndexedGeometry				smooth( IndexedGeometry& mesh, std::vector<IndexType>& edges, unsigned int tesselation );
};

} //model
} //bv