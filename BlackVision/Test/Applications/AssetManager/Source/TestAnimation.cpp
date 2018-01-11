#include "gtest/gtest.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/AssetManager.h"

#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Serialization/SerializationHelper.h"

using namespace bv;

// This tests loading animation from a directory which contains not only graphic files
// This was a cause of bug https://www.pivotaltracker.com/story/show/153056579
TEST( Assets_Animation, DirectoryWithNonGraphicFiles )
{
	auto desc = AnimationAssetDesc::Create( "TestAssets/AssetManager/MixedAnimation", 7, ".*" );

	AssetManager::GetInstance().LoadAsset( desc );
}

// The same test as above, but with loading descriptor from a XML file
TEST( Assets_Animation, DirectoryWithNonGraphicFilesViaXML )
{
	BVXMLDeserializer deser( "TestAssets/AssetManager/MixedAnimation/asset.xml", nullptr );

	auto desc = SerializationHelper::DeserializeObject< const AnimationAssetDesc >( deser, "asset" );

	AssetManager::GetInstance().RemoveFromCache( desc->GetKey() );

	AssetManager::GetInstance().LoadAsset( desc );
}
