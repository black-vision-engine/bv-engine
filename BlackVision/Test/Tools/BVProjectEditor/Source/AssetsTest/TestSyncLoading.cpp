#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"


using namespace bv;


// ***********************
// Loads assets synchronously from main thread. BV should hang on loading.
class SyncLoadTest : public bv::FrameworkTest
{
private:
public:
    SyncLoadTest() : bv::FrameworkTest( "SyncLoadTest", "BVProjectEditor.Assets.Loading", __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;

} SyncLoadTestInstance;

UnitTest::ListAdder adderSyncLoadTest ( UnitTest::Test::GetTestList(), &SyncLoadTestInstance );



// ========================================================================= //
// Implementation
// ========================================================================= //


auto imagePath_32x32 = "TestAssets/BVProjectEditor/checkerboard2_32x32.png";


// ***********************
//
void        SyncLoadTest::PreEvents           ()
{
    // Create scene
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto firstNode = editor->GetNode( "FirstScene", "root/Group1" );
    auto secondNode = editor->GetNode( "FirstScene", "root/Group2" );

    UInt32 idx = 1;
    REQUIRE( editor->AddPlugin( "FirstScene", "root/Group1", "DEFAULT_TEXTURE", "texture", "default", idx ) );
    REQUIRE( editor->AddPlugin( "FirstScene", "root/Group2", "DEFAULT_TEXTURE", "texture", "default", idx ) );

    auto assetDesc = TextureAssetDesc::Create( imagePath_32x32, true );

    // Loading assets synchronously. BV should hang and load assets immediately.
    REQUIRE( editor->LoadAsset( "FirstScene", "root/Group1", "texture", assetDesc ) );
    REQUIRE( editor->LoadAsset( "FirstScene", "root/Group2", "texture", assetDesc ) );

    // Check if assets were loaded corectly.

    auto texPlugin1 = std::static_pointer_cast< model::DefaultTexturePlugin >( firstNode->GetPlugin( "texture" ) );
    auto texPlugin2 = std::static_pointer_cast< model::DefaultTexturePlugin >( secondNode->GetPlugin( "texture" ) );

    auto lassets1 = texPlugin1->GetLAssets();
    auto lassets2 = texPlugin2->GetLAssets();
    REQUIRE( lassets1.size() == 1 );
    REQUIRE( lassets2.size() == 1 );

    CHECK( lassets1[ 0 ].assetDesc == assetDesc );
    CHECK( lassets2[ 0 ].assetDesc == assetDesc );
}






