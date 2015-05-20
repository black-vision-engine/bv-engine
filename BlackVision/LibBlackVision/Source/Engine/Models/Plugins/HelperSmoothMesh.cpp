#include "HelperSmoothMesh.h"
#include "Mathematics\Defines.h"


namespace bv { namespace model {



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
void HelperSmoothMesh::smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation, IndexedGeometry& resultMesh )
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
IndexedGeometry HelperSmoothMesh::smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation )
{
	IndexedGeometry new_mesh;
	smooth( mesh, edges, tesselation, new_mesh );
	return std::move( new_mesh );
}

/**This the real smooth function. Look at the description of smooth, if you want to know more.

@param[in] mesh Begin mesh to smooth.
@param[in] edges Edges which should remain sharp.
@param[in] tesselation Number of tesselations, that will be aplied to mesh
@param[out] resultMesh Result of smooth.*/
void HelperSmoothMesh::privateSmooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& sharpEdges, unsigned int tesselation, IndexedGeometry& resultMesh )
{
	if( tesselation == 0 )	// End of recursion.
	{
		resultMesh = std::move( mesh );		// We don't need mesh anymore so we move it, instead of coping it.
		return;
	}

	IndexedGeometry new_mesh;

	tesselate( mesh, new_mesh );
	moveVerticies( mesh, sharpEdges, new_mesh );

	privateSmooth( new_mesh, sharpEdges, tesselation - 1, resultMesh );
	// Remember! You can't do anything with new_mesh after smooth call. It have been already moved to resultMesh.
}

/**Tesselates given mesh one time.*/
void HelperSmoothMesh::tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh )
{
	const std::vector<INDEX_TYPE>& indicies = mesh.getIndicies();
	const std::vector<glm::vec3>& verticies = mesh.getVerticies();
	std::vector<INDEX_TYPE>& resultIndicies = resultMesh.getIndicies();
	std::vector<glm::vec3>& resultVerticies = resultMesh.getVerticies();

	resultIndicies.reserve( 4 * indicies.size() );
	resultVerticies.reserve( 3 * verticies.size() / 2 );
	resultVerticies.resize( verticies.size() );
	std::copy( verticies.begin(), verticies.end(), resultVerticies.begin() );		//Existing verticies have the same position as in init geometry.



	for( unsigned int i = 0; i < indicies.size(); i += 3 )
	{
		INDEX_TYPE newIndicies[3];

		for( unsigned int j = 0; j < 3; ++j )
		{
			INDEX_TYPE newIndex;
			glm::vec3 firstVertex = verticies[ indicies[i + j] ];
			glm::vec3 secondVertex = verticies[ indicies[i + (j + 1) % 3] ];
			glm::vec3 newVertex =  ( firstVertex + secondVertex ) * glm::vec3( 0.5, 0.5, 0.5 );

			if( findVertex( resultVerticies, newVertex, newIndex ) )
				newIndicies[ j ] = newIndex;
			else
			{
				newIndicies[ j ] = static_cast<unsigned short>( resultVerticies.size() );		// Position of new added vertex in vector is our new index;
				resultVerticies.push_back( newVertex );
			}

		}

		int k = 2;
		for( int j = 0; j < 3; ++j )	// Outer triangles.
		{
			resultIndicies.push_back( static_cast<INDEX_TYPE>( indicies[ i + j ] ) );
			resultIndicies.push_back( newIndicies[ k++ % 3 ] );
			resultIndicies.push_back( newIndicies[ k % 3 ] );
		}
		for( int j = 0; j < 3; ++j )	// Middle triangle.
			resultIndicies.push_back( newIndicies[j] );
	}
}

/**Moves verticies of the new mesh to appropriate positions.*/
void HelperSmoothMesh::moveVerticies( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& sharpEdges, IndexedGeometry& resultMesh )
{
	const std::vector<INDEX_TYPE>& indicies = mesh.getIndicies();
	const std::vector<glm::vec3>& verticies = mesh.getVerticies();
	std::vector<INDEX_TYPE>& resultIndicies = resultMesh.getIndicies();
	std::vector<glm::vec3>& resultVerticies = resultMesh.getVerticies();
	
	std::vector<bool> movedVerticies;
	movedVerticies.resize( resultVerticies.size(), false );
	std::vector<INDEX_TYPE> vertexNeighbours;
	std::vector<float> vertexWeights;


	for( INDEX_TYPE i = 0; i < verticies.size(); ++i )
	{// Make movement for vertex points
		vertexNeighbours = findAllNeighbours( i, indicies, INDEX_TYPE( resultVerticies.size() - 1 ) );		// 3. parameter - we are looking for neighbours in all vertices
		vertexWeights = makeWeightTable( i, vertexNeighbours, sharpEdges, false );
		resultVerticies[ i ] = computeVertexNewPosition( i, vertexNeighbours, vertexWeights, verticies, false );
	}
	for( INDEX_TYPE i = (INDEX_TYPE)verticies.size(); i < resultVerticies.size(); ++i )
	{// Make movement for edge points
		vertexNeighbours = findNeighboursForEdgeVertex( i, indicies, resultIndicies, INDEX_TYPE( verticies.size() - 1 ) );	// 3. parameter - we are looking for neighbours only in verticies from previous interation.
		vertexWeights = makeWeightTable( i, vertexNeighbours, sharpEdges, true );
		resultVerticies[ i ] = computeVertexNewPosition( i, vertexNeighbours, vertexWeights, verticies, true );
	}
}

