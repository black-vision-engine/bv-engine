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


    SECTION( "Testing letter a" )
    {
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


}

