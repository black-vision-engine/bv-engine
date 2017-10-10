#include "gtest/gtest.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"

#include "Mathematics/glm_inc.h"


using namespace bv;


// ========================================================================= //
// Simple serialization - deserialization - comparision tests
//                      for different types
// ========================================================================= //


// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeFloat )
{
    CompositeInterpolator< TimeType, float > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, 3.0f );
    expectedInterpolator.AddKey( 1.0f, 3.0f );
    expectedInterpolator.AddKey( 3.0f, 13.0f );
    expectedInterpolator.AddKey( 4.0f, 2.0f );
    expectedInterpolator.AddKey( 7.0f, 5.0f );

    Serialize( expectedInterpolator, "SerDeserFloat.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, float > >( "SerDeserFloat.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeInt )
{
    CompositeInterpolator< TimeType, int > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, 1 );
    expectedInterpolator.AddKey( 1.0f, 3 );
    expectedInterpolator.AddKey( 3.0f, 7 );
    expectedInterpolator.AddKey( 4.0f, 2 );
    expectedInterpolator.AddKey( 7.0f, 10 );

    Serialize( expectedInterpolator, "SerDeserInt.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, int > >( "SerDeserInt.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeBool )
{
    CompositeInterpolator< TimeType, bool > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, true );
    expectedInterpolator.AddKey( 1.0f, false );
    expectedInterpolator.AddKey( 3.0f, false );
    expectedInterpolator.AddKey( 4.0f, true );
    expectedInterpolator.AddKey( 7.0f, false );

    Serialize( expectedInterpolator, "SerDeserBool.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, bool > >( "SerDeserBool.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeString )
{
    CompositeInterpolator< TimeType, std::string > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, "Blaaa" );
    expectedInterpolator.AddKey( 1.0f, "Dobre coo" );
    expectedInterpolator.AddKey( 3.0f, "�mieszne warto��i" );
    expectedInterpolator.AddKey( 4.0f, "z��k�a ki��" );
    expectedInterpolator.AddKey( 7.0f, "�d�b�o" );

    Serialize( expectedInterpolator, "SerDeserString.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, std::string > >( "SerDeserString.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}


// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeWString )
{
    CompositeInterpolator< TimeType, std::wstring > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, L"Blaaa" );
    expectedInterpolator.AddKey( 1.0f, L"Dobre coo" );
    expectedInterpolator.AddKey( 3.0f, L"�mieszne warto��i" );
    expectedInterpolator.AddKey( 4.0f, L"z��k�a ki��" );
    expectedInterpolator.AddKey( 7.0f, L"�d�b�o" );

    Serialize( expectedInterpolator, "SerDeserWString.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, std::wstring > >( "SerDeserWString.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeVec2 )
{
    CompositeInterpolator< TimeType, glm::vec2 > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, glm::vec2( 2.0, 3.0 ) );
    expectedInterpolator.AddKey( 1.0f, glm::vec2( 22.0, 37.0 ) );
    expectedInterpolator.AddKey( 3.0f, glm::vec2( 0.0, 0.00001 ) );
    expectedInterpolator.AddKey( 4.0f, glm::vec2( 0.0, -34.0 ) );
    expectedInterpolator.AddKey( 7.0f, glm::vec2( 22.34315325, 3.1231 ) );

    Serialize( expectedInterpolator, "SerDeserVec2.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, glm::vec2 > >( "SerDeserVec2.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}



