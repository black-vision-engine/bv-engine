#include "gtest/gtest.h"


#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"


#include "Assets/Input/TextureInputAssetDesc.h"


using namespace bv;




// ***********************
// Deserializes TextureInputSlot descriptor with binding to slot name.
// Slot index should evaluate to zero no matter what was in serialized file.
TEST( Serialization_Assets, TextureInputSlot_BindByName )
{
    auto actual = Deserialize< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/TextureInputSlot/BindByName.xml", "asset" );
    ASSERT_NE( actual, nullptr );

    auto & actualBinding = actual->BindingInfo();
    
    EXPECT_EQ( actualBinding.GetType(), InputSlotBinding::Type::ByName );
    EXPECT_EQ( actualBinding.GetName(), "VideoCardInput1" );
    EXPECT_EQ( actualBinding.GetIndex(), 0 );
}

// ***********************
// Deserializes TextureInputSlot descriptor with binding to slot index.
// Name should evaluate to empty string.
TEST( Serialization_Assets, TextureInputSlot_BindByIndex )
{
    auto actual = Deserialize< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/TextureInputSlot/BindByIndex.xml", "asset" );
    ASSERT_NE( actual, nullptr );

    auto & actualBinding = actual->BindingInfo();

    EXPECT_EQ( actualBinding.GetType(), InputSlotBinding::Type::ByIndex );
    EXPECT_EQ( actualBinding.GetName(), "" );
    EXPECT_EQ( actualBinding.GetIndex(), 21 );
}

// ***********************
// XML contains Type::ByIndex, but there's only name.
TEST( Serialization_Assets, TextureInputSlot_NameInsteadOfIndex )
{
    auto actual = Deserialize< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/TextureInputSlot/NameInsteadOfIndex.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}

// ***********************
// XML contains Type::ByName, but there's only index.
TEST( Serialization_Assets, TextureInputSlot_IndexInsteadOfName )
{
    auto actual = Deserialize< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/TextureInputSlot/IndexInsteadOfName.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}


