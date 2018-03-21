#pragma once

#include "gtest/gtest.h"
#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Utils/Nodes/TestNodesCreator.h"

#include "Engine/Models/NodeLogics/Follow/Follow.h"



inline void         CreateOneScene      ( bv::BVProjectEditor * editor );
inline void         CreateTwoScenes     ( bv::BVProjectEditor * editor );

inline void         VerifyDefaultScene  ( bv::model::SceneModelPtr scene, bv::BVProjectEditor * editor );
inline void         AddHierarchy        ( bv::model::SceneModelPtr scene, bv::BVProjectEditor * editor );


inline bv::model::SceneModelPtr         CreateOneSceneWithColoredRect       ( bv::BVProjectEditor * editor, const std::string & sceneName );


// ***********************
//
inline void             CreateOneScene          ( bv::BVProjectEditor * editor )
{
    editor->AddScene( "FirstScene" );

    auto scene1 = editor->GetModelScene( "FirstScene" );

    ASSERT_TRUE( scene1 != nullptr );

    VerifyDefaultScene( scene1, editor );
    AddHierarchy( scene1, editor );
}

// ***********************
//
inline void             CreateTwoScenes         ( bv::BVProjectEditor * editor )
{
    editor->AddScene( "FirstScene" );
    editor->AddScene( "SecondScene" );

    auto scene1 = editor->GetModelScene( "FirstScene" );
    auto scene2 = editor->GetModelScene( "SecondScene" );

    ASSERT_TRUE( scene1 != nullptr );
    ASSERT_TRUE( scene2 != nullptr );

    VerifyDefaultScene( scene1, editor );
    VerifyDefaultScene( scene2, editor );

    AddHierarchy( scene1, editor );
    AddHierarchy( scene2, editor );
}

// ***********************
//
inline void             VerifyDefaultScene       ( bv::model::SceneModelPtr scene, bv::BVProjectEditor * /*editor*/ )
{
    auto root = scene->GetRootNode();
    ASSERT_TRUE( root != nullptr );

    auto transformPlugin = root->GetPlugin( "transform" );
    ASSERT_TRUE( transformPlugin != nullptr );

}

// ***********************
//
inline void             AddHierarchy             ( bv::model::SceneModelPtr scene, bv::BVProjectEditor * editor )
{
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root", "Group1" ) );
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root", "Group2" ) );

    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child1" ) );
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child2" ) );
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child3" ) );
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child4" ) );

    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child5" ) );
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child6" ) );
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child7" ) );
    ASSERT_TRUE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child8" ) );

    auto root = scene->GetRootNode();
    ASSERT_NE( root, nullptr );

    ASSERT_TRUE( root->GetChild( "Group1" ) != nullptr );
    ASSERT_TRUE( root->GetChild( "Group2" ) != nullptr );

    auto group1 = root->GetChild( "Group1" );

    ASSERT_TRUE( group1->GetChild( "Child1" ) != nullptr );
    ASSERT_TRUE( group1->GetChild( "Child2" ) != nullptr );
    ASSERT_TRUE( group1->GetChild( "Child3" ) != nullptr );
    ASSERT_TRUE( group1->GetChild( "Child4" ) != nullptr );

    auto group2 = root->GetChild( "Group2" );

    ASSERT_TRUE( group2->GetChild( "Child5" ) != nullptr );
    ASSERT_TRUE( group2->GetChild( "Child6" ) != nullptr );
    ASSERT_TRUE( group2->GetChild( "Child7" ) != nullptr );
    ASSERT_TRUE( group2->GetChild( "Child8" ) != nullptr );
}

// ***********************
//
inline bv::model::SceneModelPtr     CreateOneSceneWithColoredRect       ( bv::BVProjectEditor * editor, const std::string & sceneName )
{
    editor->AddScene( sceneName );

    auto scene = editor->GetModelScene( sceneName );
    auto timeline = editor->GetSceneDefaultTimeline( scene );

    auto node = bv::TestNodesCreator::ColoredRectangle( timeline, "ColoredRect", 10, 10, glm::vec4( 0.5f, 1.0, 1.0, 1.0 ) );
    
    editor->AddChildNode( scene, scene->GetRootNode(), node );
    editor->AddChildNode( scene->GetName(), "root", "Group1" );

    return editor->GetModelScene( sceneName );
}

// ***********************
//
inline bv::model::SceneModelPtr     CreateSceneForParamDesc             ( bv::BVProjectEditor * editor, const std::string & sceneName )
{
    editor->AddScene( sceneName );

    auto scene = editor->GetModelScene( sceneName );
    auto timeline = editor->GetSceneDefaultTimeline( scene );

    editor->AddLight( scene, bv::LightType::LT_POINT, timeline );
    editor->AddLight( scene, bv::LightType::LT_DIRECTIONAL, timeline );

    editor->AddCamera( scene );

    auto node1 = bv::TestNodesCreator::ColoredRectangle( timeline, "ColoredRect", 10, 10, glm::vec4( 0.5f, 1.0, 1.0, 1.0 ) );
    auto node2 = bv::TestNodesCreator::TexturedRectangle( timeline, "TexturedRect", 100, 100, "TestAssets/Common/checkerboard2_32X32.png" );

    editor->AddChildNode( scene, scene->GetRootNode(), node1 );
    editor->AddChildNode( scene, scene->GetRootNode(), node2 );
    editor->AddChildNode( scene->GetName(), "root", "Group1" );

    auto logic = std::make_shared< bv::nodelogic::Follow >( node2, timeline );
    editor->SetLogic( node2, logic );

    editor->SetNodeEffect( scene->GetName(), "root/ColoredRect", sceneName, "alpha mask" );

    return editor->GetModelScene( sceneName );
}