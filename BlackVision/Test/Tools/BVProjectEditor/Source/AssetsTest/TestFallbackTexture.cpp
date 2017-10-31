#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"

#include "Utils/Nodes/TestNodesCreator.h"
#include "Assets/DefaultAssets.h"



using namespace bv;


// ***********************
//
class FallbackTextureTest : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( FallbackTextureTest )
public:

    virtual void        PreEvents           () override;

private:

    AssetDescConstPtr       m_assetDesc;

};
REGISTER_FRAMEWORK_GTEST_INFO( FallbackTextureTest, BVProjectEditor_Assets_Loading, FallbackTextureTest )




extern const char* imagePath_32x32;     // In file SyncLoadingTest


// ***********************
//
void        FallbackTextureTest::PreEvents           ()
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    
    editor->AddScene( "TestScene" );
    auto scene = editor->GetModelScene( "TestScene" );

    auto node = TestNodesCreator::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "texNode", 100, 100, imagePath_32x32 );
    auto texturePlugin = std::static_pointer_cast< model::DefaultTexturePlugin >( node->GetPlugin( "texture" ) );
    ASSERT_TRUE( texturePlugin != nullptr );

    auto desc = TextureAssetDesc::Create( "nonExistingTexture.jpg", true );

    // Loading fallback texture result in false return value.
    EXPECT_TRUE( !texturePlugin->LoadResource( desc ) );

    auto lassets = texturePlugin->GetLAssets();
    ASSERT_TRUE( lassets.size() == 1 );

    auto texDesc = std::static_pointer_cast< const TextureAssetDesc >( lassets[ 0 ].assetDesc );
    
    EXPECT_NE( texDesc->GetOrigTextureDesc()->GetImagePath(), "nonExistingTexture.jpg" );
    EXPECT_NE( texDesc->GetOrigTextureDesc()->GetImagePath(), imagePath_32x32 );

    auto fallbackDesc = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();

    EXPECT_EQ( texDesc->GetOrigTextureDesc()->GetImagePath(), fallbackDesc->GetOrigTextureDesc()->GetImagePath() );
}
