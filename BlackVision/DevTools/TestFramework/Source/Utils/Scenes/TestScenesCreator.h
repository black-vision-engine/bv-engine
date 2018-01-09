#pragma once

#include "Engine/Models/SceneModel.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"



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


};



}	// bv

