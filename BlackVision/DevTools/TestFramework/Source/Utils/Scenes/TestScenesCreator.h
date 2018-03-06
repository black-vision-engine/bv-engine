#pragma once

#include "Engine/Models/SceneModel.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/VideoInput/VideoInputPlugin.h"



namespace bv
{

class BVProjectEditor;




// ***********************
//
class TestScenesCreator
{
public:

    static model::DefaultTexturePluginPtr       TexturedRectangle       ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height );
    static model::DefaultTexturePluginPtr       TexturedRectangle       ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height, const std::string & texPath );

    static model::VideoInputPluginPtr           VideoInputRectangle     ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height );
    static model::VideoInputPluginPtr           VideoInputRectangle     ( BVProjectEditor * editor, const std::string & sceneName, Float32 width, Float32 height, UInt32 inputIdx );
};



}	// bv

