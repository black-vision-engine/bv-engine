#include "Assets/AssetManager.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/TextureAsset.h"
#include "Engine/Types/EnumsUtils.h"
#include "DataTypes/Hash.h"

#include "LibImage.h"

#include "gtest/gtest.h"

#include <windows.h>

auto imagePath = "Assets/AssetManager/checkerboard2.png";
auto imagePath_512x512 = "Assets/AssetManager/checkerboard2_512X512.png";
auto AssetsPath = "Assets/AssetManager/";

TEST(HashTest, Hash)
{
	auto h0 = bv::Hash::FromString( imagePath_512x512 );
	auto h1 = bv::Hash::FromString( imagePath );
	auto h2 = bv::Hash::FromString( AssetsPath + std::string("checkerboard2_512X512.png") );
	auto h3 = bv::Hash::FromString( "12312312" );
	auto h4 = bv::Hash::FromString( "12312312" );

	ASSERT_TRUE( h0 == h2 );
	ASSERT_TRUE( h3 == h4 );
	ASSERT_TRUE( !( h0 == h1 ) );
}

TEST(LoaderRigistration, AssetManager)
{
	ASSERT_TRUE( bv::AssetManager::GetInstance().RegisterLoader( bv::TextureAssetDesc::UID(), new bv::model::TextureLoader() ) );
}

TEST(LoadingTextureAndGeneratingMipMaps, AssetManager)
{
	auto props = bv::image::GetImageProps( imagePath );
	ASSERT_TRUE( props.error.empty() );

	auto orig = bv::SingleTextureAssetDesc::Create( imagePath, props.width, props.height, bv::EnumsUtils::Convert( props.format ), true );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureAssetDesc::Create( orig, bv::MipMapFilterType::BILINEAR );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::AssetManager::GetInstance().LoadAsset( textureResDesc );
	ASSERT_TRUE( res );
}

TEST(LoadingTexture, AssetManager)
{
	auto props = bv::image::GetImageProps( imagePath );
	ASSERT_TRUE( props.error.empty() );

	auto orig = bv::SingleTextureAssetDesc::Create( imagePath, props.width, props.height, bv::EnumsUtils::Convert( props.format ), true );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureAssetDesc::Create( orig );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::AssetManager::GetInstance().LoadAsset( textureResDesc );
	ASSERT_TRUE( res );
}  

