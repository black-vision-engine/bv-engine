#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"

#include "Utils/Nodes/TestNodesCreator.h"
#include "Assets/DefaultAssets.h"



using namespace bv;



// ***********************
// If texture doesn't exists, plugin should load fallback texture defined in config.
// LoadResource function should return false in this case.
class FallbackTextureTest : public bv::FrameworkTest
{
private:
public:
    FallbackTextureTest() : bv::FrameworkTest( "FallbackTextureTest", "BVProjectEditor.Assets.Loading", __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;

} FallbacktextureTestInstance;

UnitTest::ListAdder adderFallbacktextureTest ( UnitTest::Test::GetTestList(), &FallbacktextureTestInstance );



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
    REQUIRE CHECK( texturePlugin != nullptr );

    auto desc = TextureAssetDesc::Create( "nonExistingTexture.jpg", true );

    // Loading fallback texture result in false return value.
    CHECK( !texturePlugin->LoadResource( desc ) );

    auto lassets = texturePlugin->GetLAssets();
    REQUIRE CHECK( lassets.size() == 1 );

    auto texDesc = std::static_pointer_cast< const TextureAssetDesc >( lassets[ 0 ].assetDesc );
    
    CHECK( texDesc->GetOrigTextureDesc()->GetImagePath() != "nonExistingTexture.jpg" );
    CHECK( texDesc->GetOrigTextureDesc()->GetImagePath() != imagePath_32x32 );

    auto fallbackDesc = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();

    CHECK( texDesc->GetOrigTextureDesc()->GetImagePath() == fallbackDesc->GetOrigTextureDesc()->GetImagePath() );
}
