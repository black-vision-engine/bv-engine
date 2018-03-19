#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/IDGenerator.h"


using namespace bv;

// ***********************
// Checks if UIDs are generated properly and if they are really unique.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_Generation )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    std::set< UniqueID > ids;

    auto scene = editor->GetModelScene( "FirstScene" );
    auto root = scene->GetRootNode();

    auto group1 = root->GetChild( 0 );
    auto group2 = root->GetChild( 1 );
    
    ids.insert( root->GetUID() );
    ids.insert( group1->GetUID() );
    ids.insert( group2->GetUID() );

    for( unsigned int i = 0; i < group1->GetNumChildren(); ++i )
    {
        ids.insert( group1->GetChild( i )->GetUID() );
    }

    for( unsigned int i = 0; i < group2->GetNumChildren(); ++i )
    {
        ids.insert( group2->GetChild( i )->GetUID() );
    }

    // Check uniqness of ids.
    EXPECT_EQ( ids.size(), 11 );

    // Check if all ids are below current Id value.
    auto curID = IDGenerator::Instance().GetID();
    bool allBelowCurrentID = true;
    for( auto iter = ids.begin(); iter != ids.end(); ++iter )
    {
        if( *iter >= curID )
            allBelowCurrentID = false;
    }

    EXPECT_TRUE( allBelowCurrentID );
}


// ***********************
// Find node by using UID. Node is direct child of root node, where GetNode is called.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_GetDirectChild )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto root = editor->GetNode( "FirstScene", "root" );
    auto node = editor->GetNode( "FirstScene", "root/Group1" );

    auto searchedUID = node->GetUID();

    auto foundNode = root->GetNode( searchedUID );
    EXPECT_EQ( foundNode, node );
}

// ***********************
// Find direct child with recursive flag set to false. GetNode shoudl return valid node.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_GetDirectChild_NotRecursive )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto root = editor->GetNode( "FirstScene", "root" );
    auto node = editor->GetNode( "FirstScene", "root/Group1" );

    auto searchedUID = node->GetUID();

    auto foundNode = root->GetNode( searchedUID, false );
    EXPECT_EQ( foundNode, node );
}

// ***********************
// Find node by using UID. Node is not direct child of root node, where GetNode is called.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_GetNestedChild )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto root = editor->GetNode( "FirstScene", "root" );
    auto node = editor->GetNode( "FirstScene", "root/Group1/Child3" );

    auto searchedUID = node->GetUID();

    auto foundNode = root->GetNode( searchedUID );
    EXPECT_EQ( foundNode, node );
}

// ***********************
// Try to find nested child with recursive flag set to false. GetNode should return nullptr.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_GetNestedChild_NotRecursive )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto root = editor->GetNode( "FirstScene", "root" );
    auto node = editor->GetNode( "FirstScene", "root/Group1/Child3" );

    auto searchedUID = node->GetUID();

    auto foundNode = root->GetNode( searchedUID, false );
    EXPECT_EQ( foundNode, nullptr );
}

// ***********************
// Call GetNode with UID of node, which is called. Node should return pointer to itself.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_GetNode_SelfUID )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto root = editor->GetNode( "FirstScene", "root" );

    auto searchedUID = root->GetUID();

    auto foundNode = root->GetNode( searchedUID, false );
    EXPECT_EQ( foundNode, root );
}


// ========================================================================= //
// GetNode using uid in path
// ========================================================================= //

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_GetNode_PathWithUID )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto node = editor->GetNode( "FirstScene", "root/Group1/Child2" );
    auto searchedUID = node->GetUID();

    std::string uidPath = "@" + Convert::T2String( searchedUID );
    auto foundNode = editor->GetNode( "FirstScene", uidPath );

    EXPECT_EQ( foundNode, node );
}

// ***********************
// GetNode should return nullptr if we will pass not existing id.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node, UIDs_GetNode_InvalidUID )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    std::string uidPath = "@" + Convert::T2String( IDGenerator::Instance().GetID() );
    auto foundNode = editor->GetNode( "FirstScene", uidPath );

    EXPECT_EQ( foundNode, nullptr );
}


