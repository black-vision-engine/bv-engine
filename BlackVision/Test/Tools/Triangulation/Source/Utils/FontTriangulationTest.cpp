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
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultText3DPlugin.h"

#include <math.h>
#include <vector>



TEST_CASE( "Testing Triangulator [Loading Fonts]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"a" ) );



    SECTION( "Loading file: [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 180, 79 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 210, 199, 79, 79 };
        int nestingArray[] = { 0, 1, 2, 3 };
        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 210, 79 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 280 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 382 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }


    SECTION( "Loading file: [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 381 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 166, 65 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 10, 19, 120, 24, 35, 16, 95 };
        int nestingArray[] = { 1, 1, 0, 1, 1, 1, 1 };
        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            true,	true,	false,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 82, 184 };
        int nestingArray[] = { 1, 0 };
        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 139, 43 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 240 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 240 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 84, 7, 40, 34 };
        int nestingArray[] = { 0, 1, 1, 1 };
        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 76, 10 };
        int nestingArray[] = { 0, 1 };
        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 236 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 236 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 357 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

    SECTION( "Loading file: [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 317 };
        int nestingArray[] = { 0 };
        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
    }

}

// ***********************
// [ Letter b Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter b]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"b" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 145, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 311, 289, 79, 79 };
        int nestingArray[] = { 0, 1, 2, 3 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 301, 79 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 170, 64 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 342, 64, 101 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 376, 75, 99 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 157, 89 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 132, 30, 15, 75, 45 };
        int nestingArray[] = { 0, 1, 1, 1, 2 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	true,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 115, 179 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 93, 48 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 210 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 210 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 73, 11, 39, 60 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 75, 20, 20 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 158, 58 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 158, 48 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 392, 34 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 233, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }

}


// ***********************
// [ Letter c Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter c]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"c" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 160 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 200, 169 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 200 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 230 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 261 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 260 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 190 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 129 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 235 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 117 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 160 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 160 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 126, 34 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 65 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 237 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 232 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 195 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 200 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter d Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter d]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"d" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 150, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 291, 300, 79, 79 };
        int nestingArray[] = { 0, 1, 2, 3 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 285, 79 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 275, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 355, 130, 80 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 458, 84 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 222, 60 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 6, 76, 120, 4, 155, 27 };
        int nestingArray[] = { 1, 1, 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,
            true,	true,	false,	true,	true,	true,
            false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 120, 166 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 111, 47 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 243 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 243 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 109, 11, 50, 29 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 25, 88 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 158, 58 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 158, 48 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 371, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 256, 66 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



}


// ***********************
// [ Letter e Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter e]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"e" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 140, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 195, 179, 59, 49 };
        int nestingArray[] = { 0, 1, 2, 3 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 195, 49 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 175, 49 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 235, 79 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 236, 90 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 155, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 13, 6, 11, 156, 10, 6, 25, 19 };
        int nestingArray[] = { 1, 1, 1, 0, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,
            true,	true,	true,	false,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 35, 210 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 118, 50 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 35, 144 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 35, 144 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 91, 49, 39 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 104 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 150 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 149 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 165, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 185, 34 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }

}



// ***********************
// [ Letter f Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter f]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"f" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 211 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 398, 286 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 375 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 190, 44 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 238, 95 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 239, 80 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 193 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 259 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 444 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 98, 31 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 300 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 300 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 37, 7 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 82 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 169 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 169 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 246, 54 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 265, 69, 18, 15 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter g Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter g]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"g" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 190, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 305, 284, 79, 79 };
        int nestingArray[] = { 0, 1, 2, 3 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 255, 79 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 322 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 442, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 444, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 197, 60 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 148, 24, 14, 40, 19, 31, 30 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 245, 60 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 102, 50, 25 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 35, 265 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 35, 265 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 85, 40, 7, 34 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 95 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 158, 126 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 163, 131 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 377, 74 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 322, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



}




// ***********************
// [ Letter h Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter h]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"h" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 160 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 385, 319 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 385 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 372 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 347, 76 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 449, 85 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 204 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 222 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 294 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 147 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 1 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.0614991, 0.144435 ) };

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 35, 261 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 35, 261 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 118, 7, 30 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 104 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 325 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 305 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 297, 34 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 287 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}




// ***********************
// [ Letter i Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter i]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"i" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 70, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 208, 55, 45, 44, 44, 29, 39, 44, 59, 204, 149 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	true,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 208, 55, 45, 44, 44, 29, 39, 44, 59, 204 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	true,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 180, 49 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 206, 79 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 201, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 162, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 106, 14, 119, 19, 55, 21, 22 };
        int nestingArray[] = { 0, 1, 0, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	true,	false,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 145, 59 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 21, 115 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 106, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 106, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 40, 38, 34, 7 };
        int nestingArray[] = { 0, 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	true,	false,
            false,	false,	false,	true,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 55, 14, 25, 24 };
        int nestingArray[] = { 0, 1, 1, 0 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	false,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 40, 38 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 35, 23 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 151, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 190, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



}





