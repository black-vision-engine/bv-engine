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


	unsigned short lastIndex1 = 1;
	unsigned short lastIndex2 = 2;
	unsigned short newIndex = 0;

	verts->AddAttribute( verticies[ indicies[newIndex] ] );
	verts->AddAttribute( verticies[ indicies[lastIndex1] ] );
	verts->AddAttribute( verticies[ indicies[lastIndex2] ] );
	usedRangeIndex = 2;

	while( remainnigTriangles )
	{
		bool hasNeighbour = true;

		while( hasNeighbour )
		{
			while( remainnigTriangles && findNeighbour( lastIndex1, lastIndex2, newIndex, indicies ) )
			{
				verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
				lastIndex1 = lastIndex2;
				lastIndex2 = newIndex;

				--remainnigTriangles;
			}

			if( remainnigTriangles )
				verts->AddAttribute( verticies[ indicies[ lastIndex2 ] ] );


			hasNeighbour = findNeighbourPair( lastIndex2, lastIndex1, lastIndex2, indicies );
			if( !hasNeighbour )
				break;

			verts->AddAttribute( verticies[ indicies[ lastIndex1 ] ] );
			verts->AddAttribute( verticies[ indicies[ lastIndex2 ] ] );

			--remainnigTriangles;

		}

		findFirstUnusedTriangle( newIndex, indicies );
		verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
		verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
		verts->AddAttribute( verticies[ indicies[ newIndex + 1 ] ] );
		verts->AddAttribute( verticies[ indicies[ newIndex + 2 ] ] );
		lastIndex1 = newIndex + 1;
		lastIndex2 = newIndex + 2;
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
bool IndexedGeometryConverter::findNeighbour( unsigned short index1, unsigned short index2, unsigned short& foundIndex, std::vector<unsigned short>& indicies )
{
	unsigned int i = usedRangeIndex;
	for( ; i < indicies.size(); ++i )
		if( indicies[ i ] == indicies[ index1 ] )
		{
			unsigned int triangleStart = i - i % 3;
			for( unsigned int k = triangleStart; k < triangleStart + 3; ++k )	// Little waste of time to compare i-th triangle, which we know. But it's better then milion of if's.
				if( indicies[ k ] == indicies[ index2 ] )	//We know that, two indicies exists in this triangle.
				{// Return remainig index.
					foundIndex = static_cast<unsigned short> ( 3 * triangleStart + 3 - (i + k) );	// Sum of indicies is 3*triangleStart + 0 + 1 + 2. We substract i and k. Result is new index.
					return true;
				}
		}

	return false;
}

/**Finds neighbour as the second findNeighbour function, but looks for only one common vertex.*/
bool IndexedGeometryConverter::findNeighbourPair( unsigned short index1, unsigned short& foundIndex1, unsigned short& foundIndex2, std::vector<unsigned short>& indicies )
{
	unsigned int i = usedRangeIndex;
	for( ; i < indicies.size(); ++i )
		if( indicies[ i ] == indicies[ index1 ] )
		{
			unsigned short offsetFromStart = i % 3;
			unsigned short triangleStart = static_cast<unsigned short>( i - offsetFromStart );
			foundIndex1 = triangleStart + ( offsetFromStart + 1 ) % 3;
			foundIndex2 = triangleStart + ( offsetFromStart + 2 ) % 3;
			return true;
		}

	return false;
}

void IndexedGeometryConverter::findFirstUnusedTriangle( unsigned short& index1 , std::vector<unsigned short>& indicies )
{
	unsigned int i = usedRangeIndex;
	for( ; i < indicies.size(); i += 3 )
		if( !usedTriangles[i/3] )
			break;

	//usedRangeIndex = 3;
	usedTriangles[i/3] = true;

	index1 = static_cast<unsigned short>( i+1 );
}

}

}