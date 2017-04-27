#include "stdafx.h"

#include "HelperIndexedGeometryConverter.h"

#include <algorithm>

#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


IndexedGeometryConverter::IndexedGeometryConverter()
    :   m_rememberConversion( false )
{
    m_epsilon = ( Float32 )10e-5;
}


IndexedGeometryConverter::IndexedGeometryConverter( float epsilon )
    :   m_epsilon( epsilon )
    ,   m_rememberConversion( false )
{}

IndexedGeometryConverter::~IndexedGeometryConverter()
{}

// ***********************
// Converter can hold conversion data. You can apply the same conversion to other geometry channels like normals or UVs.
// This works only with functions MakeIndexGeomFromStrips and MakeIndexGeomFromTriangles.
// Call ConvertFromMemory function to make conversion.
void    IndexedGeometryConverter::RememberConversionIndicies  ( bool value )
{
    m_rememberConversion = value;
}

// ========================================================================= //
// Stripifier
// ========================================================================= //

/**Function convert indexed triangle list to strips and ads verticies to verts param.
Function doesn't take into account sharp edges. It will be in future versions.

@param[in] mesh Mesh to convert.
@param[out] verts Object to store result verticies.*/
void IndexedGeometryConverter::MakeStrip( IndexedGeometry& mesh, Float3AttributeChannelPtr verts)
{
    usedRangeIndex = 0;

    std::vector<IndexType>& indicies = mesh.GetIndicies();
    std::vector<glm::vec3>& verticies = mesh.GetVerticies();
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
bool IndexedGeometryConverter::findNeighbour( unsigned int index1, unsigned int index2, unsigned int& foundIndex, std::vector<IndexType>& indicies )
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
bool IndexedGeometryConverter::findNeighbourPair( unsigned int index1, unsigned int& foundIndex1, unsigned int& foundIndex2, std::vector<IndexType>& indicies )
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

void IndexedGeometryConverter::findFirstUnusedTriangle( unsigned int& index1 , std::vector<IndexType>& indicies )
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

// ========================================================================= //
// Trianglifier
// ========================================================================= //

// ***********************
//
void    IndexedGeometryConverter::MakeTriangles           ( IndexedGeometry & mesh, Float3AttributeChannelPtr verts )
{
    auto & indicies = mesh.GetIndicies();
    auto & verticies = mesh.GetVerticies();

    MakeTriangles( verticies, indicies, verts );
}

// ***********************
//
void    IndexedGeometryConverter::MakeTriangles           ( const std::vector< glm::vec3 > & verticies, const std::vector< IndexType > & indicies, Float3AttributeChannelPtr verts )
{
    for( auto index : indicies )
    {
        verts->AddAttribute( verticies[ index ] );
    }
}

// ***********************
//
void    IndexedGeometryConverter::MakeTriangles             ( const std::vector< glm::vec2 > & srcUVs, const std::vector< IndexType > & indicies, Float2AttributeChannelPtr uvs )
{
    for( auto index : indicies )
    {
        uvs->AddAttribute( srcUVs[ index ] );
    }
}

// ========================================================================= //
// Geometry indexer
// ========================================================================= //

// ***********************
//
IndexedGeometry     IndexedGeometryConverter::MakeIndexGeomFromStrips     ( Float3AttributeChannelPtr verts )
{
    m_conversionIndicies.clear();

    auto const & srcVertices = std::const_pointer_cast< Float3AttributeChannel >( verts )->GetVertices();

    IndexedGeometry mesh;
    auto & vertices = mesh.GetVerticies();
    auto & indices = mesh.GetIndicies();
    auto vertsNum = srcVertices.size();

    // I assume that first two verticies aren't degenerated.
    vertices.push_back( srcVertices[ 0 ] );
    vertices.push_back( srcVertices[ 1 ] );

    UInt32 prevIdx = 1;
    UInt32 prePrevIdx = 0;

    if( m_rememberConversion )
    {
        m_conversionIndicies.push_back( 0 );
        m_conversionIndicies.push_back( 1 );
    }

    for( UInt32 i = 2; i < vertsNum; ++i )
    {
        auto it = std::find_if( vertices.begin(), vertices.end(), [ & ]( const glm::vec3 & vert ){
            return glm::length( vert - srcVertices[ i ] ) < m_epsilon;
        });

        if( it == vertices.end() )
        {
            vertices.push_back( srcVertices[ i ] );
            if( m_rememberConversion )
                m_conversionIndicies.push_back( i );

            // Beacause source geometry was triangle strip, we must emit 3 indicies instead of 1.
            if( i & 0x1 )   // Check parity.
            {
                // Triangles ordering in triangle strips changes evenry triangle.
                indices.push_back( (IndexType)prePrevIdx );
                indices.push_back( (IndexType)( vertices.size() - 1 ) );
                indices.push_back( (IndexType)prevIdx );
            }
            else
            {
                indices.push_back( (IndexType)prePrevIdx );
                indices.push_back( (IndexType)prevIdx );
                indices.push_back( (IndexType)( vertices.size() - 1 ) );
            }

            prePrevIdx = prevIdx;
            prevIdx = (IndexType)vertices.size() - 1;  // It's the newest vertex index.
        }
        else
        {
            auto idx = ( UInt32 )( std::distance( vertices.begin(), it ) );

            // Beacause source geometry was triangle strip, we must emit 3 indicies instead of 1.
            if( i & 0x1 )   // Check parity.
            {
                // Triangles ordering in triangle strips changes evenry triangle.
                indices.push_back( (IndexType)prePrevIdx );
                indices.push_back( (IndexType)idx );
                indices.push_back( (IndexType)prevIdx );
            }
            else
            {
                indices.push_back( (IndexType)prePrevIdx );
                indices.push_back( (IndexType)prevIdx );
                indices.push_back( (IndexType)idx );
            }

            prePrevIdx = prevIdx;
            prevIdx = idx;  // It's the newest vertex index.
        }
    }

    // Remove degenerated triangles.
    for( int i = 0; i < ( Int32 )indices.size(); i += 3 )
    {
        if( indices[ i ] == indices[ i + 1 ]     ||
            indices[ i + 1 ] == indices[ i + 2 ] ||
            indices[ i + 2 ] == indices[ i ] )
        {
            auto iter = indices.erase( indices.begin() + i, indices.begin() + ( i + 3 ) );
            i = (int)std::distance( indices.begin(), iter );
        }
    }

    return mesh;
}

// ***********************
//
IndexedGeometry     IndexedGeometryConverter::MakeIndexGeomFromTriangles  ( Float3AttributeChannelPtr verts )
{
    m_conversionIndicies.clear();

    auto const & srcVertices = std::const_pointer_cast< Float3AttributeChannel >( verts )->GetVertices();

    IndexedGeometry mesh;
    auto & vertices = mesh.GetVerticies();
    auto & indices = mesh.GetIndicies();
    auto vertsNum = srcVertices.size();

    indices.reserve( vertsNum );

    for( UInt32 i = 0; i < vertsNum; ++i )
    {
        auto it = std::find_if( vertices.begin(), vertices.end(), [ & ]( const glm::vec3 & vert ){
            return glm::length( vert - srcVertices[ i ] ) < m_epsilon;
        });

        if( it == vertices.end() )
        {
            vertices.push_back( srcVertices[ i ] );
            if( m_rememberConversion )
                m_conversionIndicies.push_back( i );

            indices.push_back( (IndexType)( vertices.size() - 1 ) );
        }
        else
        {
            auto idx = ( UInt32 )( std::distance( vertices.begin(), it ) );
            indices.push_back( (IndexType)idx );
        }
    }

    assert( indices.size() == vertsNum );

    return mesh;
}

// ***********************
//
template< typename ChannelType, typename AttribType >
std::vector< AttribType >   IndexedGeometryConverter::ConvertFromMemoryImpl       ( ChannelType channel )
{
    std::vector< AttribType > destAttrib;
    auto & srcAttrib = channel->GetVertices();

    destAttrib.reserve( m_conversionIndicies.size() );

    for( auto index : m_conversionIndicies )
    {
        destAttrib.push_back( srcAttrib[ index ] );
    }

    return destAttrib;
}

// ***********************
//
std::vector< glm::vec3 >    IndexedGeometryConverter::ConvertFromMemory           ( Float3AttributeChannelPtr channel )
{
    return ConvertFromMemoryImpl< Float3AttributeChannelPtr, glm::vec3 >( channel );
}

// ***********************
//
std::vector< glm::vec2 >    IndexedGeometryConverter::ConvertFromMemory           ( Float2AttributeChannelPtr channel )
{
    return ConvertFromMemoryImpl< Float2AttributeChannelPtr, glm::vec2 >( channel );
}

}   // model
}   // bv