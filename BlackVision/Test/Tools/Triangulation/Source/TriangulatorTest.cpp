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



TEST_CASE( "Testing Triangulator" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultMeshPluginPtr plugin = std::static_pointer_cast< bv::model::DefaultMeshPlugin >( pluginsManager.CreatePlugin( "DEFAULT_MESH", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );


    REQUIRE( plugin != nullptr );



    SECTION( "Loading file: [cherries.svg]" )
    {
        int contourSizeArray[] = { 70, 70, 70, 185, 20, 40, 51, 91, 101, 60, 60 };
        int nestingArray[] = { 2, 2, 2, 0, 1, 1, 1, 1, 1, 2, 2 };
        //bool includingArray[]

        int numContours = sizeof( contourSizeArray ) / sizeof( int );

        std::vector< int > sizeArray;                               sizeArray.resize( numContours );
        std::vector< int > nesting;                                 nesting.resize( numContours );
        std::vector< std::vector< glm::vec2 > > intersections;      intersections.resize( numContours );
        std::vector< std::vector< bool > > including;               including.resize( numContours );

        std::copy( contourSizeArray, contourSizeArray + sizeArray.size(), sizeArray.begin() );
        std::copy( nestingArray, nestingArray + nesting.size(), nesting.begin() );

        TestSVGFile( "cherries.svg",
                     plugin,
                     triangulate,
                     sizeArray,
                     nesting,
                     intersections,
                     including
                     );
    }

    SECTION( "Loading file: [coins.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "coins.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [drugs.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "drugs.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [group.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "group.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [house.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "house.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [kupa_ptak.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "kupa_ptak.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [kupa_twarz.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "kupa_twarz.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [piggy-bank.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "piggy-bank.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [pineapple.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "pineapple.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

    SECTION( "Loading file: [settings.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "settings.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }


    SECTION( "Loading file: [smartphone.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "smartphone.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }


    SECTION( "Loading file: [SomeShapes.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "SomeShapes.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }


    SECTION( "Loading file: [tea-cup.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "tea-cup.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );
    }

// ***********************
// Not working

    SECTION( "Loading file: [cheese.svg]" )
    {
        int contourSizeArray[] = { 280, 170, 60, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 561, 105, 121, 758 };
        int numContours = sizeof( contourSizeArray ) / sizeof( int );

        std::vector< int > sizeArray;                               sizeArray.resize( numContours );
        std::vector< int > nesting;                                 nesting.resize( numContours );
        std::vector< std::vector< glm::vec2 > > intersections;      intersections.resize( numContours );
        std::vector< std::vector< bool > > including;               including.resize( numContours );

        std::copy( contourSizeArray, contourSizeArray + sizeArray.size(), sizeArray.begin() );

        TestSVGFile( "cheese.svg",
                     plugin,
                     triangulate,
                     sizeArray,
                     nesting,
                     intersections,
                     including
                     );
    }

    SECTION( "Loading file: [apple.svg]" )
    {
        auto assetDesc = projectManager->GetAssetDesc( "", "svgs", "apple.svg" );
        REQUIRE( assetDesc != nullptr );

        bool result = plugin->LoadResource( assetDesc );
        REQUIRE( result == true );

        auto attributesChannel = plugin->GetVertexAttributesChannel();
        REQUIRE( attributesChannel != nullptr );

        auto components = attributesChannel->GetComponents();
        CHECK( components.size() == 1 );
        REQUIRE( components.size() > 0 );

        auto contours = triangulate->ExtractContours( components[ 0 ] );
        CHECK( contours.size() == 6 );

        Triangulator triangulator( std::move( contours ) );
        REQUIRE_THROWS( auto mesh = triangulator.MakeMesh() );
    }

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
    Triangulator triangulator( std::move( contours ) );

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

}
