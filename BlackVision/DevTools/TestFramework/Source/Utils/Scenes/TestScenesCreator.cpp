#include "TestScenesCreator.h"


#include "Utils/Nodes/TestNodesCreator.h"



namespace bv
{


// ***********************
//
model::DefaultTexturePluginPtr          TestScenesCreator::TexturedRectangle        ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height )
{
    return TexturedRectangle( editor, sceneName, width, height, "" );
}

// ***********************
//
model::DefaultTexturePluginPtr          TestScenesCreator::TexturedRectangle        ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height, const std::string & texPath )
{
    auto scene = CreateEmptyScene( editor, sceneName );
    auto texturedNode = TestNodesCreator::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "texturedNode", width, height, texPath );

    editor->AddChildNode( scene, scene->GetRootNode(), texturedNode );

    return std::static_pointer_cast< model::DefaultTexturePlugin >( texturedNode->GetPlugin( "texture" ) );
}

// ***********************
//
model::VideoInputPluginPtr              TestScenesCreator::VideoInputRectangle      ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height )
{
    return VideoInputRectangle( editor, sceneName, width, height, 0 );
}

// ***********************
//
model::VideoInputPluginPtr              TestScenesCreator::VideoInputRectangle      ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height, UInt32 inputIdx )
{
    auto scene = CreateEmptyScene( editor, sceneName );
    auto videoNode = TestNodesCreator::VideoInputRectangle( editor->GetSceneDefaultTimeline( scene ), "videoNode", width, height, inputIdx );

    editor->AddChildNode( scene, scene->GetRootNode(), videoNode );

    return std::static_pointer_cast< model::VideoInputPlugin >( videoNode->GetPlugin( "video input" ) );
}

// ***********************
//
nodelogic::FollowPtr                    TestScenesCreator::RectFollowsRect          ( BVProjectEditor * editor, const std::string & sceneName, const std::string & followedNode, const std::string & followingNode )
{
    auto scene = TestScenesCreator::CreateEmptyScene( editor, sceneName );

    auto root = scene->GetRootNode();
    auto followed = TestNodesCreator::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), followedNode, 1.0f, 1.0f, glm::vec4( 1.0, 0.0, 1.0, 1.0 ) );
    auto following = TestNodesCreator::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), followingNode, 1.0f, 1.0f, glm::vec4( 0.0, 1.0, 1.0, 1.0 ) );

    auto newLogic = std::make_shared< nodelogic::Follow >( following, editor->GetSceneDefaultTimeline( scene ) );
    editor->SetLogic( following, newLogic );

    editor->AddChildNode( scene, root, followed );
    editor->AddChildNode( scene, root, following );

    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::FOLLOW_X ), 0.0f, true );
    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::FOLLOW_Y ), 0.0f, true );
    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::FOLLOW_Z ), 0.0f, true );

    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::OFFSET_X ), 0.0f, 1.0f );
    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::OFFSET_Y ), 0.0f, 1.0f );
    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::OFFSET_Z ), 0.0f, 1.0f );

    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::ALIGN_X ), 0.0f, nodelogic::Follow::BBAlignementX::CenterX );
    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::ALIGN_Y ), 0.0f, nodelogic::Follow::BBAlignementY::CenterY );
    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::ALIGN_Z ), 0.0f, nodelogic::Follow::BBAlignementZ::CenterZ );

    model::SetParameter( newLogic->GetParameter( nodelogic::Follow::PARAMETERS::FOLLOWING_MODE ), 0.0f, nodelogic::Follow::FollowingMode::Previous );

    return newLogic;
}

// ***********************
//
model::SceneModelPtr                    TestScenesCreator::CreateEmptyScene         ( BVProjectEditor * editor, const std::string & sceneName )
{
    editor->AddScene( sceneName );
    return editor->GetModelScene( sceneName );
}


}	// bv


