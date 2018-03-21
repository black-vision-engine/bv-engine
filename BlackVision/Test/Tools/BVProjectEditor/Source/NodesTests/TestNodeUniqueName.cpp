#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/IDGenerator.h"


using namespace bv;



// ========================================================================= //
// Unique name Tests
// ========================================================================= //


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UniqueName_AddNodes )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto node = std::static_pointer_cast< model::BasicNode >( editor->GetNode( "FirstScene", "root/Group1" ) );

    ASSERT_TRUE( editor->AddChildNode( "FirstScene", "root/Group1", "Child1" ) );
    ASSERT_TRUE( editor->AddChildNode( "FirstScene", "root/Group1", "Child1" ) );

    ASSERT_EQ( node->GetNumChildren(), 6 );

    EXPECT_EQ( node->GetChild( 4 )->GetName(), "Child1(1)" );
    EXPECT_EQ( node->GetChild( 5 )->GetName(), "Child1(2)" );
}

