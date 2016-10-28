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


TEST_CASE( "Testing Triangulator [Loading SelfIntersecting SVGs]" )
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



    SECTION( "Loading file : [Spiral1.svg]")
    {
	    int contourSizeArray[] = { 51 };
	    int nestingArray[] = { 0 };
	    int intersectsSizes[] = { 2 };
	    glm::vec2 intersectsArray[] = { glm::vec2( 2.79864, -3.15698 ), glm::vec2( 2.36741, -4.10785 ) };

	    bool includingArray[] =
	    {
		    false
	    };

	    TestFileWithArrays( "Spiral1.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [Spiral2.svg]")
    {
	    int contourSizeArray[] = { 62 };
	    int nestingArray[] = { 0 };
	    int intersectsSizes[] = { 3 };
	    glm::vec2 intersectsArray[] = { glm::vec2( 3.37728, -6.46237 ), glm::vec2( 1.16602, -7.83002 ), glm::vec2( 3.07736, -9.20758 ) };

	    bool includingArray[] =
	    {
		    false
	    };

	    TestFileWithArrays( "Spiral2.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [OuterSpiral.svg]")
    {
	    int contourSizeArray[] = { 51 };
	    int nestingArray[] = { 0 };
	    int intersectsSizes[] = { 1 };
	    glm::vec2 intersectsArray[] = { glm::vec2( 2.95125, -1.30068 ) };

	    bool includingArray[] =
	    {
		    false
	    };

	    TestFileWithArrays( "OuterSpiral.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [MultiIntersection.svg]")
    {
	    int contourSizeArray[] = { 114 };
	    int nestingArray[] = { 0 };
	    int intersectsSizes[] = { 7 };
	    glm::vec2 intersectsArray[] = { glm::vec2( 2.94624, -1.07299 ), glm::vec2( 4.17959, -2.05895 ), glm::vec2( 1.01341, -2.40463 ), glm::vec2( 2.37658, -3.37852 ), glm::vec2( 4.07289, -4.76194 ), glm::vec2( 2.96331, -5.12286 ), glm::vec2( 3.39318, -5.6013 ) };

	    bool includingArray[] =
	    {
		    false
	    };

	    TestFileWithArrays( "MultiIntersection.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [InnerZigZag.svg]")
    {
	    int contourSizeArray[] = { 107 };
	    int nestingArray[] = { 0 };
	    int intersectsSizes[] = { 6 };
	    glm::vec2 intersectsArray[] = { glm::vec2( 4.22776, -0.769686 ), glm::vec2( 2.28266, -1.08817 ), glm::vec2( 4.71113, -1.39482 ), glm::vec2( 5.11492, -1.77449 ), glm::vec2( 4.00326, -1.78721 ), glm::vec2( 5.47355, -2.34524 ) };

	    bool includingArray[] =
	    {
		    false
	    };

	    TestFileWithArrays( "InnerZigZag.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [InnerSpiral.svg]")
    {
	    int contourSizeArray[] = { 48 };
	    int nestingArray[] = { 0 };
	    int intersectsSizes[] = { 1 };
	    glm::vec2 intersectsArray[] = { glm::vec2( 3.37773, -0.642526 ) };

	    bool includingArray[] =
	    {
		    false
	    };

	    TestFileWithArrays( "InnerSpiral.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }



    SECTION( "Loading file : [ContourHole.svg]")
    {
	    int contourSizeArray[] = { 78 };
	    int nestingArray[] = { 0 };
	    int intersectsSizes[] = { 5 };
	    glm::vec2 intersectsArray[] = { glm::vec2( 4.05415, -1.20441 ), glm::vec2( 3.03898, -1.26438 ), glm::vec2( 5.29988, -1.81253 ), glm::vec2( 3.95207, -1.81505 ), glm::vec2( 4.3889, -2.85595 ) };

	    bool includingArray[] =
	    {
		    false
	    };

	    TestFileWithArrays( "ContourHole.svg", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );
    }

}


