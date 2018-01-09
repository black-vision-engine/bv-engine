#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Editors/BVProjectEditor.h"

#include"Assets/Texture/TextureAssetDescriptor.h"

#include "Utils/Scenes/TestScenesCreator.h"


// ========================================================================= //
// Test for loading textures. Tests use Default texture plugin but loading
// implementation from TexturePluginBase is tested in reality.
// ========================================================================= //

using namespace bv;


// ***********************
// Note: Fallback texture loading test is placed in TestBVProjectEditor/AssetsTest



//// ***********************
//// Providing path to non existing file should load fallback texture.
//SIMPLE_FRAMEWORK_TEST_IN_SUITE( Plugins_TexturePlugins, ResourceLoading_TextureDescriptor_InvalidPath )
//{
//    auto texturePlugin = TestScenesCreator::TexturedRectangle( GetProjectEditor(), "Scene", 400, 400 );
//
//    auto texDesc = TextureAssetDesc::Create( "Plugins/TestTexturePlugins/NonExistingTexturePath.jpg", false );
//
//
//}





