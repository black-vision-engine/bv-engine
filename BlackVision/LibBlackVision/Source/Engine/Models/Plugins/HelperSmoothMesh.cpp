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
void HelperSmoothMesh::smooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation, IndexedGeometry& resultMesh )
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
IndexedGeometry HelperSmoothMesh::smooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation )
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
void HelperSmoothMesh::privateSmooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation, IndexedGeometry& resultMesh )
{
	if( tesselation == 0 )	// End of recursion.
	{
		resultMesh = std::move( mesh );		// We don't need mesh anymore so we move it, instead of coping it.
		return;
	}

	IndexedGeometry new_mesh;

	tesselate( mesh, new_mesh );
	moveVerticies( mesh, edges, resultMesh );

	smooth( new_mesh, edges, tesselation - 1, resultMesh );
	// Remember! You can't do anything with new_mesh after smooth call. It have been already moved to resultMesh.
}

/**Tesselates given mesh one time.*/
void HelperSmoothMesh::tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh )
{
	std::vector<unsigned short>& indicies = mesh.getIndicies();
	std::vector<glm::vec3>& verticies = mesh.getVerticies();
	std::vector<unsigned short>& resultIndicies = mesh.getIndicies();
	std::vector<glm::vec3>& resultVerticies = resultMesh.getVerticies();

	std::vector<bool> usedVerticies;

	resultMesh.getIndicies().reserve( 4 * indicies.size() );
	std::copy( verticies.begin(), verticies.end(), resultVerticies.begin() );		//Existing verticies have the same position as in init geometry.
	resultMesh.getVerticies().reserve( 3 * verticies.size() / 2 );

	usedVerticies.resize( verticies.size(), false );

	for( unsigned int i = 0; i < indicies.size(); i += 3 )
	{
		unsigned short newIndicies[3];

		for( unsigned int j = 0; j < 3; ++j )
			if( usedVerticies[ indicies[i + j] ] && usedVerticies[ indicies[i + j + 1] ] )
			{// This edge have been already devided.
				// Looking for vertex containing this edge in resultIndicies. 
				for( unsigned int k = 0; k < resultIndicies.size(); k += 12 )	// We have been adding 12 indicies at one time.
					for( int v = 0; v < 3; v += 3 )		// Onlu 3 verticies are interesting.
						if( resultIndicies[ k + v ] == indicies[i + j] )	// Indicies are in the same order
							if( resultIndicies[ k + (v+3) % 9] == indicies[i + j + 1] )
							{// We found it
								newIndicies[j] = resultIndicies[ k + 9 + v / 3 ];	// Thats because we know, how we made this table.
							}
						else if( resultIndicies[ k + v ] == indicies[i + j + 1] )	// Indicies are in diffrent order.
							if( resultIndicies[ k + (v+3) % 9] == indicies[i + j] )
							{// We found it
								newIndicies[j] = resultIndicies[ k + 9 + v / 3];	// Thats because we know, how we made this table.
							}
			}
			else
			{// We must devide edge.
				glm::vec3 firstVertex = verticies[ indicies[i + j] ];
				glm::vec3 secondVertex = verticies[ indicies[i + j + 1] ];
				resultVerticies.push_back( ( firstVertex + secondVertex ) * glm::vec3( 0.5, 0.5, 0.5 ) );

				newIndicies[j] = static_cast<unsigned short>( resultVerticies.size() - 1 );		// Position of new added vertex in vector is out new index;

				usedVerticies[ indicies[i + j] ] = true;		// Set verticies as used.
				usedVerticies[ indicies[i + j + 1] ] = true;	// Set verticies as used.
			}

		// Adding new traingles. Don't touch adding order. Function depeds on it, when vertex already existed.
		int k = 2;
		for( int j = 0; j < 3; ++j )	// Outer triangles.
		{
			resultIndicies.push_back( static_cast<unsigned short>( i + j ) );
			resultIndicies.push_back( newIndicies[ k++ % 3 ] );
			resultIndicies.push_back( newIndicies[ k % 3 ] );
		}
		for( int j = 0; j < 3; ++j )	// Middle triangle.
			resultIndicies.push_back( newIndicies[j] );
	}
}

/**Moves verticies of the new mesh to appropriate positions.*/
void moveVerticies( IndexedGeometry& mesh, std::vector<unsigned short>& edges, IndexedGeometry& resultMesh )
{
	mesh;
	edges;
	resultMesh;
}


}	//model
}	//bv
