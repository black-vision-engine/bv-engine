#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"



// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node_Move, MoveNodeInScene )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateTwoScenes( editor );

    auto scene1Name = editor->GetModelScene( "FirstScene" )->GetName();

    auto g1Child1 = editor->GetNode( scene1Name, "root/Group1/Child1" );
    auto g1Child2 = editor->GetNode( scene1Name, "root/Group1/Child2" );
    auto g1Child3 = editor->GetNode( scene1Name, "root/Group1/Child3" );
    auto g1Child4 = editor->GetNode( scene1Name, "root/Group1/Child4" );

    auto g2Child1 = editor->GetNode( scene1Name, "root/Group2/Child5" );
    auto g2Child2 = editor->GetNode( scene1Name, "root/Group2/Child6" );
    auto g2Child3 = editor->GetNode( scene1Name, "root/Group2/Child7" );
    auto g2Child4 = editor->GetNode( scene1Name, "root/Group2/Child8" );

    EXPECT_TRUE( g1Child1 != nullptr );
    EXPECT_TRUE( g1Child2 != nullptr );
    EXPECT_TRUE( g1Child3 != nullptr );
    EXPECT_TRUE( g1Child4 != nullptr );

    EXPECT_TRUE( g2Child1 != nullptr );
    EXPECT_TRUE( g2Child2 != nullptr );
    EXPECT_TRUE( g2Child3 != nullptr );
    EXPECT_TRUE( g2Child4 != nullptr );

    // ========================================================================= //
    // Move nodes of the same parent
    // ========================================================================= //

    bool result = editor->MoveNode( scene1Name, "root/Group1", 0, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    auto group1 = std::static_pointer_cast< bv::model::BasicNode >( editor->GetNode( scene1Name, "root/Group1" ) );
    ASSERT_TRUE( group1 != nullptr );

    EXPECT_TRUE( group1->GetChild( 0 ) == g1Child4 );
    EXPECT_TRUE( group1->GetChild( 1 ) == g1Child1 );
    EXPECT_TRUE( group1->GetChild( 2 ) == g1Child2 );
    EXPECT_TRUE( group1->GetChild( 3 ) == g1Child3 );

    result = editor->MoveNode( scene1Name, "root/Group1", 1, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    EXPECT_TRUE( group1->GetChild( 0 ) == g1Child1 );
    EXPECT_TRUE( group1->GetChild( 1 ) == g1Child4 );
    EXPECT_TRUE( group1->GetChild( 2 ) == g1Child2 );
    EXPECT_TRUE( group1->GetChild( 3 ) == g1Child3 );

    result = editor->MoveNode( scene1Name, "root/Group1", 2, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    EXPECT_TRUE( group1->GetChild( 0 ) == g1Child1 );
    EXPECT_TRUE( group1->GetChild( 1 ) == g1Child2 );
    EXPECT_TRUE( group1->GetChild( 2 ) == g1Child4 );
    EXPECT_TRUE( group1->GetChild( 3 ) == g1Child3 );

    result = editor->MoveNode( scene1Name, "root/Group1", 3, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    EXPECT_TRUE( group1->GetChild( 0 ) == g1Child1 );
    EXPECT_TRUE( group1->GetChild( 1 ) == g1Child2 );
    EXPECT_TRUE( group1->GetChild( 2 ) == g1Child3 );
    EXPECT_TRUE( group1->GetChild( 3 ) == g1Child4 );

    // ========================================================================= //
    // Add node after end of children vector
    // ========================================================================= //

    // Reset to index 0
    result = editor->MoveNode( scene1Name, "root/Group1", 0, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );
    // Set index after last node.
    result = editor->MoveNode( scene1Name, "root/Group1", 4, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    EXPECT_TRUE( group1->GetChild( 0 ) == g1Child1 );
    EXPECT_TRUE( group1->GetChild( 1 ) == g1Child2 );
    EXPECT_TRUE( group1->GetChild( 2 ) == g1Child3 );
    EXPECT_TRUE( group1->GetChild( 3 ) == g1Child4 );


    // ========================================================================= //
    // Node can't be moved as child of itself
    // ========================================================================= //

    result = editor->MoveNode( scene1Name, "root/Group1/Child4", 0, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( !result );
}