// ***********************
// [ Letter j Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter j]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"j" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 150, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 170, 49, 39, 39, 49, 29, 39, 49, 304, 254 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 170, 49, 39, 39, 49, 29, 39, 49, 304 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 0 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 190, 49 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 264, 79, 59 };
        int nestingArray[] = { 0, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 296, 69, 39 };
        int nestingArray[] = { 0, 1, 0 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	false,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 122, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 201 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 150, 204 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 21, 110, 25 };
        int nestingArray[] = { 0, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            false,	false,	true,
            false,	false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 215, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 215, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 40, 28, 34, 7 };
        int nestingArray[] = { 0, 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	true,	false,
            false,	false,	false,	true,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 52, 19, 54, 14, 25 };
        int nestingArray[] = { 0, 1, 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	false,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	true,	true,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 108, 39 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 108, 34 };
        int nestingArray[] = { 0, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 175, 39, 74 };
        int nestingArray[] = { 0, 0, 0 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 170, 39, 39 };
        int nestingArray[] = { 0, 1, 0 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	false,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}


// ***********************
// [ Letter k Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter k]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"k" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 155, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 465, 429 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 455 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 366 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 504, 91 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 451, 94 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 246 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CookieMonster.ttf]" )
    {
        int contourSizeArray[] = { 165 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CookieMonster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 276 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 118, 26 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 35, 253, 41 };
        int nestingArray[] = { 1, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            true,	false,	true,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 35, 253, 41 };
        int nestingArray[] = { 1, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            true,	false,	true,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 157, 29, 7, 19 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 82 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 187 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 187 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 362, 34 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 284 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter l Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter l]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"l" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 80 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 250, 194 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 250 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 170 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 247, 89 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 253, 99 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 147 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 160 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 65, 36 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 40, 134 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 40, 134 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 64, 11 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 45 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 39 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 24 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 160, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 206 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}


// ***********************
// [ Letter m Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter m]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
    //auto projectManager = bv::ProjectManager::GetInstance();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"m" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 200 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 450, 409 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 450 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 450 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 465, 15 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 552 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 313 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 468 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 235 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 282 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 282 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 165, 30, 12, 7 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 135 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 483 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 473 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 383 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 412 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter n Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter n]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"n" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 180 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 340, 304 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 270 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 360 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 310, 24 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 374 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 225 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 278 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 169 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 1 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.0600243, 0.144208 ) };

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 196 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 196 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 127, 30, 12 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 75 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 315 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 300 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 243 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 298 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}


// ***********************
// [ Letter o Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter o]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"o" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 120, 39, 59 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 110, 149, 234 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 1, 0 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.0319245, 0.0980277 ) };

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 110, 149 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 1 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.0317192, 0.0988746 ) };

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 140, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 211, 79, 54 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 213, 80, 71 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 90, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 230 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 55, 54, 30 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 165 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 165 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 80, 49, 29 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 35, 39 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 42, 172 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 42, 172 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 130, 60, 54 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 186, 70, 54 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}


// ***********************
// [ Letter p Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter p]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"p" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 171, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 255, 254, 99, 79 };
        int nestingArray[] = { 0, 1, 2, 3 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 255, 89 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 270, 89 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 425, 99 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 519, 79 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 192, 60 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 250 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 123, 55 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 225 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 225 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 55, 29, 40, 3 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 78, 24 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 163, 58 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 163, 48 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 466 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 275, 74, 49 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }

}


// ***********************
// [ Letter q Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter q]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"q" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 160, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 295, 329, 79, 69 };
        int nestingArray[] = { 0, 1, 2, 3 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	true,	true,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 295, 99 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 325 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 367 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 477 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 66, 151 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 51, 211 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 115, 46 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 333 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 333 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 80, 40, 7, 39 };
        int nestingArray[] = { 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,
            false,	false,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 105 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 172, 64 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 172, 49 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 424 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 1 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.0724091, 0.0838402 ) };

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 392, 79, 24 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}


