#include "Framework/FrameworkTest.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"

#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/AssetManager.h"

#include "Utils/Accessors/Assets/AssetManagerAccessor.h"


using namespace bv;


// ***********************
// InputSlots loaders should be initialized by BVAppLogic separatly, beacause they need InputSlots
// object pointer.
// This test checks if initialization properly registered loader.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Engine_InputSlots, Initialization_RegisterLoader )
{
    auto loader = TEST_ACCESSOR( AssetManager )::FindLoader( AssetManager::GetInstance(), TextureInputAssetDesc::UID() );
    EXPECT_NE( loader, nullptr );
}

