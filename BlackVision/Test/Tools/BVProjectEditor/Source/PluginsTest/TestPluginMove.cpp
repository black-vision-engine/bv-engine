#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


#include "../Utils/TwoScenesFixture.h"


// ***********************
// Moves plugin in the same node (changes plugins order).
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.Plugin.Move, MoveInNode )
{
    auto editor = GetProjectEditor();
    auto scene = CreateOneSceneWithColoredRect( editor, "FirstScene" );

    REQUIRE CHECK( editor->MovePlugin( scene->GetName(), "root/ColoredRect", 0, scene->GetName(), "root/ColoredRect", "solid color" ) );

    auto node = editor->GetNode( scene->GetName(), "root/ColoredRect" );
    auto plugin = node->GetPluginList()->GetPlugin( 0 );

    CHECK( plugin->GetName() == "solid color" );
}


// ***********************
// Moves plugin between two different node.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.Plugin.Move, MoveBetweenNodes )
{
    auto editor = GetProjectEditor();
    auto scene = CreateOneSceneWithColoredRect( editor, "FirstScene" );

    REQUIRE CHECK( editor->MovePlugin( scene->GetName(), "root/Group1", 1, scene->GetName(), "root/ColoredRect", "solid color" ) );

    auto node = editor->GetNode( scene->GetName(), "root/Group1" );
    CHECK( node->GetPlugin( "solid color" ) );
}

