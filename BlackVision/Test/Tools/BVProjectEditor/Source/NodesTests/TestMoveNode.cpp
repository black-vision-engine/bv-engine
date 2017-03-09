#include "gtest/gtest.h"

#include "Utils/TwoScenesFixture.h"



TEST_F( TwoScenesTest, MoveNodeInScene )
{
    auto scene1Name = m_editor->GetModelScene( "FirstScene" )->GetName();

    auto g1Child1 = m_editor->GetNode( scene1Name, "root/Group1/Child1" );
    auto g1Child2 = m_editor->GetNode( scene1Name, "root/Group1/Child2" );
    auto g1Child3 = m_editor->GetNode( scene1Name, "root/Group1/Child3" );
    auto g1Child4 = m_editor->GetNode( scene1Name, "root/Group1/Child4" );

    auto g2Child1 = m_editor->GetNode( scene1Name, "root/Group2/Child1" );
    auto g2Child2 = m_editor->GetNode( scene1Name, "root/Group2/Child2" );
    auto g2Child3 = m_editor->GetNode( scene1Name, "root/Group2/Child3" );
    auto g2Child4 = m_editor->GetNode( scene1Name, "root/Group2/Child4" );

    ASSERT_NE( g1Child1, nullptr );
    ASSERT_NE( g1Child2, nullptr );
    ASSERT_NE( g1Child3, nullptr );
    ASSERT_NE( g1Child4, nullptr );

    ASSERT_NE( g2Child1, nullptr );
    ASSERT_NE( g2Child2, nullptr );
    ASSERT_NE( g2Child3, nullptr );
    ASSERT_NE( g2Child4, nullptr );

// ========================================================================= //
// Move nodes of the same parent
// ========================================================================= //

    bool result = m_editor->MoveNode( scene1Name, "root/Group1", 0, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    auto group1 = std::static_pointer_cast< bv::model::BasicNode >( m_editor->GetNode( scene1Name, "root/Group1" ) );
    ASSERT_NE( group1, nullptr );

    ASSERT_EQ( group1->GetChild( 0 ), g1Child4 );
    ASSERT_EQ( group1->GetChild( 1 ), g1Child1 );
    ASSERT_EQ( group1->GetChild( 2 ), g1Child2 );
    ASSERT_EQ( group1->GetChild( 3 ), g1Child3 );

    result = m_editor->MoveNode( scene1Name, "root/Group1", 1, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    ASSERT_EQ( group1->GetChild( 0 ), g1Child1 );
    ASSERT_EQ( group1->GetChild( 1 ), g1Child4 );
    ASSERT_EQ( group1->GetChild( 2 ), g1Child2 );
    ASSERT_EQ( group1->GetChild( 3 ), g1Child3 );

    result = m_editor->MoveNode( scene1Name, "root/Group1", 2, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    ASSERT_EQ( group1->GetChild( 0 ), g1Child1 );
    ASSERT_EQ( group1->GetChild( 1 ), g1Child2 );
    ASSERT_EQ( group1->GetChild( 2 ), g1Child4 );
    ASSERT_EQ( group1->GetChild( 3 ), g1Child3 );

    result = m_editor->MoveNode( scene1Name, "root/Group1", 3, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    ASSERT_EQ( group1->GetChild( 0 ), g1Child1 );
    ASSERT_EQ( group1->GetChild( 1 ), g1Child2 );
    ASSERT_EQ( group1->GetChild( 2 ), g1Child3 );
    ASSERT_EQ( group1->GetChild( 3 ), g1Child4 );

// ========================================================================= //
// Add node after end of children vector
// ========================================================================= //

    // Reset to index 0
    result = m_editor->MoveNode( scene1Name, "root/Group1", 0, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );
    // Set index after last node.
    result = m_editor->MoveNode( scene1Name, "root/Group1", 4, scene1Name, "root/Group1/Child4" );
    ASSERT_TRUE( result );

    ASSERT_EQ( group1->GetChild( 0 ), g1Child1 );
    ASSERT_EQ( group1->GetChild( 1 ), g1Child2 );
    ASSERT_EQ( group1->GetChild( 2 ), g1Child3 );
    ASSERT_EQ( group1->GetChild( 3 ), g1Child4 );


// ========================================================================= //
// Node can't be moved as child of itself
// ========================================================================= //

    result = m_editor->MoveNode( scene1Name, "root/Group1/Child4", 0, scene1Name, "root/Group1/Child4" );
    ASSERT_FALSE( result );

}

