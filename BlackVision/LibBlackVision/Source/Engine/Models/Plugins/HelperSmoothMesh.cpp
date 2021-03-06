#include "stdafx.h"

#include "HelperSmoothMesh.h"
#include "Mathematics\Defines.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

const float CREASE_VERTEX_WEIGHT = 6.0f;

float weightTable[4][4] =
{
	{ 3.0f, 3.0f, 3.0f, 3.0f },
	{ 3.0f, 1.0f, 5.0f, 5.0f },
	{ 3.0f, 3.0f, 1.0f, 1.0f },
	{ 3.0f, 3.0f, 1.0f, 1.0f },
};


HelperSmoothMesh::HelperSmoothMesh(void)
{
}


HelperSmoothMesh::~HelperSmoothMesh(void)
{
}

/**This is main function, that should be called. It tmake given mesh smooth using number of tesselations in param.

Function calls private smooth to protect user from hurt himself. If he called private_smooth function with
tesselation = 0, function would move content of mesh to resultMesh, leaving mesh in bad shape.
@param[in] mesh Begin mesh to smooth.
@param[in] edges Edges which should remain sharp.
@param[in] tesselation Number of tesselations, that will be aplied to mesh
@param[out] resultMesh Result of smooth.*/
void HelperSmoothMesh::smooth( IndexedGeometry& mesh, std::vector<IndexType>& edges, unsigned int tesselation, IndexedGeometry& resultMesh )
{
	if( tesselation == 0 )
		resultMesh = mesh;		// Normal copy assignment.
	privateSmooth( mesh, edges, tesselation, resultMesh );
}

/**This is main function, that should be called. It tmake given mesh smooth using number of tesselations in param.

Function calls private smooth to protect user from hurt himself. If he called private_smooth function with
tesselation = 0, function would move content of mesh to resultMesh, leaving mesh in bad shape.
@param[in] mesh Begin mesh to smooth.
@param[in] edges Edges which should remain sharp.
@param[in] tesselation Number of tesselations, that will be aplied to mesh
@return resultMesh Result of smooth.*/
IndexedGeometry HelperSmoothMesh::smooth( IndexedGeometry& mesh, std::vector<IndexType>& edges, unsigned int tesselation )
{
	IndexedGeometry newMesh;
	smooth( mesh, edges, tesselation, newMesh );
	return std::move( newMesh );
}

/**This the real smooth function. Look at the description of smooth, if you want to know more.

@param[in] mesh Begin mesh to smooth.
@param[in] edges Edges which should remain sharp.
@param[in] tesselation Number of tesselations, that will be aplied to mesh
@param[out] resultMesh Result of smooth.*/
void HelperSmoothMesh::privateSmooth( IndexedGeometry& mesh, std::vector<IndexType>& sharpEdges, unsigned int tesselation, IndexedGeometry& resultMesh )
{
	if( tesselation == 0 )	// End of recursion.
	{
		resultMesh = std::move( mesh );		// We don't need mesh anymore so we move it, instead of coping it.
		return;
	}

	if( sharpEdges.size() % 2 )	// If true, something wrong with edges.
		sharpEdges.pop_back();		

	IndexedGeometry newMesh;

	std::vector<IndexType> newSharpEdges =  tesselate( mesh, newMesh, sharpEdges );
	moveVerticies( mesh, sharpEdges, newMesh );

	privateSmooth( newMesh, newSharpEdges, tesselation - 1, resultMesh );
	// Remember! You can't do anything with new_mesh after smooth call. It have been already moved to resultMesh.
}

