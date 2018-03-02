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
    editor->AddScene( sceneName );

    auto scene = editor->GetModelScene( sceneName );
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
    editor->AddScene( sceneName );

    auto scene = editor->GetModelScene( sceneName );
    auto videoNode = TestNodesCreator::VideoInputRectangle( editor->GetSceneDefaultTimeline( scene ), "videoNode", width, height, inputIdx );

    editor->AddChildNode( scene, scene->GetRootNode(), videoNode );

    return std::static_pointer_cast< model::VideoInputPlugin >( videoNode->GetPlugin( "video input" ) );
}


}	// bv


