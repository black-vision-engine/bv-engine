#pragma warning ( disable : 4100 )
#include "Catch/catch.hpp"

#include "ProjectManager.h"
#include "ConfigManager.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Utils/PluginUtils.h"
#include "Utils/TestUtils.h"

#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/TriangulatePlugin.h"

#include <math.h>
#include <vector>






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



    SECTION( "Loading file : [cherries.svg]" )
    {
        int contourSizeArray[] = { 70, 70, 70, 185, 20, 40, 51, 91, 101, 60, 60 };
        int nestingArray[] = { 2, 2, 2, 0, 1, 1, 1, 1, 1, 2, 2 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	true,	false,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            true,	false,	true,	false,	false,	false,	false,	false,	false,	true,	false,
            false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "cherries.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
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

        TestFileWithArrays( "group.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "house.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "kupa_ptak.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "kupa_twarz.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "piggy-bank.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "settings.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "smartphone.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "SomeShapes.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "tea-cup.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "bicycle.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "browser.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "cup.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "dollar-symbol.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "gold.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "like.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "linkedin.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
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

        TestFileWithArrays( "money.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }




}

TEST_CASE( "Triangulator failure [Loading SVGs]" )
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


    // ***********************
    // Not working

    SECTION( "Loading file: [notworking/cheese.svg]" )
    {
        // !!!!!!
        // This data is copied and is surely not correct

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

        TestFileWithArrays( "notworking/cheese.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/apple.svg]" )
    {
        // !!!!!!
        // This data is copied and is surely not correct

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

        TestFileWithArrays( "notworking/apple.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [notworking/coins.svg]" )
    {
        // !!!!!!
        // This data is copied and is surely not correct

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

        TestFileWithArrays( "notworking/coins.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/drugs.svg]" )
    {
        // !!!!!!
        // This data is copied and is surely not correct


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

        TestFileWithArrays( "notworking/drugs.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/pineapple.svg]" )
    {
        // !!!!!!
        // This data is copied and is surely not correct


        int contourSizeArray[] = { 304, 475, 40, 40 };
        int nestingArray[] = { 0, 1, 2, 3 };
        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "notworking/pineapple.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    // Test below crashes test

    //SECTION( "Loading file: [notworking/people.svg]" )
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

    //    TestFileWithArrays( "notworking/people.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    //}

    SECTION( "Loading file: [notworking/chef.svg]" )
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

        TestFileWithArrays( "notworking/chef.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/ecology.svg]" )
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

        TestFileWithArrays( "notworking/ecology.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/eevee.svg]" )
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

        TestFileWithArrays( "notworking/eevee.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [notworking/pikachu.svg]" )
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

        TestFileWithArrays( "notworking/pikachu.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [nottested/pokeball.svg]" )
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

        TestFileWithArrays( "notworking/pokeball.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [nottested/quora.svg]" )
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

        TestFileWithArrays( "notworking/quora.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/route.svg]" )
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

        TestFileWithArrays( "notworking/route.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/settings2.svg]" )
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

        TestFileWithArrays( "notworking/settings2.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/settings3.svg]" )
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

        TestFileWithArrays( "notworking/settings3.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/swarm.svg]" )
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

        TestFileWithArrays( "notworking/swarm.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/tablet.svg]" )
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

        TestFileWithArrays( "notworking/tablet.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/teamwork.svg]" )
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

        TestFileWithArrays( "notworking/teamwork.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/telegram.svg]" )
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

        TestFileWithArrays( "notworking/telegram.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/trash.svg]" )
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

        TestFileWithArrays( "notworking/trash.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/viber.svg]" )
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

        TestFileWithArrays( "notworking/viber.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/webcam.svg]" )
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

        TestFileWithArrays( "notworking/webcam.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [notworking/weedle.svg]" )
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

        TestFileWithArrays( "notworking/weedle.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


}

