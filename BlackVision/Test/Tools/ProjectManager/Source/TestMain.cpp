#include "ProjectManager.h"
#include "Impl/Accessors/TextureAssetAccessor.h"
#include "Impl/Accessors/FontAssetAccessor.h"
#include "Impl/Accessors/AnimationAssetAccessor.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Assets/FwdDecls.h"

#include "gtest/gtest.h"

using namespace bv;

static ProjectManager * g_pm0 = nullptr;
static ProjectManager * g_pm1 = nullptr;

namespace bv
{
    auto global_tm = new model::TimelineManager();
} // bv

// *******************************
//
bool    LoadTexture     ( model::IPluginPtr plugin, const Path & projectName, const Path & path )
{
    auto texDesc = std::static_pointer_cast< const SingleTextureAssetDesc >( g_pm0->GetAssetDesc( projectName, "textures", path ) );

    return plugin->LoadResource( TextureAssetDesc::Create( texDesc ) );
}

bv::model::BasicNodeConstPtr CreateTestScene0()
{
    std::vector< model::IPluginDescriptor * > descriptors;

    descriptors.push_back( new model::DefaultTransformPluginDesc() );
    descriptors.push_back( new model::DefaultRectPluginDesc() );
    descriptors.push_back( new model::DefaultTexturePluginDesc() );

    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( descriptors );

    auto globalTimeline = model::OffsetTimeEvaluatorPtr( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) );
    auto root = model::BasicNode::Create( "textured_rect", globalTimeline );

    StringVector plugins;
    plugins.push_back( "DEFAULT_TRANSFORM" );
    plugins.push_back( "DEFAULT_RECTANGLE" );
    plugins.push_back( "DEFAULT_TEXTURE" );

    auto success = root->AddPlugins( plugins, globalTimeline );
    assert( success );

    auto wp = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto hp = root->GetPlugin( "rectangle" )->GetParameter( "height" );
    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    success &= SetParameter( wp, 0.f, 1.f );
    success &= SetParameter( hp, 0.f, 1.f );
    success &= SetParameterTranslation( tr, 0, 0.0f, glm::vec3( 1.f, 1.f, 1.f ) );

    success = LoadTexture( root->GetPlugin( "texture" ), "proj00", "flagi/pol.jpg" );
    assert( success );    

    return root;
}

TEST( CleanAll, ProjectManager )
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

TEST( CreatingPM, ProjectManager )
{
    global_tm->RegisterRootTimeline( model::OffsetTimeEvaluatorPtr( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) ) );

    g_pm0 = ProjectManager::GetInstance( "bv_media", global_tm );
	g_pm1 = ProjectManager::GetInstance( "bv_media1", global_tm );
}

TEST( ProjectsListing, ProjectManager )
{
	ASSERT_TRUE( g_pm0->ListProjectsNames().empty() );
	ASSERT_TRUE( g_pm1->ListProjectsNames().empty() );
}

