#include "gtest/gtest.h"


#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"


#include "Assets/Input/TextureInputAssetDesc.h"


using namespace bv;




// ***********************
// Deserializes TextureInputSlot descriptor with binding to slot name.
TEST( Serialization_Assets, TextureInputSlot_BindByName )
{
    auto actual = Deserialize< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/TextureInputSlot/BindByName.xml", "asset" );
    ASSERT_NE( actual, nullptr );

    auto & actualBinding = actual->BindingInfo();
    
    EXPECT_EQ( actualBinding.GetType(), InputSlotBinding::Type::ByName );
    EXPECT_EQ( actualBinding.GetName(), "VideoCardInput1" );
    EXPECT_EQ( actualBinding.GetIndex(), 0 );
}


