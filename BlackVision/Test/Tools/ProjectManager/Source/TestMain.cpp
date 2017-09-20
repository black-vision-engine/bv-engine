#include "ProjectManager.h"
#include "Impl/Accessors/TextureAssetAccessor.h"
#include "Impl/Accessors/FontAssetAccessor.h"
#include "Impl/Accessors/AnimationAssetAccessor.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"

#include "Engine/Models/Plugins/Simple/Shapes/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Assets/FwdDecls.h"

#include "gtest/gtest.h"

using namespace bv;

static ProjectManager * g_pm0 = nullptr;
static ProjectManager * g_pm1 = nullptr;

namespace bv
{
    void ChangeProjectManagerInstanceTo( const std::string & newPath )
    {
        ProjectManager::SetPMInstanceOnlyForTests( new ProjectManager( newPath ) );
    }

} // bv

// *******************************
//
bool    LoadTexture     ( model::IPluginPtr plugin, const Path & projectName, const Path & path )
{
    auto texDesc = std::static_pointer_cast< const TextureAssetDesc >( g_pm0->GetAssetDesc( projectName, "textures", path ) );

    return plugin->LoadResource( texDesc );
}

bv::model::SceneModelPtr CreateTestScene0()
{
	auto sceneName = "textured_rect";
	auto scene = bv::model::SceneModel::Create( sceneName );
	auto timeline = scene->GetTimeline();

    model::TimelineManager::GetInstance()->AddTimeline( timeline );
    
    auto defaultTimeline = bv::model::OffsetTimeEvaluator::Create( "default", 0.0f );
    model::TimelineManager::GetInstance()->AddTimelineToTimeline( defaultTimeline, timeline );

    std::vector< model::IPluginDescriptor * > descriptors;

    descriptors.push_back( new model::DefaultTransformPluginDesc() );
    descriptors.push_back( new model::DefaultRect::PluginDesc() );
    descriptors.push_back( new model::DefaultTexturePluginDesc() );

    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( descriptors );

	auto root = model::BasicNode::Create( "textured_rect", timeline );

    StringVector plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_RECTANGLE" );
    plugins.push_back( "DEFAULT_TEXTURE" );

    auto success = root->AddPlugins( plugins, timeline );
    assert( success );

    auto wp = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto hp = root->GetPlugin( "rectangle" )->GetParameter( "height" );
    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    success &= SetParameter( wp, 0.f, 1.f );
    success &= SetParameter( hp, 0.f, 1.f );
    success &= SetParameterTranslation( tr, 0.0f, glm::vec3( 1.f, 1.f, 1.f ) );

    success = LoadTexture( root->GetPlugin( "texture" ), "proj00", "flagi/pol.jpg" );
    assert( success );    

	//assert( false );

	scene->SetRootNode( root );

    return scene;
}

TEST( ProjectManager, CleanAll )
{
	if( Path::Exists( "bv_media" ) )
	{
		Path::Remove( "bv_media" );
	}

	if( Path::Exists( "bv_media1" ) )
	{
		Path::Remove( "bv_media1" );
	}

}

TEST( ProjectManager, CreatingPM )
{
    auto static TM = std::make_shared < model::TimelineManager >();

    model::TimelineManager::SetInstance( TM.get() ); // FIXME: WTF? SetInstance? I don't know such a design patern.
    model::TimelineManager::GetInstance()->RegisterRootTimeline( model::OffsetTimeEvaluator::Create( "global timeline", TimeType( 0.0 ) ) );

    ChangeProjectManagerInstanceTo( "bv_media" );

    g_pm0 = ProjectManager::GetInstance();
}

TEST( ProjectManager, ProjectsListing )
{
	ASSERT_TRUE( g_pm0->ListProjectsNames().empty() );
}

