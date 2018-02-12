#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/SVG/SVGAssetDescriptor.h"

#include "Utils/Scenes/TestScenesCreator.h"
#include "Utils/Nodes/TestNodesCreator.h"

#include "TestHelpers/ParametersSets.h"

#include "Assets/DefaultAssets.h"


// ========================================================================= //
// Test for loading textures. Tests use Default texture plugin but loading
// implementation from TexturePluginBase is tested in reality.
// ========================================================================= //

using namespace bv;


const char* imagePath_32x32 = "TestAssets/BVProjectEditor/checkerboard2_32x32.png";
const char* imagePath_checkerboard2 = "TestAssets/Common/checkerboard2.png";;


// ***********************
// Test checks normal valid exisitng texture loading scenario.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Plugins_TexturePlugins, ResourceLoading_ValidTexture )
{
    auto texturePlugin = TestScenesCreator::TexturedRectangle( GetProjectEditor(), "Scene", 400, 400, imagePath_32x32 );
    auto desc = TextureAssetDesc::Create( imagePath_checkerboard2, false );

    ASSERT_NE( texturePlugin, nullptr );
    EXPECT_TRUE( texturePlugin->LoadResource( desc ) );

    auto lassets = texturePlugin->GetLAssets();
    ASSERT_TRUE( lassets.size() == 1 );

    auto texDesc = std::static_pointer_cast< const TextureAssetDesc >( lassets[ 0 ].assetDesc );

    EXPECT_EQ( texDesc->GetOrigTextureDesc()->GetImagePath(), desc->GetOrigTextureDesc()->GetImagePath() );
}



// ***********************
// Providing path to non existing file should load fallback texture.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Plugins_TexturePlugins, ResourceLoading_FallbackTexture )
{
    auto texturePlugin = TestScenesCreator::TexturedRectangle( GetProjectEditor(), "Scene", 400, 400, imagePath_32x32 );
    auto desc = TextureAssetDesc::Create( "Plugins/TestTexturePlugins/NonExistingTexturePath.jpg", false );

    ASSERT_NE( texturePlugin, nullptr );

    // Loading fallback texture result in false return value.
    EXPECT_FALSE( texturePlugin->LoadResource( desc ) );

    auto lassets = texturePlugin->GetLAssets();
    ASSERT_TRUE( lassets.size() == 1 );

    auto texDesc = std::static_pointer_cast< const TextureAssetDesc >( lassets[ 0 ].assetDesc );

    EXPECT_NE( texDesc->GetOrigTextureDesc()->GetImagePath(), "Plugins/TestTexturePlugins/NonExistingTexturePath.jpg" );
    EXPECT_NE( texDesc->GetOrigTextureDesc()->GetImagePath(), imagePath_32x32 );

    auto fallbackDesc = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();

    EXPECT_EQ( texDesc->GetOrigTextureDesc()->GetImagePath(), fallbackDesc->GetOrigTextureDesc()->GetImagePath() );
}


// ***********************
// Sampler parameters set for texture doesn't change when new texture is loaded.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Plugins_TexturePlugins, ResourceLoading_SamplerParamsRemainAfterLoad )
{
    auto texturePlugin = TestScenesCreator::TexturedRectangle( GetProjectEditor(), "Scene", 400, 400, imagePath_32x32 );

    ParametersSets::SetSamplerStateParamsTestSet1( texturePlugin->GetResourceStateModel( "Tex0" ) );

    auto texDesc = TextureAssetDesc::Create( imagePath_checkerboard2, false );
    ASSERT_TRUE( texturePlugin->LoadResource( texDesc ) );

    EXPECT_TRUE( ParametersSets::CompareToSamplerStateParamsTestSet1( texturePlugin->GetResourceStateModel( "Tex0" ) ) );
}

// ***********************
// If someone provide descriptor with type not accepted by texture plugins, LoadResource should return
// and texture state shouldn't change.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Plugins_TexturePlugins, ResourceLoading_WrongDescriptorType )
{
    auto texturePlugin = TestScenesCreator::TexturedRectangle( GetProjectEditor(), "Scene", 400, 400, imagePath_32x32 );

    ParametersSets::SetSamplerStateParamsTestSet1( texturePlugin->GetResourceStateModel( "Tex0" ) );

    auto svgDesc = SVGAssetDescriptor::Create( "SomeSVGAsset.svg" );
    ASSERT_FALSE( texturePlugin->LoadResource( svgDesc ) );

    // Check number of assets.
    auto lassets = texturePlugin->GetLAssets();
    ASSERT_TRUE( lassets.size() == 1 );

    auto texDesc = std::static_pointer_cast< const TextureAssetDesc >( lassets[ 0 ].assetDesc );

    EXPECT_TRUE( ParametersSets::CompareToSamplerStateParamsTestSet1( texturePlugin->GetResourceStateModel( "Tex0" ) ) );
    EXPECT_EQ( texDesc->GetOrigTextureDesc()->GetImagePath(), imagePath_32x32 );
}

// ***********************
// TexturePlugin can load input slots as textures.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Plugins_TexturePlugins, ResourceLoading_TextureInputSlot )
{
    auto texturePlugin = TestScenesCreator::TexturedRectangle( GetProjectEditor(), "Scene", 400, 400, imagePath_32x32 );
    auto inputSlotDesc = TextureInputAssetDesc::Create( InputSlotBinding( 0 ) );

    ASSERT_TRUE( texturePlugin->LoadResource( inputSlotDesc ) );

    auto lassets = texturePlugin->GetLAssets();
    ASSERT_TRUE( lassets.size() == 1 );

    EXPECT_EQ( lassets[ 0 ].assetDesc->GetUID(), TextureInputAssetDesc::UID() );
    
    auto slotDesc = std::static_pointer_cast< const TextureInputAssetDesc >( lassets[ 0 ].assetDesc );
    
    EXPECT_EQ( slotDesc->BindingInfo().GetIndex(), 0 );
}



