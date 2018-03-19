#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/IDGenerator.h"


using namespace bv;


// ***********************
// # is forbidden character in name, since it's used in indexed path.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, Rename_HashInName )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto node = editor->GetNode( "FirstScene", "root/Group1" );

    EXPECT_FALSE( editor->RenameNode( node, "bla#bla" ) );
}

// ***********************
// @ is forbidden sign in name, since it's used in uid path.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, Rename_UIDPrefixInName )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto node = editor->GetNode( "FirstScene", "root/Group1" );

    EXPECT_FALSE( editor->RenameNode( node, "bla@bla" ) );
}

