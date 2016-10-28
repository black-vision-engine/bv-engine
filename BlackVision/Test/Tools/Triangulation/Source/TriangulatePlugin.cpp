#pragma warning ( disable : 4100 )
#include "Catch/catch.hpp"

#include "ProjectManager.h"
#include "ConfigManager.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Utils/PluginUtils.h"

#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/TriangulatePlugin.h"



namespace bv {
namespace model
{
DEFINE_PTR_TYPE( DefaultMeshPlugin );
DEFINE_PTR_TYPE( TriangulatePlugin );
}
}


//
//TEST_CASE( "Testing TriangulatePlugin" )
//{
//    // Test needs config.xml placed in exe directory.
//    REQUIRE( bv::ConfigManager::LoadXMLConfig() );
//
//    auto & pluginsManager = bv::model::PluginsManager::DefaultInstanceRef();
//    //auto projectManager = bv::ProjectManager::GetInstance();
//
//    pluginsManager.RegisterDescriptors( pluginUtils::CreatePlugins() );
//    auto timeEvaluator = bv::model::TimelineHelper::CreateTimeEvaluator( "default", bv::TimelineType::TT_DEFAULT );
//
//    bv::model::DefaultMeshPluginPtr plugin = std::static_pointer_cast<bv::model::DefaultMeshPlugin>( pluginsManager.CreatePlugin( "DEFAULT_MESH", "SVG", nullptr, timeEvaluator ) );
//    bv::model::TriangulatePluginPtr triangulate = std::static_pointer_cast<bv::model::TriangulatePlugin>( pluginsManager.CreatePlugin( "TRIANGULATE", "Triangulator", nullptr, timeEvaluator ) );
//
//    triangulate->SetPrevPlugin( plugin );
//
//    REQUIRE( plugin != nullptr );
//    REQUIRE( triangulate != nullptr );
//
//    //triangulate->
//
//}