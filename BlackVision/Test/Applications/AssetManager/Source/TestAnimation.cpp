#include "gtest/gtest.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/AssetManager.h"

using namespace bv;

TEST( Assets_Animation, DirectoryWithNonGraphicFiles )
{
	auto desc = AnimationAssetDesc::Create( "TestAssets/AssetManager/MixedAnimation", 7, ".*" );

	AssetManager::GetInstance().LoadAsset( desc );
}
