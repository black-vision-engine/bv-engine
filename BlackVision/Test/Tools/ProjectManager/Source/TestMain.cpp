#include "ProjectManager.h"
#include "Impl/Accessors/TextureAssetAccessor.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"

#include "gtest/gtest.h"

using namespace bv;

static ProjectManager * g_pm0 = nullptr;
static ProjectManager * g_pm1 = nullptr;

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
	g_pm0 = ProjectManager::GetInstance( "bv_media" );
	g_pm1 = ProjectManager::GetInstance( "bv_media1" );
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

int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
