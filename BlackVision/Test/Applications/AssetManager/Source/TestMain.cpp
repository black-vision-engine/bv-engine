#include "Assets/Assets.h"
#include "Engine/Types/EnumsUtils.h"
#include "DataTypes/Hash.h"

#include "Tools/Base64.h"
#include "LibImage.h"

#include "gtest/gtest.h"

#include "win_sock.h"


namespace bv
{

auto imagePath = "TestAssets/AssetManager/checkerboard2.png";
auto imagePath_512x512 = "TestAssets/AssetManager/checkerboard2_512X512.png";
auto AssetsPath = std::string( "TestAssets/AssetManager/" );
auto PMFilePrefix = std::string( "file:/" );




// ***********************
//
TEST( Assets_AssetManager, LoadingThumbnails )
{

}

// ***********************
//
TEST( Assets_AssetManager, Hash )
{
    static char * c = new char[ 6 ];
    memset( c, 'a', 6 );
    auto m = MemoryChunk::Create( c , 6);
    auto str = EncodeBase64( m );

    auto d = DecodeBase64( str );

    auto h0 = Hash::FromString( imagePath_512x512 );
    auto h1 = Hash::FromString( imagePath );
    auto h2 = Hash::FromString( AssetsPath + std::string("checkerboard2_512X512.png") );
    auto h3 = Hash::FromString( "12312312" );
    auto h4 = Hash::FromString( "12312312" );

    ASSERT_TRUE( h0 == h2 );
    ASSERT_TRUE( h3 == h4 );
    ASSERT_TRUE( !( h0 == h1 ) );
}

// ***********************
//
TEST( Assets_AssetManager, LoadingTextureAndGeneratingMipMaps )
{
	auto res = LoadTextureAsset( PMFilePrefix + imagePath, MipMapFilterType::BILINEAR );
	ASSERT_TRUE( res != nullptr );
}

// ***********************
//
TEST( Assets_AssetManager, LoadingTexture )
{
	auto res = LoadTextureAsset( PMFilePrefix + imagePath );
	ASSERT_TRUE( res != nullptr );
}

// ***********************
//
TEST( Assets_AssetManager, LoadingTexturePowefOf2Texture )
{
    // FIXME: If this test fails, maybe solution is to clean cache directory.
    // FIXME: What is exepcted solution, when texture is loaded from cache ?
    auto typedRes = LoadTextureAsset( PMFilePrefix + imagePath_512x512, MipMapFilterType::BILINEAR );
    ASSERT_TRUE( typedRes->GetOriginal()->GetData() == typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
}  

// ***********************
//
TEST( Assets_AssetManager, LoadingTextureWitmMipmaps )
{
    auto propsOrig = image::GetImageProps( AssetsPath + std::string("checkerboard2_500X500.png") );
    ASSERT_TRUE( propsOrig.error.empty() );
    auto props0 = image::GetImageProps( AssetsPath + std::string("checkerboard2_512X512.png") );
    ASSERT_TRUE( props0.error.empty() );
    auto props1 = image::GetImageProps( AssetsPath + std::string("checkerboard2_256X256.png") );
    ASSERT_TRUE( props1.error.empty() );
    auto props2 = image::GetImageProps( AssetsPath + std::string("checkerboard2_128X128.png") );
    ASSERT_TRUE( props2.error.empty() );

	auto origDesc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_500X500.png"), propsOrig.width, propsOrig.height, EnumsUtils::Convert( propsOrig.format ), true );
	ASSERT_TRUE( origDesc != nullptr );

	auto mm0Desc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_512X512.png"), props0.width, props0.height, EnumsUtils::Convert( props0.format ), true );
	ASSERT_TRUE( mm0Desc != nullptr );

	auto mm1Desc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_256X256.png"), props1.width, props1.height, EnumsUtils::Convert( props1.format ), true );
	ASSERT_TRUE( mm1Desc != nullptr );

	auto mm2Desc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_128X128.png"), props2.width, props2.height, EnumsUtils::Convert( props2.format ), true );
	ASSERT_TRUE( mm2Desc != nullptr );

    std::vector< SingleTextureAssetDescConstPtr > mmVec;

    mmVec.push_back( mm0Desc );
    mmVec.push_back( mm1Desc );
    mmVec.push_back( mm2Desc );

    auto mmDesc = MipMapAssetDesc::Create( mmVec );

	auto textureResDesc = TextureAssetDesc::Create( origDesc, mmDesc );
	ASSERT_TRUE( textureResDesc != nullptr );

	auto res = AssetManager::GetInstance().LoadAsset( textureResDesc );
	ASSERT_TRUE( res != nullptr );

    auto typedRes = QueryTypedRes< TextureAssetConstPtr >( res );
    ASSERT_TRUE( typedRes->GetOriginal()->GetData() != typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
    ASSERT_TRUE( typedRes->GetMipMaps()->GetLevelsNum() == 3 );

	{
		propsOrig = image::GetImageProps( AssetsPath + std::string("checkerboard2_512X512.png") );
		ASSERT_TRUE( propsOrig.error.empty() );
		props1 = image::GetImageProps( AssetsPath + std::string("checkerboard2_256X256.png") );
		ASSERT_TRUE( props1.error.empty() );
		props2 = image::GetImageProps( AssetsPath + std::string("checkerboard2_128X128.png") );
		ASSERT_TRUE( props2.error.empty() );
		auto props3 = image::GetImageProps( AssetsPath + std::string("checkerboard2_64X64.png") );
		ASSERT_TRUE( props3.error.empty() );

		origDesc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_512X512.png"), propsOrig.width, propsOrig.height, EnumsUtils::Convert( propsOrig.format ), true );
		ASSERT_TRUE( origDesc != nullptr );

		mm0Desc = origDesc;

		mm1Desc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_256X256.png"), props1.width, props1.height, EnumsUtils::Convert( props1.format ), true );
		ASSERT_TRUE( mm1Desc != nullptr );

		mm2Desc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_128X128.png"), props2.width, props2.height, EnumsUtils::Convert( props2.format ), true );
		ASSERT_TRUE( mm2Desc != nullptr );

		auto mm3Desc = SingleTextureAssetDesc::Create( PMFilePrefix + AssetsPath + std::string("checkerboard2_64X64.png"), props3.width, props3.height, EnumsUtils::Convert( props3.format ), true );
		ASSERT_TRUE( mm3Desc != nullptr );

        mmVec.clear();

        mmVec.push_back( mm0Desc );
        mmVec.push_back( mm1Desc );
        mmVec.push_back( mm2Desc );
        mmVec.push_back( mm3Desc );

		mmDesc = MipMapAssetDesc::Create( mmVec );

		textureResDesc = TextureAssetDesc::Create( origDesc, mmDesc );
		ASSERT_TRUE( textureResDesc != nullptr );

		res = AssetManager::GetInstance().LoadAsset( textureResDesc );
		ASSERT_TRUE( res != nullptr );

        auto typedRes1 = QueryTypedRes< TextureAssetConstPtr >( res );
        ASSERT_TRUE( typedRes1->GetOriginal()->GetData() == typedRes1->GetMipMaps()->GetLevel( 0 )->GetData() );

        ASSERT_TRUE( typedRes1->GetOriginal()->GetData() != typedRes->GetOriginal()->GetData() );
        ASSERT_TRUE( typedRes1->GetOriginal()->GetData() == typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
        ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevel( 0 )->GetData() == typedRes->GetMipMaps()->GetLevel( 0 )->GetData() );
        ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevel( 1 )->GetData() == typedRes->GetMipMaps()->GetLevel( 1 )->GetData() );
        ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevel( 2 )->GetData() == typedRes->GetMipMaps()->GetLevel( 2 )->GetData() );
        ASSERT_TRUE( typedRes1->GetMipMaps()->GetLevelsNum() == 4 );
    }
}  

} // bv


// ***********************
//
int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}

