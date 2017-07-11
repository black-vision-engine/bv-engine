#include "gtest/gtest.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/XML/XMLDeserializer.h"

using namespace bv;


// ***********************
// This test is connected to bug [#148345947] https://www.pivotaltracker.com/story/show/148345947
// Interpolators deserialization accessed elements in array in two passes. JSON deserializer
// pushes current array index on stack always when it enters array object. Index is poped from stack
// when deserialization code exits array object. In deserialization code in CompositeInterpolator::Create
// array is entered once, but deserialization makes two passes. This causes that indicies aren't poped from stack
// and deserialization hangs in infinite loop.


void            TestDeserialization     ( const IDeserializer & deser );


// ***********************
//
TEST( DoublePassArrayAccessJSON, TestSerializers )
{
    JsonDeserializeObject deser;
    deser.LoadFile( "TestAssets/TestSerializers/DoublePassArrayAccess.json" );

    TestDeserialization( deser );
}

// ***********************
//
TEST( DoublePassArrayAccessXML, TestSerializers )
{
    XMLDeserializer deser( "TestAssets/TestSerializers/DoublePassArrayAccess.xml", nullptr );
    TestDeserialization( deser );
}


// ***********************
//
void            TestDeserialization         ( const IDeserializer & deser )
{
    ASSERT_TRUE( deser.EnterChild( "transform" ) );
    ASSERT_TRUE( deser.EnterChild( "interpolators" ) );


    ASSERT_TRUE( deser.EnterChild( "interpolator" ) );
    ASSERT_TRUE( deser.EnterChild( "interpolations" ) );

    // Access first array element.
    EXPECT_TRUE( deser.EnterChild( "interpolation" ) );
    EXPECT_FALSE( deser.NextChild() );

    ASSERT_EQ( deser.GetAttribute( "controlField" ), "interpolator1" );

    ASSERT_TRUE( deser.ExitChild() );

    // Access the same array for the second time.
    EXPECT_TRUE( deser.EnterChild( "interpolation" ) );
    ASSERT_TRUE( deser.ExitChild() );

    ASSERT_TRUE( deser.ExitChild() );       // interpolations

    // If there's bug in code NextChild will be set still to the first element.
    ASSERT_TRUE( deser.NextChild() );       // next interpolator

    ASSERT_TRUE( deser.EnterChild( "interpolations" ) );

    // Access first array element.
    EXPECT_TRUE( deser.EnterChild( "interpolation" ) );
    EXPECT_FALSE( deser.NextChild() );

    ASSERT_EQ( deser.GetAttribute( "controlField" ), "interpolator2" );

}
