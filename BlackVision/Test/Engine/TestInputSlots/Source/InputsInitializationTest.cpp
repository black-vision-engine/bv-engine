#include "Framework/FrameworkTest.h"

#include "Framework/BVTestAppLogic.h"
#include "Engine/Graphics/Effects/Logic/RenderLogic.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/Logic/InputLogic.h"

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


// ***********************
// Config defines 2 video cards and 3 inputs. Check if InputSlots were initialized properly.
POST_RENDER_FRAMEWORK_TEST( Engine_InputSlots, Initialization_2VideoCards3Inputs )
{
    auto inputSlots = GetAppLogic()->GetRenderLogic()->GetInputLogic()->GetInputSlots();

    auto slot0 = inputSlots->AccessSource( 0 );
    ASSERT_TRUE( slot0.IsValid() );
    EXPECT_NE( slot0.GetVal().Texture, nullptr );
    EXPECT_NE( slot0.GetVal().Audio, nullptr );

    auto slot1 = inputSlots->AccessSource( 1 );
    ASSERT_TRUE( slot1.IsValid() );
    EXPECT_NE( slot1.GetVal().Texture, nullptr );
    EXPECT_NE( slot1.GetVal().Audio, nullptr );

    auto slot2 = inputSlots->AccessSource( 2 );
    ASSERT_TRUE( slot2.IsValid() );
    EXPECT_NE( slot2.GetVal().Texture, nullptr );
    EXPECT_NE( slot2.GetVal().Audio, nullptr );
}