/**Tesselates given mesh one time.*/
std::vector<IndexType> HelperSmoothMesh::tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh, std::vector<IndexType> sharpEdges )
{
	const std::vector<IndexType>& indicies = mesh.GetIndicies();
	const std::vector<glm::vec3>& verticies = mesh.GetVerticies();
	std::vector<IndexType>& resultIndicies = resultMesh.GetIndicies();
	std::vector<glm::vec3>& resultVerticies = resultMesh.GetVerticies();
	std::vector<IndexType> newSharpEdges;

	resultIndicies.reserve( 4 * indicies.size() );
	resultVerticies.reserve( 3 * verticies.size() / 2 );
	resultVerticies.resize( verticies.size() );			//Existing verticies have the same position as in init geometry.


	for( unsigned int i = 0; i < indicies.size(); i += 3 )
	{
		IndexType newIndicies[3];

		for( unsigned int j = 0; j < 3; ++j )
		{
			IndexType newIndex;
			glm::vec3 firstVertex = verticies[ indicies[i + j] ];
			glm::vec3 secondVertex = verticies[ indicies[i + (j + 1) % 3] ];
			glm::vec3 newVertex = ( firstVertex + secondVertex ) * glm::vec3( 0.5, 0.5, 0.5 );

			if( findVertex( resultVerticies, newVertex, newIndex, (IndexType)verticies.size() ) )	//Checking only list of added verticies in this iteration.
				newIndicies[ j ] = newIndex;
			else
			{
				newIndicies[ j ] = static_cast<unsigned short>( resultVerticies.size() );		// Position of new added vertex in vector is our new index;
				resultVerticies.push_back( newVertex );

				// Adding indicies to sharp edges.
				if( isSharpEdge( indicies[i + j], indicies[i + (j + 1) % 3], sharpEdges ) )
				{
					newSharpEdges.push_back( indicies[i + j] );
					newSharpEdges.push_back( newIndicies[ j ] );
					newSharpEdges.push_back( newIndicies[ j ] );
					newSharpEdges.push_back( indicies[i + (j + 1) % 3] );
				}
			}
		}

		// Don't touch order od verticies. Function moveVerticies uses it.
		int k = 2;
		for( int j = 0; j < 3; ++j )	// Outer triangles.
		{
			resultIndicies.push_back( static_cast<IndexType>( indicies[ i + j ] ) );
			resultIndicies.push_back( newIndicies[ ++k % 3 ] );
			resultIndicies.push_back( newIndicies[ ( k - 1 ) % 3 ] );
		}
		for( int j = 0; j < 3; ++j )	// Middle triangle.
			resultIndicies.push_back( newIndicies[j] );
	}

	return std::move( newSharpEdges );
}

/**Moves verticies of the new mesh to appropriate positions.

Old version. New is more efficient, but can have problems with not closed meshes.*/
void HelperSmoothMesh::oldMoveVerticies( IndexedGeometry& mesh, std::vector<IndexType>& sharpEdges, IndexedGeometry& resultMesh )
{
	const std::vector<IndexType>& indicies = mesh.GetIndicies();
	const std::vector<glm::vec3>& verticies = mesh.GetVerticies();
	std::vector<IndexType>& resultIndicies = resultMesh.GetIndicies();
	std::vector<glm::vec3>& resultVerticies = resultMesh.GetVerticies();
	
	std::vector<IndexType> vertexNeighbours;
	std::vector<float> vertexWeights;


	for( IndexType i = 0; i < verticies.size(); ++i )
	{// Make movement for vertex points
		vertexNeighbours = findAllNeighbours( i, indicies, IndexType( resultVerticies.size() - 1 ) );		// 3. parameter - we are looking for neighbours in all vertices
		vertexWeights = makeWeightTable( i, vertexNeighbours, sharpEdges, false );
		resultVerticies[ i ] = computeVertexNewPosition( i, vertexNeighbours, vertexWeights, verticies, false );
	}
	for( IndexType i = (IndexType)verticies.size(); i < resultVerticies.size(); ++i )
	{// Make movement for edge points
		vertexNeighbours = findNeighboursForEdgeVertex( i, indicies, resultIndicies, IndexType( verticies.size() - 1 ) );	// 3. parameter - we are looking for neighbours only in verticies from previous interation.
		vertexWeights = makeWeightTable( i, vertexNeighbours, sharpEdges, true );
		resultVerticies[ i ] = computeVertexNewPosition( i, vertexNeighbours, vertexWeights, verticies, true );
	}
}

