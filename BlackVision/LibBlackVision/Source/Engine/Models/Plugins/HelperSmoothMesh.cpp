#include "HelperSmoothMesh.h"


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
		vertexNeighbours = findAllNeighbours( i, indicies );
		vertexWeights = makeWeightTable( i, vertexNeighbours, sharpEdges );
		resultVerticies[ i ] = computeVertexNewPosition( i, vertexNeighbours, vertexWeights, verticies, false );
	}
	for( INDEX_TYPE i = (INDEX_TYPE)verticies.size(); i < resultVerticies.size(); ++i )
	{// Make movement for edge points
		vertexNeighbours = findNeighboursForEdgeVertex( i, indicies, resultIndicies );
		vertexWeights = makeWeightTable( i, vertexNeighbours, sharpEdges );
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
std::vector<INDEX_TYPE> HelperSmoothMesh::findAllNeighbours( INDEX_TYPE index, const std::vector<INDEX_TYPE>& indicies )
{
	std::vector<INDEX_TYPE> vertexNeighbours;
	vertexNeighbours.reserve( 6 );		// It's the number of neighbours that I expect in mesh for one vertex.

	for( INDEX_TYPE i : indicies )
		if( i == index )
		{

		}

	return std::move( vertexNeighbours );
}


std::vector<float> HelperSmoothMesh::makeWeightTable( INDEX_TYPE index, std::vector<INDEX_TYPE>& vertexNeighbours, std::vector<INDEX_TYPE>& sharpEdges )
{
	sharpEdges;
	index;

	std::vector<float> vertexWeights;
	vertexWeights.reserve( vertexNeighbours.size() );

	return std::move( vertexWeights );
}

/**Computes new position of vertex.

@param[in] ignoreIndex If we compute edge vertex, we don't want to count it in. Set flag to true.*/
glm::vec3 HelperSmoothMesh::computeVertexNewPosition( INDEX_TYPE index, const std::vector<INDEX_TYPE>& vertexNeighbours, const std::vector<float>& vertexWeights, const std::vector<glm::vec3>& verticies, bool ignoreIndex )
{
	index;
	vertexNeighbours;
	vertexWeights;
	verticies;
	ignoreIndex;
	return glm::vec3( 0.0, 0.0, 0.0 );
}

/**
Edge points have another neighbours than vertex points.*/
std::vector<INDEX_TYPE> HelperSmoothMesh::findNeighboursForEdgeVertex( INDEX_TYPE index, const std::vector<INDEX_TYPE>& preIndicies, const std::vector<INDEX_TYPE>& postIndicies )
{
	std::vector<INDEX_TYPE> vertexNeighbours;
	vertexNeighbours.reserve( 6 );		// It's the number of neighbours that I expect in mesh for one vertex.

	index;
	preIndicies;
	postIndicies;

	return std::move( vertexNeighbours );
}


}	//model
}	//bv
