#pragma once

#include "gtest/gtest.h"

#include "ProjectEditorUtils.h"


class TwoScenesTest : public ::testing::Test
{
protected:
    virtual void        SetUp       ()
    {
        m_editor = m_context.Editor();

        m_editor->AddScene( "FirstScene" );
        m_editor->AddScene( "SecondScene" );

        auto scene1 = m_editor->GetModelScene( "FirstScene" );
        auto scene2 = m_editor->GetModelScene( "SecondScene" );

        ASSERT_NE( scene1, nullptr );
        ASSERT_NE( scene2, nullptr );

        VerifyDefaultScene( scene1 );
        VerifyDefaultScene( scene2 );

        AddHierarchy( scene1 );
        AddHierarchy( scene2 );
    }

    virtual void        TearDown    () {}


private:

    bv::ProjectEditorContext        m_context;

protected:

    bv::BVProjectEditor*            m_editor;

private:

    inline void         VerifyDefaultScene  ( bv::model::SceneModelPtr scene );
    inline void         AddHierarchy        ( bv::model::SceneModelPtr scene );
};


// ***********************
//
inline void             TwoScenesTest::VerifyDefaultScene       ( bv::model::SceneModelPtr scene )
{
    auto root = scene->GetRootNode();
    ASSERT_NE( root, nullptr );

    auto transformPlugin = root->GetPlugin( "transform" );
    ASSERT_NE( transformPlugin, nullptr );

}

// ***********************
//
inline void             TwoScenesTest::AddHierarchy             ( bv::model::SceneModelPtr scene )
{
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "root", "Group1" ) );
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "root", "Group2" ) );

    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group1", "Child1" ) );
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group1", "Child2" ) );
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group1", "Child3" ) );
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group1", "Child4" ) );

    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group2", "Child1" ) );
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group2", "Child2" ) );
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group2", "Child3" ) );
    ASSERT_TRUE( m_editor->AddChildNode( scene->GetName(), "Group2", "Child4" ) );

    auto root = scene->GetRootNode();
    ASSERT_NE( root, nullptr );

    ASSERT_NE( root->GetChild( "Group1" ), nullptr );
    ASSERT_NE( root->GetChild( "Group2" ), nullptr );

    auto group1 = root->GetChild( "Group1" );

    ASSERT_NE( group1->GetChild( "Child1" ), nullptr );
    ASSERT_NE( group1->GetChild( "Child2" ), nullptr );
    ASSERT_NE( group1->GetChild( "Child3" ), nullptr );
    ASSERT_NE( group1->GetChild( "Child4" ), nullptr );

    auto group2 = root->GetChild( "Group2" );

    ASSERT_NE( group2->GetChild( "Child1" ), nullptr );
    ASSERT_NE( group2->GetChild( "Child2" ), nullptr );
    ASSERT_NE( group2->GetChild( "Child3" ), nullptr );
    ASSERT_NE( group2->GetChild( "Child4" ), nullptr );
}