/**Finds given vertex in array of verticies.

@return Returns true if vertex already exists in verticies.*/
bool HelperSmoothMesh::findVertex( const std::vector<glm::vec3>& verticies, glm::vec3 vertex, INDEX_TYPE& index )
{
	for( unsigned int i = 0; i < verticies.size(); ++i )
		if( verticies[ i ] == vertex )
		{
			index = (INDEX_TYPE)i;
			return true;
		}

	index = 0;
	return false;
}


/**
@return Returns all neighbours of given vertex under index.*/
std::vector<INDEX_TYPE> HelperSmoothMesh::findAllNeighbours( INDEX_TYPE index, const std::vector<INDEX_TYPE>& indicies, INDEX_TYPE maxIndex )
{
	std::vector<INDEX_TYPE> vertexNeighbours;
	vertexNeighbours.reserve( 6 );		// It's the number of neighbours that I expect in mesh for one vertex.

	for( INDEX_TYPE i = 0; i < indicies.size(); ++i )
	{
		if( indicies[ i ] == index )
		{
			INDEX_TYPE offsetFromTriangleStart = i % 3;
			INDEX_TYPE triangleStart = static_cast<INDEX_TYPE>( i - offsetFromTriangleStart );
			INDEX_TYPE foundIndex1 = indicies[ triangleStart + ( offsetFromTriangleStart + 1 ) % 3 ];
			INDEX_TYPE foundIndex2 = indicies[ triangleStart + ( offsetFromTriangleStart + 2 ) % 3 ];
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
std::vector<float> HelperSmoothMesh::makeWeightTable( INDEX_TYPE index, std::vector<INDEX_TYPE>& vertexNeighbours, std::vector<INDEX_TYPE>& sharpEdges, bool edgePoint )
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
glm::vec3 HelperSmoothMesh::computeVertexNewPosition( INDEX_TYPE index, const std::vector<INDEX_TYPE>& vertexNeighbours, const std::vector<float>& vertexWeights, const std::vector<glm::vec3>& verticies, bool ignoreIndex )
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
std::vector<INDEX_TYPE> HelperSmoothMesh::findNeighboursForEdgeVertex( INDEX_TYPE index, const std::vector<INDEX_TYPE>& preIndicies, const std::vector<INDEX_TYPE>& postIndicies, INDEX_TYPE maxIndex )
{
	std::vector<INDEX_TYPE> vertexNeighbours;

	preIndicies;

	vertexNeighbours = findAllNeighbours( index, postIndicies, maxIndex );
	
	assert( vertexNeighbours.size() == 2 );		// Should be 2 neighbours. Otherwise there's bug.

	for( INDEX_TYPE i = 0; i < preIndicies.size(); ++i )
		if( preIndicies[ i ] == vertexNeighbours[ 0 ] )
		{
			INDEX_TYPE triangleStart = i - i % 3;
			for( INDEX_TYPE k = triangleStart; k < triangleStart + 3; ++k )	// For every triangle
				if( preIndicies[ k ] == vertexNeighbours[ 1 ] )	//We know that, two indicies exists in this triangle.
				// Return remainig index.
					vertexNeighbours.push_back( preIndicies[ static_cast<INDEX_TYPE>( 3 * triangleStart + 3 - (i + k) ) ] );	// Sum of indicies is 3*triangleStart + 0 + 1 + 2. We substract i and k. Result is new index.
		}

	assert( vertexNeighbours.size() == 4 );

	return std::move( vertexNeighbours );
}


void HelperSmoothMesh::addIfNotExists( INDEX_TYPE index, std::vector<INDEX_TYPE>& vertexNeighbours )
{
	for( auto i : vertexNeighbours )
		if( i == index )
			return;
	vertexNeighbours.push_back( index );
}

float HelperSmoothMesh::computeCenterVertexWeight( unsigned short numNeighbours )
{
	double functionA = 5.0 / 8.0 - pow( ( 3.0 + 2.0 * cos( TWOPI / (double) numNeighbours) ), 2 ) / 64.0;
	return static_cast<float>( double(numNeighbours) / functionA - 1.0 );
}

}	//model
}	//bv
