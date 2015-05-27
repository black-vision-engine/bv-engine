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

	std::vector<INDEX_TYPE>& indicies = mesh.getIndicies();
	std::vector<glm::vec3>& verticies = mesh.getVerticies();
	if( indicies.size() < 3 )
		return;

	remainnigTriangles = static_cast<unsigned int>( indicies.size() / 3 );
	usedTriangles.resize( remainnigTriangles, false );


	unsigned int lastIndex1 = 1;
	unsigned int lastIndex2 = 2;
	unsigned int newIndex = 0;

	verts->AddAttribute( verticies[ indicies[newIndex] ] );
	verts->AddAttribute( verticies[ indicies[lastIndex1] ] );
	verts->AddAttribute( verticies[ indicies[lastIndex2] ] );
	usedRangeIndex = 3;
	usedTriangles[0] = true;
	--remainnigTriangles;

	while( remainnigTriangles > 0 )
	{
		bool hasNeighbour = true;

		while( hasNeighbour )
		{
			while( remainnigTriangles > 0 && findNeighbour( lastIndex1, lastIndex2, newIndex, indicies ) )
			{
				verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
				lastIndex1 = lastIndex2;
				lastIndex2 = newIndex;

				--remainnigTriangles;
			}
			moveRangeIndex();

			if( remainnigTriangles > 0 )
				verts->AddAttribute( verticies[ indicies[ lastIndex2 ] ] );


			hasNeighbour = findNeighbourPair( lastIndex2, lastIndex1, lastIndex2, indicies );
			if( !hasNeighbour )
				break;

			verts->AddAttribute( verticies[ indicies[ lastIndex1 ] ] );
			verts->AddAttribute( verticies[ indicies[ lastIndex2 ] ] );

			--remainnigTriangles;

		}

		if( remainnigTriangles > 0 )
		{
			findFirstUnusedTriangle( newIndex, indicies );
			verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
			verts->AddAttribute( verticies[ indicies[ newIndex ] ] );
			verts->AddAttribute( verticies[ indicies[ newIndex + 1 ] ] );
			verts->AddAttribute( verticies[ indicies[ newIndex + 2 ] ] );
			lastIndex1 = newIndex + 1;
			lastIndex2 = newIndex + 2;

			--remainnigTriangles;
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
bool IndexedGeometryConverter::findNeighbour( unsigned int index1, unsigned int index2, unsigned int& foundIndex, std::vector<INDEX_TYPE>& indicies )
{
	unsigned int i = usedRangeIndex;
	for( ; i < indicies.size(); ++i )
		if( indicies[ i ] == indicies[ index1 ] && !usedTriangles[ i / 3 ] )
		{
			unsigned int triangleStart = i - i % 3;
			for( unsigned int k = triangleStart; k < triangleStart + 3; ++k )	// Little waste of time to compare i-th triangle, which we know. But it's better then milion of if's.
				if( indicies[ k ] == indicies[ index2 ] )	//We know that, two indicies exists in this triangle.
				{// Return remainig index.
					foundIndex = static_cast<unsigned int> ( 3 * triangleStart + 3 - (i + k) );	// Sum of indicies is 3*triangleStart + 0 + 1 + 2. We substract i and k. Result is new index.

					usedTriangles[i / 3] = true;
					return true;
				}
			//i = triangleStart + 3;	// Omit checked verticies
		}

	return false;
}

/**Finds neighbour as the second findNeighbour function, but looks for only one common vertex.*/
bool IndexedGeometryConverter::findNeighbourPair( unsigned int index1, unsigned int& foundIndex1, unsigned int& foundIndex2, std::vector<INDEX_TYPE>& indicies )
{
	unsigned int i = usedRangeIndex;
	for( ; i < indicies.size(); ++i )
		if( indicies[ i ] == indicies[ index1 ] && !usedTriangles[ i / 3 ] )
		{
			unsigned int offsetFromTriangleStart = i % 3;
			unsigned int triangleStart = static_cast<unsigned int>( i - offsetFromTriangleStart );
			foundIndex1 = triangleStart + ( offsetFromTriangleStart + 1 ) % 3;
			foundIndex2 = triangleStart + ( offsetFromTriangleStart + 2 ) % 3;

			usedTriangles[i / 3] = true;
			return true;
		}

	return false;
}

void IndexedGeometryConverter::findFirstUnusedTriangle( unsigned int& index1 , std::vector<INDEX_TYPE>& indicies )
{
	unsigned int i = usedRangeIndex;
	for( ; i < indicies.size(); i += 3 )
		if( !usedTriangles[i / 3] )
			break;

	//usedRangeIndex = 3;
	usedTriangles[i / 3] = true;

	index1 = static_cast<unsigned int>( i );
}

/**For optimizing performence.
Variable usedIndexRange is used to avoid looking form new triangled in used range
of vector. All searching begins from usedIndexRange.*/
void IndexedGeometryConverter::moveRangeIndex()
{
	unsigned int i = usedRangeIndex / 3;
	while( i < usedTriangles.size() && usedTriangles[ i ] )
		++i;
	usedRangeIndex = static_cast<unsigned int>( 3*i );
}

}

}