/**Moves verticies of the new mesh to appropriate positions.*/
void HelperSmoothMesh::moveVerticies( IndexedGeometry& mesh, std::vector<IndexType>& sharpEdges, IndexedGeometry& resultMesh )
{
	const std::vector<IndexType>& indicies = mesh.GetIndicies();
	const std::vector<glm::vec3>& verticies = mesh.GetVerticies();
	std::vector<IndexType>& resultIndicies = resultMesh.GetIndicies();
	std::vector<glm::vec3>& resultVerticies = resultMesh.GetVerticies();


	std::vector<VertexData> vertexData;
	vertexData.resize( verticies.size(), VertexData() );

	// Zero vertex check vertex type.
	for( IndexType i = 0; i < verticies.size(); ++i )
	{
		resultVerticies[ i ] = glm::vec3( 0.0, 0.0, 0.0 );
		vertexData[ i ].type = computeVertexType( i, sharpEdges );
	}

	// We iterate through edges and add position with weight to verticies.
	for( unsigned int i = 0; i < indicies.size(); i += 3 )
    {
		for( IndexType j = 0; j < 3; ++j )
		{
			IndexType index1 = indicies[ i + j ];
			IndexType index2 = indicies[ i + (j + 1) % 3 ];

			float weight1 = computeVertexWeight( index1, index2, vertexData[ index1 ].type, sharpEdges ) / 2.0f;
			resultVerticies[ index1 ] += weight1 * verticies[ index2 ];

			float weight2 = computeVertexWeight( index2, index1, vertexData[ index2 ].type, sharpEdges ) / 2.0f;
			resultVerticies[ index2 ] += weight2 * verticies[ index1 ];

			vertexData[ index1 ].numNeighbours++;
			vertexData[ index2 ].numNeighbours++;
			vertexData[ index1 ].sumWeigths += weight1;
			vertexData[ index2 ].sumWeigths += weight2;
		}
    }

	// We add center vertex position.
	for( IndexType i = 0; i < verticies.size(); ++i )
	{
		if( vertexData[ i ].type == VertexType::SMOOTH_VERTEX )
		{
			float numNeighbours = static_cast<float>( vertexData[ i ].numNeighbours / 2 );	// We have taken each vertex two times.
			float weight = computeCenterVertexWeight( static_cast<unsigned short>( numNeighbours ) );
			resultVerticies[ i ] = ( resultVerticies[ i ] + verticies[ i ] * weight ) / ( vertexData[ i ].sumWeigths + weight );
		}
		else if( vertexData[ i ].type == VertexType::CORNER_VERTEX )
			resultVerticies[ i ] = verticies[ i ];
		else if( vertexData[ i ].type == VertexType::CREASE_VERTEX )
			resultVerticies[ i ] = ( verticies[ i ] * CREASE_VERTEX_WEIGHT + resultVerticies[ i ] ) / ( CREASE_VERTEX_WEIGHT + vertexData[ i ].sumWeigths );	//Check weight in docs
	}

	// Clear edge verticies.
	size_t verticiesOffset = verticies.size();
	for( size_t i = verticiesOffset; i < resultVerticies.size(); ++i )
		resultVerticies[ i ] = glm::vec3( 0.0, 0.0, 0.0 );

	// We need sum of weights
	std::vector<float> sumWeights;
	sumWeights.resize( resultVerticies.size() - verticiesOffset, 0.0f );

	// We iterate added verticies.
	for( unsigned int i = 0; i < resultIndicies.size(); i += 12 )
    {
		for( IndexType j = 0; j < 9; j += 3 )
		{
			IndexType edgeIndex1 = resultIndicies[ i + j ];
			IndexType edgeIndex2 = resultIndicies[ i + (j + 3) % 9 ];
			IndexType index3 = resultIndicies[ i + (j + 6) % 9 ];
			IndexType edgeVertex = resultIndicies[ i + j + 1 ];		// We use our knowledge about order of verticies after tessellation.

			float weight1;
			float weight2;
			float weight3;
			if( isSharpEdge( edgeIndex1, edgeIndex2, sharpEdges ) )
			{
				weight1 = computeEdgeVertexWeight( edgeIndex2, edgeIndex1, vertexData );
				weight2 = computeEdgeVertexWeight( edgeIndex1, edgeIndex2, vertexData );
				weight3 = 0.0;
			}
			else
			{
				weight1 = 3.0f;
				weight2 = 3.0f;
				weight3 = 1.0f;
			}
			weight1 /= 2.0f;
			weight2 /= 2.0f;

			resultVerticies[ edgeVertex ] += verticies[ edgeIndex1 ] * weight1 + verticies[ edgeIndex2 ] * weight2 + verticies[ index3 ] * weight3;
			sumWeights[ edgeVertex - verticiesOffset ] += weight1 + weight2 + weight3;
		}
    }

	for( size_t i = verticiesOffset; i < resultVerticies.size(); ++i )
		resultVerticies[ i ] = float( 1.0 / sumWeights[ i - verticiesOffset ] ) * resultVerticies[ i ];
}

