#pragma warning ( disable : 4100 )
#include "Catch/catch.hpp"

#include "ProjectManager.h"
#include "ConfigManager.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Utils/PluginUtils.h"

#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/TriangulatePlugin.h"

#include <math.h>
#include <vector>



namespace bv {  namespace model
{   
    DEFINE_PTR_TYPE( DefaultMeshPlugin );
    DEFINE_PTR_TYPE( TriangulatePlugin );
}}


void        TestSVGFile ( const char * file,
                          bv::model::DefaultMeshPluginPtr meshPlugin,
                          bv::model::TriangulatePluginPtr triangulate,
                          const std::vector< int > & contoursSizes,
                          const std::vector< int > & nesting,
                          const std::vector< std::vector< glm::vec2 > > & intersections,
                          const std::vector< std::vector< bool > > & including
                        );

// ***********************
// Helpers

// @note includingCount is ContoursCount * ContoursCount but I don't know 
template< int ContoursCount >
void        TestSVGWithArrays   ( const char * file,
                                  bv::model::DefaultMeshPluginPtr meshPlugin,
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

    TestSVGFile( file,
                 meshPlugin,
                 triangulate,
                 sizeArray,
                 nesting,
                 intersections,
                 including
                 );
}



// ***********************
//
void    TestSVGFile ( const char * file,
                      bv::model::DefaultMeshPluginPtr meshPlugin,
                      bv::model::TriangulatePluginPtr triangulate,
                      const std::vector< int > & contoursSizes,
                      const std::vector< int > & nesting,
                      const std::vector< std::vector< glm::vec2 > > & intersections,
                      const std::vector< std::vector< bool > > & including )
{
    auto projectManager = bv::ProjectManager::GetInstance();

    auto assetDesc = projectManager->GetAssetDesc( "", "svgs", file );
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
    CHECK( size == contoursSizes.size() );

    // ***********************
    // Check contours size
    auto maxArray = std::min( size, static_cast<decltype( size )>( contoursSizes.size() ) );
    for( int i = 0; i < maxArray; ++i )
    {
        INFO( "Contour index: " << i );
        CHECK( contoursSizes[ i ] == contours[ i ]->PointCount() );
    }

    // ***********************
    // Triangulate
    Triangulator triangulator( std::move( contours ), "testSVGContours.txt", file );

    Mesh mesh;
    REQUIRE_NOTHROW( mesh = triangulator.MakeMesh() );


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
            CHECK( intersections[ i ][ j ].x == realIntersects[ j ]->x );
            CHECK( intersections[ i ][ j ].y == realIntersects[ j ]->y );
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


// ***********************
// Test cases


TEST_CASE( "Testing Triangulator [Loading SVGs]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultMeshPluginPtr plugin = std::static_pointer_cast< bv::model::DefaultMeshPlugin >( pluginsManager.CreatePlugin( "DEFAULT_MESH", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );


    REQUIRE( plugin != nullptr );



    SECTION( "Loading file: [cherries.svg]" )
    {
        int contourSizeArray[] = { 70, 70, 70, 185, 20, 40, 51, 91, 101, 60, 60 };
        int nestingArray[] = { 2, 2, 2, 0, 1, 1, 1, 1, 1, 2, 2 };
        bool includingArray[] =
        {
            false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false,
            false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,   true,   true,	false,	true,	true,	true,	true,	true,	true,	true,
            false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,   false,  true,	false,	false,	false,	false,	false,	false,	true,	false,
            false,  true,   false,  false,	false,	false,	false,	false,	false,	false,	true,
            false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false,
            false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "cherries.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [group.svg]" )
    {
        int contourSizeArray[] = { 40, 82, 66, 83 };
        int nestingArray[] = { 0, 1, 1, 1 };
        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestSVGWithArrays( "group.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [house.svg]" )
    {
        int contourSizeArray[] = { 42, 90, 169, 6, 27, 63, 91, 23, 137, 81, 90, 68, 4, 27, 68, 4, 25 };
        int nestingArray[] = { 2, 2, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 2, 3, 3 };
        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	false,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "house.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [kupa_ptak.svg]" )
    {
        int contourSizeArray[] = { 4, 192 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestSVGWithArrays( "kupa_ptak.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [kupa_twarz.svg]" )
    {
        int contourSizeArray[] = { 4, 43 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestSVGWithArrays( "kupa_twarz.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [piggy-bank.svg]" )
    {
        int contourSizeArray[] = { 40, 40, 359, 152, 272, 33, 43, 801, 40, 130, 70 };
        int nestingArray[] = { 2, 3, 0, 1, 1, 1, 1, 1, 1, 2, 2 };
        bool includingArray[] =
        {
            false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	false,	true,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "piggy-bank.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [settings.svg]" )
    {
        int contourSizeArray[] = { 304, 475, 40, 40 };
        int nestingArray[] = { 0, 1, 2, 3 };
        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestSVGWithArrays( "settings.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [smartphone.svg]" )
    {
        int contourSizeArray[] = { 40, 40, 40, 40, 52, 226, 31, 31, 31, 31, 64, 56, 55, 14, 14, 60 };
        int nestingArray[] = { 2, 3, 2, 3, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        bool includingArray[] =
        {
            false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	true,	true,	true,	false,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	true,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "smartphone.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [SomeShapes.svg]" )
    {
        int contourSizeArray[] = { 6, 80 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestSVGWithArrays( "SomeShapes.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [tea-cup.svg]" )
    {
        int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "tea-cup.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [bicycle.svg]" )
    {
        int contourSizeArray[] = { 150, 40, 23, 40 };
        int nestingArray[] = { 0, 1, 1, 1 };
        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestSVGWithArrays( "bicycle.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

// ***********************
//

    SECTION( "Loading file: [browser.svg]" )
    {
        int contourSizeArray[] = { 44, 24, 24, 80, 490, 80, 80, 80, 175 };
        int nestingArray[] = { 0, 1, 1, 3, 2, 2, 2, 2, 3 };
        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	true,	true,	true,	false,
            false,	false,	false,	true,	true,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	true,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "browser.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [cup.svg]" )
    {
        int contourSizeArray[] = { 64, 43, 88, 44, 44, 42, 44, 44, 212, 101, 81, 64, 42 };
        int nestingArray[] = { 1, 2, 1, 1, 4, 6, 3, 5, 0, 2, 2, 2, 3 };
        bool includingArray[] =
        {
            false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	true,
            false,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,	true,	false,
            false,	false,	false,	false,	false,	true,	false,	true,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	true,	true,	false,	true,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	true,	true,	true,	true,	true,	true,	false,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "cup.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [dollar-symbol.svg]" )
    {
        int contourSizeArray[] = { 62, 40, 62, 40, 62, 40, 62, 40, 92, 70, 112, 80, 102, 70, 92, 70, 172 };
        int nestingArray[] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
        bool includingArray[] =
        {
            false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "dollar-symbol.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [gold.svg]" )
    {
        int contourSizeArray[] = { 42, 42, 42, 42, 42, 42, 42 };
        int nestingArray[] = { 0, 0, 0, 0, 0, 0, 0 };
        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "gold.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [like.svg]" )
    {
        int contourSizeArray[] = { 44, 149, 4, 108 };
        int nestingArray[] = { 1, 0, 2, 1 };
        bool includingArray[] =
        {
            false,	false,	true,	false,
            true,	false,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestSVGWithArrays( "like.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [linkedin.svg]" )
    {
        int contourSizeArray[] = { 72, 44, 45, 40, 109, 112 };
        int nestingArray[] = { 0, 0, 2, 2, 2, 1 };
        bool includingArray[] =
        {
            false,	false,	true,	true,	true,	false,
            false,	false,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false
        };

        TestSVGWithArrays( "linkedin.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [money.svg]" )
    {
        int contourSizeArray[] = { 694, 4, 172, 40, 40 };
        int nestingArray[] = { 0, 1, 2, 0, 1 };
        bool includingArray[] =
        {
            false,	true,	false,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	true,	false,	true,
            false,	false,	true,	false,	false
        };

        TestSVGWithArrays( "money.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }



// ***********************
// Not working

    //SECTION( "Loading file: [cheese.svg]" )
    //{
    //    int contourSizeArray[] = { 280, 170, 60, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 561, 105, 121, 758 };
    //    int numContours = sizeof( contourSizeArray ) / sizeof( int );

    //    std::vector< int > sizeArray;                               sizeArray.resize( numContours );
    //    std::vector< int > nesting;                                 nesting.resize( numContours );
    //    std::vector< std::vector< glm::vec2 > > intersections;      intersections.resize( numContours );
    //    std::vector< std::vector< bool > > including;               including.resize( numContours );

    //    std::copy( contourSizeArray, contourSizeArray + sizeArray.size(), sizeArray.begin() );

    //    TestSVGFile( "cheese.svg",
    //                 plugin,
    //                 triangulate,
    //                 sizeArray,
    //                 nesting,
    //                 intersections,
    //                 including
    //                 );
    //}

    //SECTION( "Loading file: [apple.svg]" )
    //{
    //    auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "apple.svg" );
    //    REQUIRE( assetDesc != nullptr );

    //    bool result = plugin->LoadResource( assetDesc );
    //    REQUIRE( result == true );

    //    auto attributesChannel = plugin->GetVertexAttributesChannel();
    //    REQUIRE( attributesChannel != nullptr );

    //    auto components = attributesChannel->GetComponents();
    //    CHECK( components.size() == 1 );
    //    REQUIRE( components.size() > 0 );

    //    auto contours = triangulate->ExtractContours( components[ 0 ] );
    //    CHECK( contours.size() == 6 );

    //    Triangulator triangulator( std::move( contours ) );
    //    REQUIRE_THROWS( auto mesh = triangulator.MakeMesh() );
    //}


    //SECTION( "Loading file: [coins.svg]" )
    //{
    //    // !!!!!!
    //    // This data is copied and is surely not correct

    //    int contourSizeArray[] = { 70, 70, 70, 185, 20, 40, 51, 91, 101, 60, 60 };
    //    int nestingArray[] = { 2, 2, 2, 0, 1, 1, 1, 1, 1, 2, 2 };
    //    bool includingArray[] =
    //    {
    //        false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        true,   true,   true,	false,	true,	true,	true,	true,	true,	true,	true,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        true,   false,  true,	false,	false,	false,	false,	false,	false,	true,	false,
    //        false,  true,   false,  false,	false,	false,	false,	false,	false,	false,	true,
    //        false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "coins.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [drugs.svg]" )
    //{
    //    // !!!!!!
    //    // This data is copied and is surely not correct


    //    int contourSizeArray[] = { 70, 70, 70, 185, 20, 40, 51, 91, 101, 60, 60 };
    //    int nestingArray[] = { 2, 2, 2, 0, 1, 1, 1, 1, 1, 2, 2 };
    //    bool includingArray[] =
    //    {
    //        false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        true,   true,   true,	false,	true,	true,	true,	true,	true,	true,	true,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        true,   false,  true,	false,	false,	false,	false,	false,	false,	true,	false,
    //        false,  true,   false,  false,	false,	false,	false,	false,	false,	false,	true,
    //        false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false,
    //        false,  false,  false,  false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "drugs.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [pineapple.svg]" )
    //{
    //    // !!!!!!
    //    // This data is copied and is surely not correct


    //    int contourSizeArray[] = { 304, 475, 40, 40 };
    //    int nestingArray[] = { 0, 1, 2, 3 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,
    //        false,	false,	true,	true,
    //        false,	false,	false,	true,
    //        false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "pineapple.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/people.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/people.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/chef.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/chef.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/ecology.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/ecology.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/eevee.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/eevee.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}


    //SECTION( "Loading file: [nottested/pikachu.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/pikachu.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/pokeball.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/pokeball.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/quora.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/quora.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/route.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/route.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/settings2.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/settings2.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/settings3.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/settings3.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/swarm.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/swarm.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/tablet.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/tablet.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/teamwork.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/teamwork.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/telegram.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/telegram.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/trash.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/trash.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/viber.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/viber.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/webcam.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/webcam.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    //SECTION( "Loading file: [nottested/weedle.svg]" )
    //{
    //    int contourSizeArray[] = { 178, 30, 5, 70, 84, 130, 130, 130, 70 };
    //    int nestingArray[] = { 0, 1, 1, 1, 1, 0, 0, 0, 2 };
    //    bool includingArray[] =
    //    {
    //        false,	true,	true,	true,	true,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	true,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false,
    //        false,	false,	false,	false,	false,	false,	false,	false,	false
    //    };

    //    TestSVGWithArrays( "nottested/weedle.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}


}



