#include "Engine/Models/Resources/ResourceManager.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Resources/Texture/TextureResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/TextureResource.h"
#include "Core/Hash.h"

#include "ImageProps.h"

#include "gtest/gtest.h"

#include <windows.h>

auto imagePath = "Assets/ResourceManager/checkerboard2.png";
auto imagePath_512x512 = "Assets/ResourceManager/checkerboard2_512X512.png";
auto AssetsPath = "Assets/ResourceManager/";

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

TEST(LoaderRigistration, ResourceManager)
{
	ASSERT_TRUE( bv::ResourceManager::GetInstance().RegisterLoader( bv::TextureResourceDesc::UID(), new bv::model::TextureLoader() ) );
}

TEST(LoadingTextureAndGeneratingMipMaps, ResourceManager)
{
	auto props = GetImageProps( imagePath );

	auto orig = bv::SingleTextureResourceDesc::Create( imagePath, props.width, props.height, props.format );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureResourceDesc::Create( orig, bv::MipMapFilterType::BILINEAR );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::ResourceManager::GetInstance().LoadResource( textureResDesc );
	ASSERT_TRUE( res );
}

TEST(LoadingTexture, ResourceManager)
{
	auto props = GetImageProps( imagePath );

	auto orig = bv::SingleTextureResourceDesc::Create( imagePath, props.width, props.height, props.format );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureResourceDesc::Create( orig );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::ResourceManager::GetInstance().LoadResource( textureResDesc );
	ASSERT_TRUE( res );
}  

TEST(LoadingTexturePowefOf2Texture, ResourceManager)
{
	auto props = GetImageProps( imagePath_512x512 );

	auto orig = bv::SingleTextureResourceDesc::Create( imagePath_512x512, props.width, props.height, props.format );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureResourceDesc::Create( orig, bv::MipMapFilterType::BILINEAR );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::ResourceManager::GetInstance().LoadResource( textureResDesc );
	ASSERT_TRUE( res );

	auto typedRes = bv::model::QueryTypedRes< bv::TextureResourceConstPtr >( res );
	ASSERT_TRUE( typedRes->GetOriginal()->GetData() == typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
}  

TEST(LoadingTextureWitmMipmaps, ResourceManager)
{
	auto propsOrig = GetImageProps( AssetsPath + std::string("checkerboard2_500X500.png") );
	auto props0 = GetImageProps( AssetsPath + std::string("checkerboard2_512X512.png") );
	auto props1 = GetImageProps( AssetsPath + std::string("checkerboard2_256X256.png") );
	auto props2 = GetImageProps( AssetsPath + std::string("checkerboard2_128X128.png") );

	auto origDesc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_500X500.png"), propsOrig.width, propsOrig.height, propsOrig.format );
	ASSERT_TRUE( origDesc );

	auto mm0Desc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_512X512.png"), props0.width, props0.height, props0.format );
	ASSERT_TRUE( mm0Desc );

	auto mm1Desc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_256X256.png"), props1.width, props1.height, props1.format );
	ASSERT_TRUE( mm1Desc );

	auto mm2Desc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_128X128.png"), props2.width, props2.height, props2.format );
	ASSERT_TRUE( mm2Desc );

	std::vector< bv::SingleTextureResourceDescConstPtr > mmVec;

	mmVec.push_back( mm0Desc );
	mmVec.push_back( mm1Desc );
	mmVec.push_back( mm2Desc );

	auto mmDesc = bv::MipMapResourceDesc::Create( mmVec );

	auto textureResDesc = bv::TextureResourceDesc::Create( origDesc, mmDesc );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::ResourceManager::GetInstance().LoadResource( textureResDesc );
	ASSERT_TRUE( res );

	auto typedRes = bv::model::QueryTypedRes< bv::TextureResourceConstPtr >( res );
	ASSERT_TRUE( typedRes->GetOriginal()->GetData() != typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
	ASSERT_TRUE( typedRes->GetMipMaps()->GetLevelsNum() == 3 );

	{
		auto propsOrig = GetImageProps( AssetsPath + std::string("checkerboard2_512X512.png") );
		auto props1 = GetImageProps( AssetsPath + std::string("checkerboard2_256X256.png") );
		auto props2 = GetImageProps( AssetsPath + std::string("checkerboard2_128X128.png") );
		auto props3 = GetImageProps( AssetsPath + std::string("checkerboard2_64X64.png") );

		auto origDesc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_512X512.png"), propsOrig.width, propsOrig.height, propsOrig.format );
		ASSERT_TRUE( origDesc );

		auto mm0Desc = origDesc;

		auto mm1Desc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_256X256.png"), props1.width, props1.height, props1.format );
		ASSERT_TRUE( mm1Desc );

		auto mm2Desc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_128X128.png"), props2.width, props2.height, props2.format );
		ASSERT_TRUE( mm2Desc );

		auto mm3Desc = bv::SingleTextureResourceDesc::Create( AssetsPath + std::string("checkerboard2_64X64.png"), props3.width, props3.height, props3.format );
		ASSERT_TRUE( mm3Desc );

		std::vector< bv::SingleTextureResourceDescConstPtr > mmVec;

		mmVec.push_back( mm0Desc );
		mmVec.push_back( mm1Desc );
		mmVec.push_back( mm2Desc );
		mmVec.push_back( mm3Desc );

		auto mmDesc = bv::MipMapResourceDesc::Create( mmVec );

		auto textureResDesc = bv::TextureResourceDesc::Create( origDesc, mmDesc );
		ASSERT_TRUE( textureResDesc );

		auto res = bv::ResourceManager::GetInstance().LoadResource( textureResDesc );
		ASSERT_TRUE( res );

		auto typedRes1 = bv::model::QueryTypedRes< bv::TextureResourceConstPtr >( res );
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