TEST(LoadingTexturePowefOf2Texture, AssetManager)
{
	auto props = bv::image::GetImageProps( imagePath_512x512 );
	ASSERT_TRUE( props.error.empty() );

	auto orig = bv::SingleTextureAssetDesc::Create( imagePath_512x512, props.width, props.height, bv::EnumsUtils::Convert( props.format ), true );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureAssetDesc::Create( orig, bv::MipMapFilterType::BILINEAR );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::AssetManager::GetInstance().LoadAsset( textureResDesc );
	ASSERT_TRUE( res );

	auto typedRes = bv::model::QueryTypedRes< bv::TextureAssetConstPtr >( res );
	ASSERT_TRUE( typedRes->GetOriginal()->GetData() == typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
}  

TEST(LoadingTextureWitmMipmaps, AssetManager)
{
	auto propsOrig = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_500X500.png") );
	ASSERT_TRUE( propsOrig.error.empty() );
	auto props0 = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_512X512.png") );
	ASSERT_TRUE( props0.error.empty() );
	auto props1 = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_256X256.png") );
	ASSERT_TRUE( props1.error.empty() );
	auto props2 = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_128X128.png") );
	ASSERT_TRUE( props2.error.empty() );

	auto origDesc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_500X500.png"), propsOrig.width, propsOrig.height, bv::EnumsUtils::Convert( propsOrig.format ), true );
	ASSERT_TRUE( origDesc );

	auto mm0Desc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_512X512.png"), props0.width, props0.height, bv::EnumsUtils::Convert( props0.format ), true );
	ASSERT_TRUE( mm0Desc );

	auto mm1Desc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_256X256.png"), props1.width, props1.height, bv::EnumsUtils::Convert( props1.format ), true );
	ASSERT_TRUE( mm1Desc );

	auto mm2Desc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_128X128.png"), props2.width, props2.height, bv::EnumsUtils::Convert( props2.format ), true );
	ASSERT_TRUE( mm2Desc );

	std::vector< bv::SingleTextureAssetDescConstPtr > mmVec;

	mmVec.push_back( mm0Desc );
	mmVec.push_back( mm1Desc );
	mmVec.push_back( mm2Desc );

	auto mmDesc = bv::MipMapAssetDesc::Create( mmVec );

	auto textureResDesc = bv::TextureAssetDesc::Create( origDesc, mmDesc );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::AssetManager::GetInstance().LoadAsset( textureResDesc );
	ASSERT_TRUE( res );

	auto typedRes = bv::model::QueryTypedRes< bv::TextureAssetConstPtr >( res );
	ASSERT_TRUE( typedRes->GetOriginal()->GetData() != typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
	ASSERT_TRUE( typedRes->GetMipMaps()->GetLevelsNum() == 3 );

	{
		auto propsOrig = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_512X512.png") );
		ASSERT_TRUE( propsOrig.error.empty() );
		auto props1 = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_256X256.png") );
		ASSERT_TRUE( props1.error.empty() );
		auto props2 = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_128X128.png") );
		ASSERT_TRUE( props2.error.empty() );
		auto props3 = bv::image::GetImageProps( AssetsPath + std::string("checkerboard2_64X64.png") );
		ASSERT_TRUE( props3.error.empty() );

		auto origDesc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_512X512.png"), propsOrig.width, propsOrig.height, bv::EnumsUtils::Convert( propsOrig.format ), true );
		ASSERT_TRUE( origDesc );

		auto mm0Desc = origDesc;

		auto mm1Desc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_256X256.png"), props1.width, props1.height, bv::EnumsUtils::Convert( props1.format ), true );
		ASSERT_TRUE( mm1Desc );

		auto mm2Desc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_128X128.png"), props2.width, props2.height, bv::EnumsUtils::Convert( props2.format ), true );
		ASSERT_TRUE( mm2Desc );

		auto mm3Desc = bv::SingleTextureAssetDesc::Create( AssetsPath + std::string("checkerboard2_64X64.png"), props3.width, props3.height, bv::EnumsUtils::Convert( props3.format ), true );
		ASSERT_TRUE( mm3Desc );

		std::vector< bv::SingleTextureAssetDescConstPtr > mmVec;

		mmVec.push_back( mm0Desc );
		mmVec.push_back( mm1Desc );
		mmVec.push_back( mm2Desc );
		mmVec.push_back( mm3Desc );

		auto mmDesc = bv::MipMapAssetDesc::Create( mmVec );

		auto textureResDesc = bv::TextureAssetDesc::Create( origDesc, mmDesc );
		ASSERT_TRUE( textureResDesc );

		auto res = bv::AssetManager::GetInstance().LoadAsset( textureResDesc );
		ASSERT_TRUE( res );

		auto typedRes1 = bv::model::QueryTypedRes< bv::TextureAssetConstPtr >( res );
		ASSERT_TRUE( typedRes1->GetOriginal()->GetData() == typedRes1->GetMipMaps()->GetLevel( 0 )->GetData() );

		ASSERT_TRUE( typedRes1->GetOriginal()->GetData() != typedRes->GetOriginal()->GetData() );
		ASSERT_TRUE( typedRes1->GetOriginal()->GetData() == typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
		ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevel( 0 )->GetData() == typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
		ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevel( 1 )->GetData() == typedRes->GetMipMaps()->GetLevel( 1 )->GetData() );
		ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevel( 2 )->GetData() == typedRes->GetMipMaps()->GetLevel( 2 )->GetData() );
		ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevelsNum() == 4 );
	}
}  


int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
