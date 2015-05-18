#include "HelperIndexedGeometryConverter.h"

namespace bv { namespace model {


IndexedGeometryConverter::IndexedGeometryConverter(void)
{
}


IndexedGeometryConverter::~IndexedGeometryConverter(void)
{
}

/**Function convert indexed triangle list to strips and ads verticies to verts param.
Function doesn't take into account sharp edges. It will be in future versions.

@param[in] mesh Mesh to convert.
@param[out] verts Object to store result verticies.*/
void IndexedGeometryConverter::makeStrip( IndexedGeometry& mesh, Float3AttributeChannelPtr verts)
{
	usedRangeIndex = 0;

	std::vector<unsigned short>& indicies = mesh.getIndicies();
	std::vector<glm::vec3>& verticies = mesh.getVerticies();
	if( indicies.size() < 3 )
		return;

	remainnigTriangles = indicies.size() / 3;
	usedTriangles.resize( remainnigTriangles, false );


	unsigned short lastIndex1;
	unsigned short lastIndex2;
	unsigned short newIndex;


	while( remainnigTriangles )
	{
		addFirstUnusedPoints( lastIndex1, lastIndex2, indicies, verticies, verts );

		while( findNeighbour( lastIndex1, newIndex, indicies, verticies ) || findNeighbour( lastIndex2, newIndex, indicies, verticies ) )
		{

			while( remainnigTriangles && findNeighbour( lastIndex1, lastIndex2, newIndex, indicies, verticies ) )
			{
				verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
				lastIndex1 = lastIndex2;
				lastIndex2 = newIndex;

				--remainnigTriangles;
			}

			if( remainnigTriangles )
				verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
		}

		if( remainnigTriangles )
		{
			verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
			verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
		}
	}

	usedTriangles.resize( 0, false );
}

/**Finds neighbour for triangle, that consists of verticies pointed by indicies pointed by
parameters!!!.
Theese parameters aren't indicies to vertex table, but to index table.
The same for return index;

If next index is found, whole triangle is set as used in usedIndicies table.

@param[in] index1 First index of index of vertex.
@param[in] index1 Second index of index of vertex.
return Returns false if couldn't find traingle having such indicies.
*/
bool IndexedGeometryConverter::findNeighbour( unsigned short index1, unsigned short index2, unsigned short& foundIndex, std::vector<unsigned short>& indicies, std::vector<glm::vec3>& verticies )
{
	index1;
	index2;
	foundIndex;
	indicies;
	verticies;
	return false;
}

/**Finds neighbour as the second findNeighbour function, but looks for only one common vertex.*/
bool IndexedGeometryConverter::findNeighbour( unsigned short index1, unsigned short& foundIndex, std::vector<unsigned short>& indicies, std::vector<glm::vec3>& verticies )
{
	index1;
	foundIndex;
	indicies;
	verticies;
	return false;
}

void IndexedGeometryConverter::addFirstUnusedPoints( unsigned short& index1, unsigned short& index2, std::vector<unsigned short>& indicies, std::vector<glm::vec3>& verticies, Float3AttributeChannelPtr verts )
{
	unsigned int i = usedRangeIndex;
	for( ; i < indicies.size(); i += 3 )
		if( !usedTriangles[i/3] )
			break;

	verts->AddAttribute( verticies[ indicies[i] ] );
	verts->AddAttribute( verticies[ indicies[i+1] ] );
	verts->AddAttribute( verticies[ indicies[i+2] ] );

	//usedRangeIndex = 3;
	usedTriangles[i/3] = true;

	index1 = static_cast<unsigned short>( i+1 );
	index2 = static_cast<unsigned short>( i+2 );
}

}

}