#include "gtest/gtest.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/AssetManager.h"

#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Serialization/SerializationHelper.h"

using namespace bv;

TEST( Assets_Animation, DirectoryWithNonGraphicFiles )
{
	auto desc = AnimationAssetDesc::Create( "TestAssets/AssetManager/MixedAnimation", 7, ".*" );

	AssetManager::GetInstance().LoadAsset( desc );
}

TEST( Assets_Animation, DirectoryWithNonGraphicFilesViaXML )
{
	BVXMLDeserializer deser( "TestAssets/AssetManager/MixedAnimation/asset.xml", nullptr );

	auto desc = SerializationHelper::DeserializeObject< const AnimationAssetDesc >( deser, "asset" );

	AssetManager::GetInstance().RemoveFromCache( desc->GetKey() );

	AssetManager::GetInstance().LoadAsset( desc );
}
