#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"


// ***********************
//
class MoveNodeInScene : public bv::FrameworkTest
{
private:
public:
    MoveNodeInScene() : bv::FrameworkTest( "MoveNodeInScene", "BVProjectEditor.Node.Move", __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;
} MoveNodeInSceneInstance;

UnitTest::ListAdder adderTestTest ( UnitTest::Test::GetTestList(), &MoveNodeInSceneInstance );


// ***********************
//
void        MoveNodeInScene::PreEvents     ()
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateTwoScenes( editor );

    auto scene1Name = editor->GetModelScene( "FirstScene" )->GetName();

    auto g1Child1 = editor->GetNode( scene1Name, "root/Group1/Child1" );
    auto g1Child2 = editor->GetNode( scene1Name, "root/Group1/Child2" );
    auto g1Child3 = editor->GetNode( scene1Name, "root/Group1/Child3" );
    auto g1Child4 = editor->GetNode( scene1Name, "root/Group1/Child4" );

    auto g2Child1 = editor->GetNode( scene1Name, "root/Group2/Child1" );
    auto g2Child2 = editor->GetNode( scene1Name, "root/Group2/Child2" );
    auto g2Child3 = editor->GetNode( scene1Name, "root/Group2/Child3" );
    auto g2Child4 = editor->GetNode( scene1Name, "root/Group2/Child4" );

    CHECK( g1Child1 != nullptr );
    CHECK( g1Child2 != nullptr );
    CHECK( g1Child3 != nullptr );
    CHECK( g1Child4 != nullptr );

    CHECK( g2Child1 != nullptr );
    CHECK( g2Child2 != nullptr );
    CHECK( g2Child3 != nullptr );
    CHECK( g2Child4 != nullptr );

    // ========================================================================= //
    // Move nodes of the same parent
    // ========================================================================= //

    bool result = editor->MoveNode( scene1Name, "root/Group1", 0, scene1Name, "root/Group1/Child4" );
    REQUIRE( result );

    auto group1 = std::static_pointer_cast< bv::model::BasicNode >( editor->GetNode( scene1Name, "root/Group1" ) );
    REQUIRE( group1 != nullptr );

    CHECK( group1->GetChild( 0 ) == g1Child4 );
    CHECK( group1->GetChild( 1 ) == g1Child1 );
    CHECK( group1->GetChild( 2 ) == g1Child2 );
    CHECK( group1->GetChild( 3 ) == g1Child3 );

    result = editor->MoveNode( scene1Name, "root/Group1", 1, scene1Name, "root/Group1/Child4" );
    REQUIRE( result );

    CHECK( group1->GetChild( 0 ) == g1Child1 );
    CHECK( group1->GetChild( 1 ) == g1Child4 );
    CHECK( group1->GetChild( 2 ) == g1Child2 );
    CHECK( group1->GetChild( 3 ) == g1Child3 );

    result = editor->MoveNode( scene1Name, "root/Group1", 2, scene1Name, "root/Group1/Child4" );
    REQUIRE( result );

    CHECK( group1->GetChild( 0 ) == g1Child1 );
    CHECK( group1->GetChild( 1 ) == g1Child2 );
    CHECK( group1->GetChild( 2 ) == g1Child4 );
    CHECK( group1->GetChild( 3 ) == g1Child3 );

    result = editor->MoveNode( scene1Name, "root/Group1", 3, scene1Name, "root/Group1/Child4" );
    REQUIRE( result );

    CHECK( group1->GetChild( 0 ) == g1Child1 );
    CHECK( group1->GetChild( 1 ) == g1Child2 );
    CHECK( group1->GetChild( 2 ) == g1Child3 );
    CHECK( group1->GetChild( 3 ) == g1Child4 );

    // ========================================================================= //
    // Add node after end of children vector
    // ========================================================================= //

    // Reset to index 0
    result = editor->MoveNode( scene1Name, "root/Group1", 0, scene1Name, "root/Group1/Child4" );
    REQUIRE( result );
    // Set index after last node.
    result = editor->MoveNode( scene1Name, "root/Group1", 4, scene1Name, "root/Group1/Child4" );
    REQUIRE( result );

    CHECK( group1->GetChild( 0 ) == g1Child1 );
    CHECK( group1->GetChild( 1 ) == g1Child2 );
    CHECK( group1->GetChild( 2 ) == g1Child3 );
    CHECK( group1->GetChild( 3 ) == g1Child4 );


    // ========================================================================= //
    // Node can't be moved as child of itself
    // ========================================================================= //

    result = editor->MoveNode( scene1Name, "root/Group1/Child4", 0, scene1Name, "root/Group1/Child4" );
    REQUIRE( !result );
}

