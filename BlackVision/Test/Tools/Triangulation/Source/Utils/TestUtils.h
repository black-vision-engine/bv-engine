#pragma once

#include "ProjectManager.h"
#include "ConfigManager.h"

#include "Assets/Font/FontAssetDescriptor.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Utils/PluginUtils.h"

#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/TriangulatePlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultText3DPlugin.h"

#include <math.h>
#include <vector>


namespace bv {
namespace model
{
DEFINE_PTR_TYPE( DefaultMeshPlugin );
DEFINE_PTR_TYPE( TriangulatePlugin );
DEFINE_PTR_TYPE( DefaultText3DPlugin );
}
}




// ***********************
// Helpers


template< typename PluginType >
inline const char *        GetCategoryName     () { return ""; }

template<>
inline const char *        GetCategoryName< bv::model::DefaultMeshPluginPtr >     () { return "svgs"; }

template<>
inline const char *        GetCategoryName< bv::model::DefaultText3DPluginPtr >    () { return "fonts"; }


template< typename PluginType >
inline bv::AssetDescConstPtr    CreateDesc  ( const char * file ) { return nullptr; }

template<>
inline bv::AssetDescConstPtr    CreateDesc< bv::model::DefaultMeshPluginPtr >  ( const char * file )
{
    auto projectManager = bv::ProjectManager::GetInstance();
    return projectManager->GetAssetDesc( "", GetCategoryName< bv::model::DefaultMeshPluginPtr >(), file );
}

template<>
inline bv::AssetDescConstPtr    CreateDesc< bv::model::DefaultText3DPluginPtr >  ( const char * file )
{
    auto projectManager = bv::ProjectManager::GetInstance();
    auto filePath = projectManager->GetRootDir() / bv::Path( GetCategoryName< bv::model::DefaultText3DPluginPtr >() ) / bv::Path( file );

    return bv::FontAssetDesc::Create( filePath.Str(), 200, 0, 0, false );
}



// ***********************
//
template< typename PluginType >
inline
void    TestFile ( const char * file,
                   PluginType meshPlugin,
                   bv::model::TriangulatePluginPtr triangulate,
                   const std::vector< int > & contoursSizes,
                   const std::vector< int > & nesting,
                   const std::vector< std::vector< glm::vec2 > > & intersections,
                   const std::vector< std::vector< bool > > & including )
{
    auto assetDesc = CreateDesc< PluginType >( file );
    REQUIRE( assetDesc != nullptr );

    bool result = meshPlugin->LoadResource( assetDesc );
    REQUIRE( result == true );

    auto attributesChannel = meshPlugin->GetVertexAttributesChannel();
    REQUIRE( attributesChannel != nullptr );

    auto components = attributesChannel->GetComponents();
    CHECK( components.size() == 1 );
    REQUIRE( components.size() > 0 );

    auto contours = triangulate->ExtractContours( components[ 0 ] );
    auto size = contours.size();

    
    size_t numIntersections = 0;
    for( auto & contourIntersect : intersections )
    {
        numIntersections += contourIntersect.size();
    }

    if( numIntersections == 0 )
    {
        CHECK( size == contoursSizes.size() );  // It's true only if triangulator makes always one contour if intersections apears.

        // ***********************
        // Check contours size
        auto maxArray = std::min( size, static_cast<decltype( size )>( contoursSizes.size() ) );
        for( int i = 0; i < maxArray; ++i )
        {
            // 
            INFO( "Contour index: " << i );
            CHECK( contoursSizes[ i ] >= contours[ i ]->PointCount() );
        }
    }
    // ***********************
    // Triangulate
    Triangulator triangulator( std::move( contours ), "testContours.txt", file );

    Mesh mesh;
    REQUIRE_NOTHROW( mesh = triangulator.MakeMesh() );


    // ***********************
    // Check polylines sizes
    auto maxArray = std::min( size, static_cast<decltype( size )>( contoursSizes.size() ) );
    for( int i = 0; i < maxArray; ++i )
    {
        INFO( "Contour index: " << i );
        CHECK( contoursSizes[ i ] >= triangulator.GetPolylines()[ i ].size() );
    }

    // ***********************
    // Check nesting
    size = nesting.size();
    maxArray = std::min( size, static_cast<decltype( size )>( triangulator.GetNestingArray().size() ) );

    CHECK( triangulator.GetNestingArray().size() == size );

    for( int i = 0; i < maxArray; i++ )
    {
        INFO( "Contour index: " << i );
        CHECK( nesting[ i ] == triangulator.GetNestingArray()[ i ] );
    }

    // ***********************
    // Check intersections
    const float epsilon = 0.000001f;
    
    size = intersections.size();
    maxArray = std::min( size, static_cast<decltype( size )>( triangulator.GetSelfIntersections().size() ) );

    CHECK( triangulator.GetSelfIntersections().size() == size );

    for( int i = 0; i < maxArray; i++ )
    {
        INFO( "Contour index: " << i );

        auto & realIntersects = triangulator.GetSelfIntersections()[ i ];
        CHECK( intersections[ i ].size() == realIntersects.size() );

        auto maxIntersect = std::min( intersections[ i ].size(), realIntersects.size() );
        for( int j = 0; j < maxIntersect; j++ )
        {
            INFO( "Intersection index: " << j );
            CHECK( abs( intersections[ i ][ j ].x - realIntersects[ j ]->x ) < epsilon );
            CHECK( abs( intersections[ i ][ j ].y - realIntersects[ j ]->y ) < epsilon );
        }

    }

    // ***********************
    // Check contours including
    size = including.size();
    maxArray = std::min( size, static_cast<decltype( size )>( triangulator.GetIncludingArray().size() ) );

    CHECK( triangulator.GetIncludingArray().size() == size );

    for( int i = 0; i < maxArray; i++ )
    {
        auto & refIncluding = including[ i ];
        auto & testIncluding = triangulator.GetIncludingArray()[ i ];

        for( int j = 0; j < maxArray; ++j )
        {
            INFO( "Outer contour : " << i );
            INFO( "Inner contour : " << j );

            CHECK( refIncluding[ j ] == testIncluding[ j ] );
        }
    }

    // ***********************
    // Compare including and nesting data
    for( int j = 0; j < maxArray; j++ )
    {
        auto & including = triangulator.GetIncludingArray();
        int num = 0;

        for( int i = 0; i < maxArray; ++i )
        {
            if( including[ i ][ j ] )
                num++;
        }

        INFO( "Inner contour : " << j );
        INFO( "Nesting and number of including contours differs" );
        CHECK( num == nesting[ j ] );

        CHECK( triangulator.GetContours()[ j ]->IsClockwise() == ( num % 2 == 0 ) );
    }

}


