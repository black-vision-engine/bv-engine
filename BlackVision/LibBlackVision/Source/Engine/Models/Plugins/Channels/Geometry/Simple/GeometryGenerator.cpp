#include "stdafx.h"
#include "GeometryGenerator.h"


namespace bv { namespace model {


// *********************************
// assumption that Float3AttributeChannelPtr stores triangle strips
void    GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips        ( Float3AttributeChannelConstPtr posChannel, Float3AttributeChannelPtr normChannel )
{
    auto eps = ( Float32 )10e-5;

    auto vertsNum = posChannel->GetNumEntries();
    auto const & vertices = std::const_pointer_cast< Float3AttributeChannel >( posChannel )->GetVertices();

    // build indicies mapping 
    auto uniqueVertices = std::vector< glm::vec3 >();
    auto indices = std::vector< UInt32 >( vertsNum );

    for( UInt32 i = 0; i < vertsNum; ++i )
    {
        auto it = std::find_if( uniqueVertices.begin(), uniqueVertices.end(), [ & ]( const glm::vec3 & vert ){
            return glm::length( vert - vertices[ i ] ) < eps;
        });

        if( it == uniqueVertices.end() )
        {
            uniqueVertices.push_back( vertices[ i ] );
            indices[ i ] = ( UInt32 )( uniqueVertices.size() - 1 );
            auto indiciesVec = std::vector< UInt32 >();
            indiciesVec.push_back( i );
        }
        else
        {
            auto idx = ( UInt32 )( std::distance( uniqueVertices.begin(), it ) );
            indices[ i ] = idx;
        }
    }

    // compute not weighted normals
    auto uniqueNormals = std::vector< glm::vec3 >( uniqueVertices.size(), glm::vec3( 0.0f ) );
    
    auto normal = glm::vec3( 0.0f );
    // 'first' triangle in strip
    for( UInt32 i = 0; i < vertsNum - 2; i += 2 )
    {
        normal = glm::cross( vertices[ i + 2 ] - vertices[ i ], vertices[ i + 1 ] - vertices[ i ] );
        uniqueNormals[ indices[ i ] ] += normal;
        uniqueNormals[ indices[ i + 1 ] ] += normal;
        uniqueNormals[ indices[ i + 2 ] ] += normal;
    }

    // 'second' triangle in strip
    for( UInt32 i = 0; i < vertsNum - 3; i += 2 )
    {
        normal = glm::cross( vertices[ i + 3 ] - vertices[ i + 2 ], vertices[ i + 1 ] - vertices[ i + 2 ] );
        uniqueNormals[ indices[ i + 2 ] ] += normal;
        uniqueNormals[ indices[ i + 1 ] ] += normal;
        uniqueNormals[ indices[ i + 3 ] ] += normal;
    }

    for( auto & normal : uniqueNormals )
    {
        normal = glm::normalize( normal ); 
    }

    for( UInt32 i = 0; i < vertsNum; ++i )
    {
        normChannel->AddAttribute( uniqueNormals[ indices[ i ] ] );
    }
}

} //bv
} //model