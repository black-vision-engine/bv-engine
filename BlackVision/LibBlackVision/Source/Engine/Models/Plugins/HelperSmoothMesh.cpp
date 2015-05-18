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
	private_smooth( mesh, edges, tesselation, resultMesh );
}

/**This the real smooth function. Look at the description of smooth, if you want to know more.

@param[in] mesh Begin mesh to smooth.
@param[in] edges Edges which should remain sharp.
@param[in] tesselation Number of tesselations, that will be aplied to mesh
@param[out] resultMesh Result of smooth.*/
void HelperSmoothMesh::private_smooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation, IndexedGeometry& resultMesh )
{
	if( tesselation == 0 )	// End of recursion.
	{
		resultMesh = std::move( mesh );		// We don't need mesh anymore so we move it, instead of coping it.
		return;
	}

	IndexedGeometry new_mesh( mesh );

	tesselate( mesh, new_mesh );
	move_verticies( mesh, edges, resultMesh );

	smooth( new_mesh, edges, tesselation - 1, resultMesh );
	// Remember! You can't do anything with new_mesh after smooth call. It have been already moved to resultMesh.
}

/**Tesselates given mesh one time.*/
void HelperSmoothMesh::tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh )
{
	resultMesh;
	std::vector<unsigned short>& indicies = mesh.getIndicies();
	std::vector<glm::vec3>& verticies = mesh.getVerticies();
	std::vector<bool> verticies_bitset;


	verticies_bitset.resize( verticies.size() );

	for( unsigned int i = 0; i < indicies.size(); i += 3 )
	{

	}
}

/**Moves verticies of the new mesh to appropriate positions.*/
void move_verticies( IndexedGeometry& mesh, std::vector<unsigned short>& edges, IndexedGeometry& resultMesh )
{
	mesh;
	edges;
	resultMesh;
}


}	//model
}	//bv
