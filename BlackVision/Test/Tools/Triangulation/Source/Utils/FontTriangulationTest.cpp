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


    SECTION( "Testing letter a" )
    {
        SetParameter( plugin->GetParameter( "text" ), 0.0, std::wstring( L"a" ) );

        SECTION( "Loading file: [witek/Alice and the Wicked Monster.ttf]" )
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

            TestFileWithArrays( "witek/Alice and the Wicked Monster.ttf", plugin, triangulate, contourSizeArray, nestingArray, includingArray );
        }
    }


}

