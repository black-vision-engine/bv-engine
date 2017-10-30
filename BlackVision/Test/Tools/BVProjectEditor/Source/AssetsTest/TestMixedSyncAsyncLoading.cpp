#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"


using namespace bv;




// ***********************
// Test loads asset to 2 plugins at the same time asynchronously and synchronously.
// Sync load should hang and wait for async load.
class MixedSyncAsyncLoadingTest : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO( MixedSyncAsyncLoadingTest )
public:

    virtual void        PreEvents           () override;
    virtual void        PreModelUpdate      () override;

private:

    AssetDescConstPtr       m_assetDesc;
    int                     m_numLoaded;
};
REGISTER_FRAMEWORK_GTEST_INFO( MixedSyncAsyncLoadingTest, BVProjectEditor_Assets_Loading, MixedSyncAsyncLoadingTest )



// ========================================================================= //
// Implementation
// ========================================================================= //


extern const char* imagePath_32x32;     // In file SyncLoadingTest


// ***********************
//
void        MixedSyncAsyncLoadingTest::PreEvents           ()
{
    // Create scene
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto firstNode = editor->GetNode( "FirstScene", "root/Group1" );
    auto secondNode = editor->GetNode( "FirstScene", "root/Group2" );

    UInt32 idx = 1;
    ASSERT_TRUE( editor->AddPlugin( "FirstScene", "root/Group1", "DEFAULT_TEXTURE", "texture", "default", idx ) );
    ASSERT_TRUE( editor->AddPlugin( "FirstScene", "root/Group2", "DEFAULT_TEXTURE", "texture", "default", idx ) );

    m_assetDesc = TextureAssetDesc::Create( imagePath_32x32, true );

    // Loading assets first asynchronously and second synchronosuly. BV should wait here for first asset to be loaded.
    ASSERT_TRUE( editor->LoadAssetAsync( "FirstScene", "root/Group1", "texture", m_assetDesc ) );
    ASSERT_TRUE( editor->LoadAsset( "FirstScene", "root/Group2", "texture", m_assetDesc ) );

    // Check if assets were loaded corectly.

    auto texPlugin2 = std::static_pointer_cast< model::DefaultTexturePlugin >( secondNode->GetPlugin( "texture" ) );
    auto lassets2 = texPlugin2->GetLAssets();
    
    ASSERT_TRUE( lassets2.size() == 1 );

    // Note that we don't expect that async asset is already loaded. Event will come in next frame.
    EXPECT_EQ( lassets2[ 0 ].assetDesc, m_assetDesc );
}


// ***********************
//
void        MixedSyncAsyncLoadingTest::PreModelUpdate       ()
{
    // Note that we are after first EventManager update, so this function can be treated as "next frame".
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();

    auto firstNode = editor->GetNode( "FirstScene", "root/Group2" );

    auto texPlugin1 = std::static_pointer_cast< model::DefaultTexturePlugin >( firstNode->GetPlugin( "texture" ) );
    auto lassets1 = texPlugin1->GetLAssets();

    ASSERT_EQ( lassets1.size(), 1 );
    EXPECT_EQ( lassets1[ 0 ].assetDesc, m_assetDesc );
}