// ***********************
// [ Letter r Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter r]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"r" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 155, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 335, 259 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 335 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 265 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 326, 49 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 313, 84 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 104 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 201 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 138 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 176 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 176 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 105, 7 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 90, 25 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 215 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 205 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 216, 44 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 256, 70 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter s Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter s]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"s" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 250 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 235, 199 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 255 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 315 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 142, 54 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 165, 100 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 211 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 295 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 128 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 216 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 216 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 230, 44 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 105 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 222 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 237 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 183, 44, 44 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 185, 49, 59 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter t Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter t]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"t" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 201 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 391, 381 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 391 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 250 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 416, 106, 21 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 351, 100, 59 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 114 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 323 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 111 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 220 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 220 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 47, 7 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 70, 14 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 227 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 222 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 319, 49, 41 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 322 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}


// ***********************
// [ Letter u Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter u]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"u" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 190 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 340, 299 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 280 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 300 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 376 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 367 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 299 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 279 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 212 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 31, 224 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 31, 224 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 72, 12, 12 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 91 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 208 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 193 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 273 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 335 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter v Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter v]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"v" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 150 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 270, 224 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 270 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 240 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 285, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 259, 76 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 285 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 276 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 94 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 31, 159 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 31, 159 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 98, 41 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 72 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 171 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 176 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 215, 34 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 260, 49 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter w Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter w]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"w" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 230 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 395, 344 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 395 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 350 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 461, 64 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 486, 75 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 60, 215, 50 };
        int nestingArray[] = { 1, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            true,	false,	true,
            false,	false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 371 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 201 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 25, 231 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 25, 231 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 136, 12, 21 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 121, 25 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 3, 193 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 194, 2 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 335, 34 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 1, 0 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.0692167, 0.0846379 ) };

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 388, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }

}



// ***********************
// [ Letter x Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter x]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"x" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 180 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 345, 324 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 345 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 321 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 339 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 334 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 296 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 231 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 215 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 276 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 276 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 149, 16 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 97 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 133 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 128 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 230 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 295 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}


// ***********************
// [ Letter z Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter z]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"z" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 211 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 245, 209 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 245 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 320 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 386, 59 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 424, 69 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 255 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 403 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 124, 25 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 303 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 303 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 135, 20, 20 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 107 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 121 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 121 };
        int nestingArray[] = { 0 };
        int intersectsSizes[] = { 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 262, 39 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 307, 59, 39 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter # Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter #]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"#" ) );


    SECTION( "Loading file : [witek/Alice and the Wicked Monster.ttf]" )
    {
        int contourSizeArray[] = { 328, 21 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 745, 43, 43, 59, 34, 558, 49, 39, 39, 84, 34, 34 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.16376, 0.183009 ), glm::vec2( 0.133794, 0.159175 ), glm::vec2( 0.14634, 0.105529 ) };

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 745, 43, 43, 59, 34, 49, 39, 39, 34 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 1, 1, 0, 0, 0, 0, 0, 0 };
        glm::vec2 intersectsArray[] = { glm::vec2( 0.16376, 0.183009 ), glm::vec2( 0.133794, 0.159175 ) };

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 347, 12 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 172, 3 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 154, 3 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 427, 50 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 21, 478 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 68, 8, 3, 12, 9, 11, 23, 31, 21, 18, 7, 11, 23, 15, 6, 44, 32, 13, 23, 5, 50, 24, 13, 20, 5, 11, 9, 32, 19, 7, 8, 8, 6, 5, 5, 6, 3, 24, 3, 4, 3, 13, 4, 24, 15, 7, 7, 11, 9, 3, 21, 3, 13, 3, 13, 13, 13, 2, 13, 2, 13, 13, 12, 13, 12, 2 };
        int nestingArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	true,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 30, 433 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 30, 433 };
        int nestingArray[] = { 1, 0 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,
            true,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 28, 3 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 221, 13 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 271, 11 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 271, 11 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 266, 21 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 180, 3 };
        int nestingArray[] = { 0, 1 };
        int intersectsSizes[] = { 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,
            false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }


}



// ***********************
// [ Letter % Test ]