/**Finds given vertex in array of verticies.

@return Returns true if vertex already exists in verticies.*/
bool HelperSmoothMesh::findVertex( const std::vector<glm::vec3>& verticies, glm::vec3 vertex, IndexType& index, IndexType startIndex )
{
	for( unsigned int i = startIndex; i < verticies.size(); ++i )
    {
		if( verticies[ i ] == vertex )
		{
			index = (IndexType)i;
			return true;
		}
    }

	index = 0;
	return false;
}


/**
@return Returns all neighbours of given vertex under index.*/
std::vector<IndexType> HelperSmoothMesh::findAllNeighbours( IndexType index, const std::vector<IndexType>& indicies, IndexType maxIndex )
{
	std::vector<IndexType> vertexNeighbours;
	vertexNeighbours.reserve( 6 );		// It's the number of neighbours that I expect in mesh for one vertex.

	for( IndexType i = 0; i < indicies.size(); ++i )
	{
		if( indicies[ i ] == index )
		{
			IndexType offsetFromTriangleStart = i % 3;
			IndexType triangleStart = static_cast<IndexType>( i - offsetFromTriangleStart );
			IndexType foundIndex1 = indicies[ triangleStart + ( offsetFromTriangleStart + 1 ) % 3 ];
			IndexType foundIndex2 = indicies[ triangleStart + ( offsetFromTriangleStart + 2 ) % 3 ];
			if( foundIndex1 <= maxIndex )
				addIfNotExists( foundIndex1, vertexNeighbours );
			if( foundIndex2 <= maxIndex )
				addIfNotExists( foundIndex2, vertexNeighbours );
		}
		//i = triangleStart + 3;	// Omit checked verticies
	}

	return std::move( vertexNeighbours );
}

/**
Treats edgepoints diffrent then vertex points.
Set edgePoint to true if you want process edge points.*/
std::vector<float> HelperSmoothMesh::makeWeightTable( IndexType index, std::vector<IndexType>& vertexNeighbours, std::vector<IndexType>& sharpEdges, bool edgePoint )
{
	sharpEdges;
	index;
	const float edgeWeight = 3.0;
	const float distanceWeight = 1.0;

	std::vector<float> vertexWeights;
	vertexWeights.reserve( vertexNeighbours.size() );

	if( edgePoint )
	{
		// For now there're no sharp edge dependent weights.
		assert( vertexNeighbours.size() == 4 );
		vertexWeights.push_back( edgeWeight );
		vertexWeights.push_back( edgeWeight );
		vertexWeights.push_back( distanceWeight );
		vertexWeights.push_back( distanceWeight );
	}
	else
	{
		for( auto i : vertexNeighbours )
			vertexWeights.push_back( distanceWeight ), i;		// For now there're no sharp edge dependent weights.
	}

	return std::move( vertexWeights );
}

/**Computes new position of vertex.

@param[in] ignoreIndex If we compute edge vertex, we don't want to count it in. Set flag to true.*/
glm::vec3 HelperSmoothMesh::computeVertexNewPosition( IndexType index, const std::vector<IndexType>& vertexNeighbours, const std::vector<float>& vertexWeights, const std::vector<glm::vec3>& verticies, bool ignoreIndex )
{
	glm::vec3 resultVertex( 0.0, 0.0, 0.0 );
	float sumWeights = 0.0;

	for( auto weight : vertexWeights )
		sumWeights += weight;

	if( !ignoreIndex )
	{// Vertex verticies[ index ] must be took.
		float centerWeight = computeCenterVertexWeight((unsigned short)vertexNeighbours.size() );
		sumWeights += centerWeight;
		resultVertex += verticies[ index ] * ( centerWeight / sumWeights );
	}

	for( unsigned int i = 0; i < vertexWeights.size(); ++i )
		resultVertex += verticies[ vertexNeighbours[ i ] ] * vertexWeights[ i ] / sumWeights;

	return resultVertex;
}

