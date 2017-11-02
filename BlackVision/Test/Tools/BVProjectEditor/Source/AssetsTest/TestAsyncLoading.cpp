#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Editors/AssetsLoading/Async/AssetAsyncLoadFinishedEvent.h"


using namespace bv;


// ***********************
//
class AsyncLoadingTest : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO( AsyncLoadingTest )

public:

    explicit AsyncLoadingTest()
        : m_numLoaded( 0 )
    {}

public:

    virtual void        PreEvents           () override;
    virtual void        PreModelUpdate      () override;

    void                AssetLoaded         ( IEventPtr );

private:

    AssetDescConstPtr       m_assetDesc;
    int                     m_numLoaded;
};
REGISTER_FRAMEWORK_GTEST_INFO( AsyncLoadingTest, BVProjectEditor_Assets_Loading, AsyncLoadingTest )



// ========================================================================= //
// Implementation
// ========================================================================= //


extern const char* imagePath_32x32;     // In file SyncLoadingTest


// ***********************
//
void        AsyncLoadingTest::PreEvents           ()
{
    if( GetFrameNumber() == 0 )
    {
        EndTestAfterThisFrame( false );

        // Create scene
        auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
        CreateOneScene( editor );

        auto firstNode = editor->GetNode( "FirstScene", "root/Group1" );
        auto secondNode = editor->GetNode( "FirstScene", "root/Group2" );

        UInt32 idx = 1;
        ASSERT_TRUE( editor->AddPlugin( "FirstScene", "root/Group1", "DEFAULT_TEXTURE", "texture", "default", idx ) );
        ASSERT_TRUE( editor->AddPlugin( "FirstScene", "root/Group2", "DEFAULT_TEXTURE", "texture", "default", idx ) );

        m_assetDesc = TextureAssetDesc::Create( imagePath_32x32, true );

        // Loading assets synchronously. BV should hang and load assets immediately.
        ASSERT_TRUE( editor->LoadAssetAsync( "FirstScene", "root/Group1", "texture", m_assetDesc ) );
        ASSERT_TRUE( editor->LoadAssetAsync( "FirstScene", "root/Group2", "texture", m_assetDesc ) );

        GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &AsyncLoadingTest::AssetLoaded ), AssetAsyncLoadFinishedEvent::Type() );
    }
}

// ***********************
//
void        AsyncLoadingTest::PreModelUpdate        ()
{
    if( m_numLoaded == 2 )
    {
        EndTestAfterThisFrame( true );

        GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &AsyncLoadingTest::AssetLoaded ), AssetAsyncLoadFinishedEvent::Type() );

        auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();

        auto firstNode = editor->GetNode( "FirstScene", "root/Group1" );
        auto secondNode = editor->GetNode( "FirstScene", "root/Group2" );

        // Check if assets were loaded corectly.

        auto texPlugin1 = std::static_pointer_cast< model::DefaultTexturePlugin >( firstNode->GetPlugin( "texture" ) );
        auto texPlugin2 = std::static_pointer_cast< model::DefaultTexturePlugin >( secondNode->GetPlugin( "texture" ) );

        auto lassets1 = texPlugin1->GetLAssets();
        auto lassets2 = texPlugin2->GetLAssets();
        ASSERT_TRUE( lassets1.size() == 1 );
        ASSERT_TRUE( lassets2.size() == 1 );

        EXPECT_EQ( lassets1[ 0 ].assetDesc, m_assetDesc );
        EXPECT_EQ( lassets2[ 0 ].assetDesc, m_assetDesc );
    }
}


// ***********************
//
void        AsyncLoadingTest::AssetLoaded           ( IEventPtr evt )
{
    m_numLoaded++;
}