TEST( AddingProjects, ProjectManager )
{
	g_pm0->AddNewProject( "proj00" );
	g_pm0->AddNewProject( "proj01" );
	g_pm0->AddNewProject( "proj02" );

	auto ps0 = g_pm0->ListProjectsNames();

	ASSERT_TRUE( ps0.size() == 3 );
	ASSERT_TRUE( ps0[ 0 ].Str() == "proj00" );
	ASSERT_TRUE( ps0[ 1 ].Str() == "proj01" );
	ASSERT_TRUE( ps0[ 2 ].Str() == "proj02" );

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

TEST( RegisteringCategories, ProjectManager )
{
	StringVector exts;
	exts.push_back( ".*\\.jpg" );
	exts.push_back( ".*\\.tga" );
	exts.push_back( ".*\\.png" );

	auto taa = TextureAssetAccessor::Create( g_pm0->GetRootDir() / "textures", exts );
	g_pm0->RegisterCategory( AssetCategory::Create( "textures", taa ) );

	auto cns = g_pm0->ListCategoriesNames();

	ASSERT_TRUE( cns.size() == 1 );
	ASSERT_TRUE( cns[ 0 ] == "textures" );

	taa = TextureAssetAccessor::Create( g_pm1->GetRootDir() / "textures", exts );
	g_pm1->RegisterCategory( AssetCategory::Create( "textures", taa ) );

	cns = g_pm1->ListCategoriesNames();

	ASSERT_TRUE( cns.size() == 1 );
	ASSERT_TRUE( cns[ 0 ] == "textures" );

    StringVector fontsExts;
	fontsExts.push_back( ".*\\.ttf" );

    auto faa = FontAssetAccessor::Create( g_pm0->GetRootDir() / "fonts", fontsExts );
	g_pm0->RegisterCategory( AssetCategory::Create( "fonts", faa ) );

    auto aaa = AnimationAssetAccessor::Create( g_pm0->GetRootDir() / "sequences", exts );
	g_pm0->RegisterCategory( AssetCategory::Create( "sequences", aaa ) );

    cns = g_pm0->ListCategoriesNames();
    ASSERT_TRUE( cns.size() == 3 );

    faa = FontAssetAccessor::Create( g_pm1->GetRootDir() / "fonts", fontsExts );
	g_pm1->RegisterCategory( AssetCategory::Create( "fonts", faa ) );

    aaa = AnimationAssetAccessor::Create( g_pm1->GetRootDir() / "sequences", exts );
	g_pm1->RegisterCategory( AssetCategory::Create( "sequences", aaa ) );

    cns = g_pm1->ListCategoriesNames();
    ASSERT_TRUE( cns.size() == 3 );
}

TEST( AddingAssets, ProjectManager )
{
	g_pm0->AddAsset( "proj00", "textures", "flagi/pol.jpg", SingleTextureAssetDesc::Create( "test_data.file", 0, 0, TextureFormat::F_A8R8G8B8, false ) );
	g_pm0->AddAsset( "proj01", "textures", "flagi/ger.jpg", SingleTextureAssetDesc::Create( "test_data.file", 0, 0, TextureFormat::F_A8R8G8B8, false ) );
	g_pm0->AddAsset( "proj02", "textures", "flagi/rus.jpg", SingleTextureAssetDesc::Create( "test_data.file", 0, 0, TextureFormat::F_A8R8G8B8, false ) );

	auto assets = g_pm0->ListAssetsPaths( "", "" );

	ASSERT_TRUE( g_pm0->GetAssetDesc( "proj00", "textures", "flagi/pol.jpg" ) );
	ASSERT_TRUE( g_pm0->GetAssetDesc( "proj01", "textures", "flagi/ger.jpg" ) );
	ASSERT_TRUE( g_pm0->GetAssetDesc( "proj02", "textures", "flagi/rus.jpg" ) );

	ASSERT_FALSE( g_pm0->GetAssetDesc( "proj02", "textures", "flagi/pol.jpg" ) );
	ASSERT_FALSE( g_pm0->GetAssetDesc( "proj00", "textures", "flagi/ger.jpg" ) );
	ASSERT_FALSE( g_pm0->GetAssetDesc( "proj01", "textures", "flagi/rus.jpg" ) );

	ASSERT_TRUE( assets.size() == 3 );
}

TEST( AddingScene, ProjectManager )
{
    g_pm0->AddScene( CreateTestScene0(), "proj00", "scene1/s.scn" );
}

TEST( ExportingProject, ProjectManager )
{
    g_pm0->ExportProjectToFile( "proj00", "test.exp" );
}

TEST( ImportingProject, ProjectManager )
{
    g_pm1->ImportProjectFromFile( "test.exp", "proj00" );
    g_pm1->ImportProjectFromFile( "test.exp", "proj01" );
}

TEST( RemovingUnusedAssets, ProjectManager )
{
    g_pm0->RemoveUnusedAssets( "proj01", "textures" );
    g_pm0->RemoveUnusedAssets( "proj00" );
    g_pm0->RemoveUnusedAssets();
}

TEST( SavingPreset, ProjectManager )
{
    g_pm0->SavePreset( CreateTestScene0(), "proj00", "pres/proj1.bvpreset" );
}

TEST( ListingPreset, ProjectManager )
{
    ASSERT_TRUE( g_pm0->ListPresets( "proj00", "pres" ).size() == 1 );
    ASSERT_TRUE( g_pm0->ListPresets( "proj00" ).size() == 1 );
    ASSERT_TRUE( g_pm0->ListPresets().size() == 1 );

    ASSERT_TRUE( g_pm0->ListPresets( "proj01" ).size() == 0 );
    ASSERT_TRUE( g_pm0->ListPresets( "proj01", "pres" ).size() == 0 );
}

TEST( LoadingPresets, ProjectManager )
{
    ASSERT_TRUE( g_pm0->LoadPreset( "proj00", "pres/proj1.bvpreset" ) );
}

int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