/**
Edge points have another neighbours than vertex points.*/
std::vector<IndexType> HelperSmoothMesh::findNeighboursForEdgeVertex( IndexType index, const std::vector<IndexType>& preIndicies, const std::vector<IndexType>& postIndicies, IndexType maxIndex )
{
	std::vector<IndexType> vertexNeighbours;

	preIndicies;

	vertexNeighbours = findAllNeighbours( index, postIndicies, maxIndex );
	
	assert( vertexNeighbours.size() == 2 );		// Should be 2 neighbours. Otherwise there's bug.

	for( IndexType i = 0; i < preIndicies.size(); ++i )
		if( preIndicies[ i ] == vertexNeighbours[ 0 ] )
		{
			IndexType triangleStart = i - i % 3;
			for( IndexType k = triangleStart; k < triangleStart + 3; ++k )	// For every triangle
				if( preIndicies[ k ] == vertexNeighbours[ 1 ] )	//We know that, two indicies exists in this triangle.
				// Return remainig index.
					vertexNeighbours.push_back( preIndicies[ static_cast<IndexType>( 3 * triangleStart + 3 - (i + k) ) ] );	// Sum of indicies is 3*triangleStart + 0 + 1 + 2. We substract i and k. Result is new index.
		}

	assert( vertexNeighbours.size() == 4 );

	return std::move( vertexNeighbours );
}


void HelperSmoothMesh::addIfNotExists( IndexType index, std::vector<IndexType>& vertexNeighbours )
{
	for( auto i : vertexNeighbours )
		if( i == index )
			return;
	vertexNeighbours.push_back( index );
}

float HelperSmoothMesh::computeCenterVertexWeight( unsigned short numNeighbours )
{
	double functionA = 5.0 / 8.0 - pow( ( 3.0 + 2.0 * cos( TWOPI / (double) numNeighbours) ), 2 ) / 64.0;
	return static_cast<float>( double(numNeighbours) / functionA - numNeighbours );

	//if( numNeighbours > 3 )
	//	return 3.0f / 8.0f;
	//else
	//	return 3.0f / 16.0f;
}


VertexType HelperSmoothMesh::computeVertexType( int i, std::vector<IndexType> sharpEdges )
{
	int numSharpEdges = 0;

	for( unsigned int j = 0; j < sharpEdges.size(); ++j )
		if( sharpEdges[ j ] == (IndexType)i )
			++numSharpEdges;

	if( numSharpEdges < 2 )
		return VertexType::SMOOTH_VERTEX;
	else if( numSharpEdges == 2 )
		return VertexType::CREASE_VERTEX;
	else
		return VertexType::CORNER_VERTEX;
}

float HelperSmoothMesh::computeVertexWeight( int vertexIndex, int secondVertexIndex, VertexType vertexType, std::vector<IndexType> sharpEdges )
{
	if( vertexType == VertexType::CORNER_VERTEX )
		return 0.0f;
	else if( vertexType == VertexType::SMOOTH_VERTEX )
		return 1.0f;
	else if( vertexType == VertexType::CREASE_VERTEX )
	{
		if( isSharpEdge( vertexIndex, secondVertexIndex, sharpEdges ) )
			return 1.0f;
		return 0.0f;
	}
	else
	{
		assert( false );
		return 0.0f;
	}
}

bool HelperSmoothMesh::isSharpEdge( int index1, int index2, std::vector<IndexType> sharpEdges )
{
	for( unsigned int i = 0; i < sharpEdges.size(); i += 2 )
		if( sharpEdges[ i ] == (IndexType)index1 && sharpEdges[ i + 1 ] == (IndexType)index2 ||
            sharpEdges[ i ] == (IndexType)index2 && sharpEdges[ i + 1 ] == (IndexType)index1 )
			return true;
	return false;
}

float HelperSmoothMesh::computeEdgeVertexWeight( IndexType edgeIndex1, IndexType edgeIndex2, std::vector<VertexData>& vertexData )
{
	if( vertexData[ edgeIndex1 ].numNeighbours == 12 )
		vertexData[ edgeIndex1 ].type = VertexType::REGULAR_CREASE_VERTEX;
	else
		vertexData[ edgeIndex1 ].type = VertexType::NON_REGULAR_CREASE_VERTEX;

	if( vertexData[ edgeIndex2 ].numNeighbours == 12 )
		vertexData[ edgeIndex2 ].type = VertexType::REGULAR_CREASE_VERTEX;
	else
		vertexData[ edgeIndex2 ].type = VertexType::NON_REGULAR_CREASE_VERTEX;

	return weightTable[ (int) vertexData[ edgeIndex1 ].type ][(int) vertexData[ edgeIndex2 ].type ];
}


}	//model
}	//bv
