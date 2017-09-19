#pragma once

#include "UnitTest++.h"
#include "Engine/Editors/BVProjectEditor.h"

#include "Utils/Nodes/TestNodesCreator.h"



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

    REQUIRE( scene1 != nullptr );

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

    REQUIRE( scene1 != nullptr );
    REQUIRE( scene2 != nullptr );

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
    REQUIRE( root != nullptr );

    auto transformPlugin = root->GetPlugin( "transform" );
    REQUIRE( transformPlugin != nullptr );

}

// ***********************
//
inline void             AddHierarchy             ( bv::model::SceneModelPtr scene, bv::BVProjectEditor * editor )
{
    REQUIRE( editor->AddChildNode( scene->GetName(), "root", "Group1" ) );
    REQUIRE( editor->AddChildNode( scene->GetName(), "root", "Group2" ) );

    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child1" ) );
    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child2" ) );
    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child3" ) );
    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group1", "Child4" ) );

    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child1" ) );
    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child2" ) );
    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child3" ) );
    REQUIRE( editor->AddChildNode( scene->GetName(), "root/Group2", "Child4" ) );

    auto root = scene->GetRootNode();
    REQUIRE( root, nullptr );

    REQUIRE( root->GetChild( "Group1" ) != nullptr );
    REQUIRE( root->GetChild( "Group2" ) != nullptr );

    auto group1 = root->GetChild( "Group1" );

    REQUIRE( group1->GetChild( "Child1" ) != nullptr );
    REQUIRE( group1->GetChild( "Child2" ) != nullptr );
    REQUIRE( group1->GetChild( "Child3" ) != nullptr );
    REQUIRE( group1->GetChild( "Child4" ) != nullptr );

    auto group2 = root->GetChild( "Group2" );

    REQUIRE( group2->GetChild( "Child1" ) != nullptr );
    REQUIRE( group2->GetChild( "Child2" ) != nullptr );
    REQUIRE( group2->GetChild( "Child3" ) != nullptr );
    REQUIRE( group2->GetChild( "Child4" ) != nullptr );
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