TEST_CASE( "Testing Triangulator [Loading Fonts - letter %]" )
{
    // Test needs config.xml placed in exe directory.
    REQUIRE( bv::ConfigManager::LoadXMLConfig() );

    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();

    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );

    bv::model::DefaultText3DPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultText3DPlugin>( pluginsManager.CreatePlugin( "DEFAULT_TEXT3D", "SVG", nullptr, timeEvaluator ) );
    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );

    REQUIRE( plugin != nullptr );

    SetParameter( plugin->GetParameter( "fontSize" ), 0.0f, 200.0f );
    plugin->Update( 0.0f );


    SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"%" ) );


    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Hollow.ttf]" )
    {
        int contourSizeArray[] = { 405, 49, 49, 39, 34, 44, 39, 49, 205, 149, 39, 39, 44, 34, 39, 39, 40 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	true,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Hollow.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Austie Bost You Wear Flowers Solid.ttf]" )
    {
        int contourSizeArray[] = { 405, 49, 49, 39, 34, 44, 39, 49, 149, 39, 39, 44, 34, 39, 39, 40 };
        int nestingArray[] = { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,	true,	true,	true,	true,	true,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	true,	true,	true,	true,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Austie Bost You Wear Flowers Solid.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/AveFedan_PERSONAL_USE.ttf]" )
    {
        int contourSizeArray[] = { 220, 59, 49 };
        int nestingArray[] = { 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	true,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/AveFedan_PERSONAL_USE.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollFS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 43, 69, 69, 64, 64 };
        int nestingArray[] = { 0, 0, 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,
            false,	false,	false,	true,	false,
            false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollFS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/BrannbollSS_PERSONAL.ttf]" )
    {
        int contourSizeArray[] = { 80, 79, 36, 69, 69 };
        int nestingArray[] = { 0, 0, 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	true,	false,
            false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/BrannbollSS_PERSONAL.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/CalligraphyFLF.ttf]" )
    {
        int contourSizeArray[] = { 105, 99, 240, 79 };
        int nestingArray[] = { 0, 1, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	true,	false,	false,
            false,	false,	false,	false,
            false,	false,	false,	true,
            false,	false,	false,	false
        };

        TestFileWithArrays( "witek/CalligraphyFLF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Curly Shirley.ttf]" )
    {
        int contourSizeArray[] = { 86, 39, 39 };
        int nestingArray[] = { 0, 0, 0 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Curly Shirley.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/INKITMT_.otf]" )
    {
        int contourSizeArray[] = { 68, 8, 3, 12, 9, 11, 23, 31, 21, 18, 7, 11, 23, 15, 6, 44, 32, 13, 23, 5, 50, 24, 13, 20, 5, 11, 9, 32, 19, 7, 8, 8, 6, 5, 5, 6, 3, 24, 3, 4, 3, 13, 4, 24, 15, 7, 7, 11, 9, 3, 21, 3, 13, 3, 13, 13, 13, 2, 13, 2, 13, 13, 12, 13, 12, 2 };
        int nestingArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	true,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	true,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/INKITMT_.otf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles Skinny.ttf]" )
    {
        int contourSizeArray[] = { 165, 164, 79 };
        int nestingArray[] = { 0, 0, 0 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles Skinny.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Just For Giggles.ttf]" )
    {
        int contourSizeArray[] = { 165, 164, 79 };
        int nestingArray[] = { 0, 0, 0 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Just For Giggles.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/LittleLordFontleroyNF.ttf]" )
    {
        int contourSizeArray[] = { 80, 21, 79, 59, 3, 59, 39, 39 };
        int nestingArray[] = { 0, 0, 0, 1, 1, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	true,	false,	false,	true,	false,
            false,	false,	false,	false,	true,	false,	false,	false,
            false,	false,	false,	false,	false,	true,	false,	true,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/LittleLordFontleroyNF.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/New Walt Disney.ttf]" )
    {
        int contourSizeArray[] = { 122, 44, 41, 41, 19 };
        int nestingArray[] = { 0, 0, 1, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	true,	true,
            false,	false,	true,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/New Walt Disney.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet Black.ttf]" )
    {
        int contourSizeArray[] = { 40, 71, 39, 34, 34 };
        int nestingArray[] = { 0, 0, 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	true,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Ringlet Black.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Ringlet.ttf]" )
    {
        int contourSizeArray[] = { 40, 71, 39, 34, 34 };
        int nestingArray[] = { 0, 0, 0, 1, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	true,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	true,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Ringlet.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Stylish Calligraphy Demo.ttf]" )
    {
        int contourSizeArray[] = { 81, 59, 69, 59, 69 };
        int nestingArray[] = { 0, 0, 1, 0, 1 };
        int intersectsSizes[] = { 0, 0, 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,	false,	false,
            false,	false,	true,	false,	false,
            false,	false,	false,	false,	false,
            false,	false,	false,	false,	true,
            false,	false,	false,	false,	false
        };

        TestFileWithArrays( "witek/Stylish Calligraphy Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [witek/Watermelon Script Demo.ttf]" )
    {
        int contourSizeArray[] = { 42, 49, 49 };
        int nestingArray[] = { 0, 0, 0 };
        int intersectsSizes[] = { 0, 0, 0 };
        glm::vec2* intersectsArray = nullptr;

        bool includingArray[] =
        {
            false,	false,	false,
            false,	false,	false,
            false,	false,	false
        };

        TestFileWithArrays( "witek/Watermelon Script Demo.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }

}

