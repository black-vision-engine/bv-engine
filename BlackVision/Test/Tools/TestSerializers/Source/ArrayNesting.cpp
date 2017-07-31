#include "gtest/gtest.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/XML/XMLDeserializer.h"

using namespace bv;


// ***********************
// Test checks if serializers can deal with two arrays nesting, that means 
// that there're two calls to EnterArray after each other.


void            TestArrayNesting        ( ISerializer & ser );



// ***********************
//
TEST( ArrayNestingJSON, TestSerializers )
{
    JsonSerializeObject ser;
    TestArrayNesting( ser );
    ser.Save( "ArrayNesting.json" );
}

// ***********************
//
TEST( ArrayNestingXML, TestSerializers )
{
    XMLSerializer ser( nullptr );
    TestArrayNesting( ser );
    ser.Save( "ArrayNesting.xml" );
}


// ***********************
//
void            TestArrayNesting        ( ISerializer & ser )
{
    ser.EnterChild( "root" );
    
    ser.EnterArray( "FirstNesting" );
    ser.EnterArray( "SecondNesting" );

    ser.EnterChild( "ArrayObject" );
    ser.SetAttribute( "Attribute", "Value" );
    ser.ExitChild();                    // ArrayObject

    EXPECT_TRUE( ser.ExitChild() );    // SecondNesting
    EXPECT_TRUE( ser.ExitChild() );    // FirstNesting

    EXPECT_TRUE( ser.ExitChild() );    // root
}



