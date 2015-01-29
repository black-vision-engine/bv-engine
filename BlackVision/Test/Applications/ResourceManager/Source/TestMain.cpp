#include "Engine/Models/Resources/ResourceManager.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Resources/Texture/TextureResourceDescriptor.h"

#include "ImageProps.h"

#include "gtest/gtest.h"

auto imagePath = "Assets/ResourceManager/checkerbord2.bmp";

TEST(LoaderRigistration, ResourceManager)
{
	//bv::model::TextureLoader::
	ASSERT_TRUE( bv::ResourceManager::GetInstance().RegisterLoader( bv::TextureResourceDesc::UID(), new bv::model::TextureLoader() ) );
}

TEST(LoadingTextureAndGeneratingMipMaps, ResourceManager)
{
	auto props = GetImageProps( imagePath );

	auto orig = bv::SingleTextureResourceDesc::Create( imagePath, props.width, props.height, props.format );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureResourceDesc::Create( orig, true );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::ResourceManager::GetInstance().LoadResource( textureResDesc );
	ASSERT_TRUE( res );
}

TEST(LoadingTexture, ResourceManager)
{
	auto imagePath = "";
	auto props = GetImageProps( imagePath );

	auto orig = bv::SingleTextureResourceDesc::Create( imagePath, props.width, props.height, props.format );
	ASSERT_TRUE( orig );

	auto textureResDesc = bv::TextureResourceDesc::Create( orig, false );
	ASSERT_TRUE( textureResDesc );

	auto res = bv::ResourceManager::GetInstance().LoadResource( textureResDesc );
	ASSERT_TRUE( res );
}


int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