TEST( ProjectManager, AddingProjects )
{
	g_pm0->AddNewProject( "proj00" );
	g_pm0->AddNewProject( "proj01" );
	g_pm0->AddNewProject( "proj02" );

	auto ps0 = g_pm0->ListProjectsNames();

	ASSERT_TRUE( ps0.size() == 3 );
	ASSERT_TRUE( ps0[ 0 ].Str() == "proj00" );
	ASSERT_TRUE( ps0[ 1 ].Str() == "proj01" );
	ASSERT_TRUE( ps0[ 2 ].Str() == "proj02" );
}

//TEST( RegisteringCategories, ProjectManager )
//{
//	StringVector exts;
//	exts.push_back( ".*\\.jpg" );
//	exts.push_back( ".*\\.tga" );
//	exts.push_back( ".*\\.png" );
//
//	auto taa = TextureAssetAccessor::Create( g_pm0->GetRootDir() / "textures", exts );
//	g_pm0->RegisterCategory( AssetCategory::Create( "textures", taa ) );
//
//	auto cns = g_pm0->ListCategoriesNames();
//
//	ASSERT_TRUE( cns.size() == 1 );
//	ASSERT_TRUE( cns[ 0 ] == "textures" );
//
//	taa = TextureAssetAccessor::Create( g_pm1->GetRootDir() / "textures", exts );
//	g_pm1->RegisterCategory( AssetCategory::Create( "textures", taa ) );
//
//	cns = g_pm1->ListCategoriesNames();
//
//	ASSERT_TRUE( cns.size() == 1 );
//	ASSERT_TRUE( cns[ 0 ] == "textures" );
//
//    StringVector fontsExts;
//	fontsExts.push_back( ".*\\.ttf" );
//
//    auto faa = FontAssetAccessor::Create( g_pm0->GetRootDir() / "fonts", fontsExts );
//	g_pm0->RegisterCategory( AssetCategory::Create( "fonts", faa ) );
//
//    auto aaa = AnimationAssetAccessor::Create( g_pm0->GetRootDir() / "sequences", exts );
//	g_pm0->RegisterCategory( AssetCategory::Create( "sequences", aaa ) );
//
//    cns = g_pm0->ListCategoriesNames();
//    ASSERT_TRUE( cns.size() == 3 );
//
//    faa = FontAssetAccessor::Create( g_pm1->GetRootDir() / "fonts", fontsExts );
//	g_pm1->RegisterCategory( AssetCategory::Create( "fonts", faa ) );
//
//    aaa = AnimationAssetAccessor::Create( g_pm1->GetRootDir() / "sequences", exts );
//	g_pm1->RegisterCategory( AssetCategory::Create( "sequences", aaa ) );
//
//    cns = g_pm1->ListCategoriesNames();
//    ASSERT_TRUE( cns.size() == 3 );
//}

TEST( ProjectManager, AddingAssets )
{
	g_pm0->AddAsset( "proj00", "textures", "flagi/pol.jpg", SingleTextureAssetDesc::Create( "TestAssets/ProjectManager/checker.png", 0, 0, TextureFormat::F_A8R8G8B8, false ) );
	g_pm0->AddAsset( "proj01", "textures", "flagi/ger.jpg", SingleTextureAssetDesc::Create( "TestAssets/ProjectManager/checker.png", 0, 0, TextureFormat::F_A8R8G8B8, false ) );
	g_pm0->AddAsset( "proj02", "textures", "flagi/rus.jpg", SingleTextureAssetDesc::Create( "TestAssets/ProjectManager/checker.png", 0, 0, TextureFormat::F_A8R8G8B8, false ) );

	auto assets = g_pm0->ListAssetsPaths( "", "", "", true );

	ASSERT_TRUE( g_pm0->GetAssetDesc( "proj00", "textures", "flagi/pol.jpg" ) != nullptr );
	ASSERT_TRUE( g_pm0->GetAssetDesc( "proj01", "textures", "flagi/ger.jpg" ) != nullptr  );
	ASSERT_TRUE( g_pm0->GetAssetDesc( "proj02", "textures", "flagi/rus.jpg" ) != nullptr  );

	ASSERT_FALSE( g_pm0->GetAssetDesc( "proj02", "textures", "flagi/pol.jpg" ) != nullptr );
	ASSERT_FALSE( g_pm0->GetAssetDesc( "proj00", "textures", "flagi/ger.jpg" ) != nullptr );
	ASSERT_FALSE( g_pm0->GetAssetDesc( "proj01", "textures", "flagi/rus.jpg" ) != nullptr );

	ASSERT_TRUE( assets.size() == 3 );
}