// @note includingCount is ContoursCount * ContoursCount but I don't know 
template< int ContoursCount, typename PluginType >
inline
void        TestFileWithArrays   ( const char * file,
                                  PluginType meshPlugin,
                                  bv::model::TriangulatePluginPtr triangulate,
                                  int ( &contourSizeArray )[ ContoursCount ],
                                  int ( &nestingArray )[ ContoursCount ],
                                  bool* includingArray
                                  )
{
    int numContours = ContoursCount;

    std::vector< int > sizeArray;                               sizeArray.resize( numContours );
    std::vector< int > nesting;                                 nesting.resize( numContours );
    std::vector< std::vector< glm::vec2 > > intersections;      intersections.resize( numContours );
    std::vector< std::vector< bool > > including;               including.resize( numContours );

    std::copy( contourSizeArray, contourSizeArray + sizeArray.size(), sizeArray.begin() );
    std::copy( nestingArray, nestingArray + nesting.size(), nesting.begin() );

    for( int i = 0; i < numContours; ++i )
    {
        including[ i ].reserve( numContours );
        for( int j = 0; j < numContours; j++ )
        {
            int idx = i * numContours + j;
            including[ i ].push_back( includingArray[ idx ] );
        }
    }

    TestFile( file,
              meshPlugin,
              triangulate,
              sizeArray,
              nesting,
              intersections,
              including
              );
}

// @note includingCount is ContoursCount * ContoursCount but I don't know 
template< int ContoursCount, typename PluginType >
inline
void        TestFileWithArrays   ( const char * file,
                                   PluginType meshPlugin,
                                   bv::model::TriangulatePluginPtr triangulate,
                                   int ( &contourSizeArray )[ ContoursCount ],
                                   int ( &nestingArray )[ ContoursCount ],
                                   bool* includingArray,
                                   int ( &intersectNumArray )[ ContoursCount ],
                                   glm::vec2* intersectsArray
                                   )
{
    int numContours = ContoursCount;

    std::vector< int > sizeArray;                               sizeArray.resize( numContours );
    std::vector< int > nesting;                                 nesting.resize( numContours );
    std::vector< std::vector< glm::vec2 > > intersections;      intersections.resize( numContours );
    std::vector< std::vector< bool > > including;               including.resize( numContours );

    std::copy( contourSizeArray, contourSizeArray + sizeArray.size(), sizeArray.begin() );
    std::copy( nestingArray, nestingArray + nesting.size(), nesting.begin() );

    for( int i = 0; i < numContours; ++i )
    {
        including[ i ].reserve( numContours );
        for( int j = 0; j < numContours; j++ )
        {
            int idx = i * numContours + j;
            including[ i ].push_back( includingArray[ idx ] );
        }
    }

    int intersectsCounter = 0;
    for( int i = 0; i < ContoursCount; ++i )
    {
        intersections[ i ].reserve( intersectNumArray[ i ] );
        for( int j = 0; j < intersectNumArray[ i ]; ++j )
        {
            intersections[ i ].push_back( intersectsArray[ intersectsCounter ] );
            intersectsCounter++;
        }
    }


    TestFile( file,
              meshPlugin,
              triangulate,
              sizeArray,
              nesting,
              intersections,
              including
              );
}


