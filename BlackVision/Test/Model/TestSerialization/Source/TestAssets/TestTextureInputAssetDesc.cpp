#include "gtest/gtest.h"

#include "IO/DirIO.h"

#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"

#include "Utils/Comparators/Assets/TextureInputAssetComparator.h"

#include "Assets/Input/TextureInputAssetDesc.h"


using namespace bv;

static int createOutputDirFirst = Dir::CreateDir( "Out" );


// ***********************
// Serialize and deserialize by index binding.
TEST( Serialization_Assets, TextureInputSlot_SerDeserIndexed )
{
    InputSlotBinding binding( "slot blabla" );

    auto expectedAssetDesc = TextureInputAssetDesc::Create( binding );
    Serialize( *( expectedAssetDesc.get() ), "Out/SerDeserIndexed.xml" );
    auto actualAssetDesc = Deserialize< const TextureInputAssetDesc >( "Out/SerDeserIndexed.xml", "asset" );

    EXPECT_TRUE( model::TextureInputAssetComparator::CompareStructure( expectedAssetDesc, actualAssetDesc ) );
}

// ***********************
// Serialize and deserialize by name binding.
TEST( Serialization_Assets, TextureInputSlot_SerDeserNamed )
{
    InputSlotBinding binding( 152 );

    auto expectedAssetDesc = TextureInputAssetDesc::Create( binding );
    Serialize( *( expectedAssetDesc.get() ), "Out/SerDeserNamed.xml" );
    auto actualAssetDesc = Deserialize< const TextureInputAssetDesc >( "Out/SerDeserNamed.xml", "asset" );

    EXPECT_TRUE( model::TextureInputAssetComparator::CompareStructure( expectedAssetDesc, actualAssetDesc ) );
}


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

// ***********************
// XML contains empty slotBinding marker.
TEST( Serialization_Assets, TextureInputSlot_EmptySlotBinding )
{
    auto actual = Deserialize< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/TextureInputSlot/EmptySlotBinding.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}

// ***********************
// XML contains no slotBinding marker.
TEST( Serialization_Assets, TextureInputSlot_NoSlotBindingMarker )
{
    auto actual = Deserialize< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/TextureInputSlot/NoSlotBindingMarker.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}