TEST( ProjectManager, AddingScene )
{
    g_pm0->AddScene( CreateTestScene0(), "proj00", "scene1/s.scn" );
}

//TEST( ExportingProject, ProjectManager )
//{
//    g_pm0->ExportProjectToFile( "proj00", "test.exp" );
//}

//TEST( ExportingScene, ProjectManager )
//{
//    g_pm0->ExportSceneToFile( "proj00", "scene1/s.scn", "exported_scene1.exp" );
//}

//TEST( RemovingUnusedAssets, ProjectManager )
//{
//    g_pm0->RemoveUnusedAssets( "proj01", "textures" );
//    g_pm0->RemoveUnusedAssets( "proj00" );
//    g_pm0->RemoveUnusedAssets();
//}

TEST( ProjectManager, SavingPresets )
{
    g_pm0->SavePreset( CreateTestScene0()->GetRootNode(), "proj00", "pres/proj1.bvpreset" );
}

TEST( ProjectManager, ListingPresets )
{
    ASSERT_TRUE( g_pm0->ListPresets( "proj00", "pres", true ).size() == 1 );
    ASSERT_TRUE( g_pm0->ListPresets( "proj00" ).size() == 1 );
    ASSERT_TRUE( g_pm0->ListPresets().size() == 1 );

    ASSERT_TRUE( g_pm0->ListPresets( "proj01" ).size() == 0 );
    ASSERT_TRUE( g_pm0->ListPresets( "proj01", "pres", true ).size() == 0 );
}

TEST( ProjectManager, LoadingPresets )
{
    auto scene = CreateTestScene0();
    auto timeline = bv::model::TimelineManager::GetInstance()->GetTimeEvaluator( scene->GetName() );
    auto offsetTimeline = std::dynamic_pointer_cast< model::OffsetTimeEvaluator >( timeline );

    ASSERT_TRUE( offsetTimeline != nullptr );
    ASSERT_TRUE( g_pm0->LoadPreset( "proj00", "pres/proj1.bvpreset", offsetTimeline ) != nullptr ); // FIXME: 
}

TEST( ProjectManager, CreatingSecondPM )
{
    ChangeProjectManagerInstanceTo( "bv_media1" );
    g_pm1 = ProjectManager::GetInstance();
    ASSERT_TRUE( g_pm1 != g_pm0 );
}

TEST( ProjectManager, ProjectsListing2 )
{
    ASSERT_TRUE( g_pm1->ListProjectsNames().empty() );
}

TEST( ProjectManager, AddingPorjects2 )
{
	g_pm1->AddNewProject( "proj10" );
	g_pm1->AddNewProject( "proj11" );
	g_pm1->AddNewProject( "proj12" );
    g_pm1->AddNewProject( "proj13" );

	auto ps1 = g_pm1->ListProjectsNames();

	ASSERT_TRUE( ps1.size() == 4 );
	ASSERT_TRUE( ps1[ 0 ].Str() == "proj10" );
	ASSERT_TRUE( ps1[ 1 ].Str() == "proj11" );
	ASSERT_TRUE( ps1[ 2 ].Str() == "proj12" );
    ASSERT_TRUE( ps1[ 3 ].Str() == "proj13" );
}

//TEST( ImportingScene, ProjectManager )
//{
//    g_pm1->ImportSceneFromFile( "proj01", "scene1exp/s.scn", "exported_scene1.exp" );
//}

//TEST( ImportingProject, ProjectManager )
//{
//    g_pm1->ImportProjectFromFile( "test.exp", "proj00" );
//    g_pm1->ImportProjectFromFile( "test.exp", "proj01" );
//}

int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
