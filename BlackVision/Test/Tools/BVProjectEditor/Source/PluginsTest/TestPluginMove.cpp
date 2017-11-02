#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


#include "../Utils/TwoScenesFixture.h"


// ***********************
// Moves plugin in the same node (changes plugins order).
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Plugin_Move, MoveInNode )
{
    auto editor = GetProjectEditor();
    auto scene = CreateOneSceneWithColoredRect( editor, "FirstScene" );

    ASSERT_TRUE( editor->MovePlugin( scene->GetName(), "root/ColoredRect", 0, scene->GetName(), "root/ColoredRect", "solid color" ) );

    auto node = editor->GetNode( scene->GetName(), "root/ColoredRect" );
    auto plugin = node->GetPluginList()->GetPlugin( 0 );

    EXPECT_EQ( plugin->GetName(), "solid color" );
}


// ***********************
// Moves plugin between two different node.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Plugin_Move, MoveBetweenNodes )
{
    auto editor = GetProjectEditor();
    auto scene = CreateOneSceneWithColoredRect( editor, "FirstScene" );

    ASSERT_TRUE( editor->MovePlugin( scene->GetName(), "root/Group1", 1, scene->GetName(), "root/ColoredRect", "solid color" ) );

    auto node = editor->GetNode( scene->GetName(), "root/Group1" );
    EXPECT_TRUE( node->GetPlugin( "solid color" ) );
}

