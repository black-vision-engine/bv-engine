#include "gtest/gtest.h"

#include "Serialization/ISerializer.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/XML/XMLDeserializer.h"


using namespace bv;


void        SimpleSerializeTest     ( ISerializer & ser );
void        SimpleDeserializeTest   ( IDeserializer & deser );



// ***********************
//
TEST( Serialization_Serializers_JSON, SerializeDeserialize )
{
    {
        SimpleJsonSerializeObject ser;

        SimpleSerializeTest( ser );
        ser.Save( "SerializeDeserializeJSON.json" );
    }

    {
        SimpleJsonDeserializeObject deser;
        deser.LoadFile( "SerializeDeserializeJSON.json" );
        SimpleDeserializeTest( deser );
    }
}

// ***********************
//
TEST( Serialization_Serializers_XML, SerializeDeserialize )
{
    {
        XMLSerializer ser( nullptr );

        SimpleSerializeTest( ser );
        ser.Save( "SerializeDeserializeJSON.xml" );
    }

    {
        XMLDeserializer deser;
        deser.LoadFile( "SerializeDeserializeJSON.xml" );
        SimpleDeserializeTest( deser );
    }
}


// ***********************
//
void            SimpleSerializeTest         ( ISerializer & ser )
{
    ser.EnterChild( "Main" );

        // root attributes
        ser.SetAttribute( "TimeToEat", "true" );
        ser.SetAttribute( "GolemWantsToEat", "false" );


        // Create objects
        ser.EnterChild( "Food" );
            ser.SetAttribute( "Tomatoe", "2" );
        ser.ExitChild();

        ser.EnterChild( "Drinks" );
            ser.SetAttribute( "Wine", "--" );
        ser.ExitChild();

        ser.EnterChild( "Pizzas" );
            ser.SetAttribute( "PizzaAsAService", "PaaS" );
            ser.SetAttribute( "Margherita", "6" );
            ser.SetAttribute( "Funghi", "-30" );
        ser.ExitChild();

        // Create array
        ser.EnterArray( "SweetAnimals" );

            ser.EnterChild( "Animal" );
                ser.SetAttribute( "Name", "Wombat" );
                ser.SetAttribute( "Sweetness", "100" );
            ser.ExitChild();

            ser.EnterChild( "Animal" );
                ser.SetAttribute( "Name", "Alpaca" );
                ser.SetAttribute( "Sweetness", "80" );
            ser.ExitChild();

        ser.ExitChild();

    ser.ExitChild();

}

// ***********************
//
void            SimpleDeserializeTest       ( IDeserializer & deser )
{
    ASSERT_TRUE( deser.EnterChild( "Main" ) );

    // Negative tests
    EXPECT_EQ( deser.GetAttribute( "NonExistingAttribute" ), "" );

    ASSERT_FALSE( deser.EnterChild( "NonExistingObjectOrArray" ) );
    //


    EXPECT_EQ( deser.GetAttribute( "TimeToEat" ), "true" );
    EXPECT_EQ( deser.GetAttribute( "GolemWantsToEat" ), "false" );

    ASSERT_TRUE( deser.EnterChild( "Food" ) );
        EXPECT_EQ( deser.GetAttribute( "Tomatoe" ), "2" );
    ASSERT_TRUE( deser.ExitChild() );

    ASSERT_TRUE( deser.EnterChild( "Drinks" ) );
        EXPECT_EQ( deser.GetAttribute( "Wine" ), "--" );
    ASSERT_TRUE( deser.ExitChild() );

    ASSERT_TRUE( deser.EnterChild( "Pizzas" ) );
        EXPECT_EQ( deser.GetAttribute( "PizzaAsAService" ), "PaaS" );
        EXPECT_EQ( deser.GetAttribute( "Margherita" ), "6" );
        EXPECT_EQ( deser.GetAttribute( "Funghi" ), "-30" );
    ASSERT_TRUE( deser.ExitChild() );

    ASSERT_TRUE( deser.EnterChild( "SweetAnimals" ) );
    ASSERT_TRUE( deser.EnterChild( "Animal" ) );

        EXPECT_EQ( deser.GetAttribute( "Name" ), "Wombat" );
        EXPECT_EQ( deser.GetAttribute( "Sweetness" ), "100" );

    ASSERT_TRUE( deser.NextChild() );

        EXPECT_EQ( deser.GetAttribute( "Name" ), "Alpaca" );
        EXPECT_EQ( deser.GetAttribute( "Sweetness" ), "80" );

    ASSERT_FALSE( deser.NextChild() );

    ASSERT_TRUE( deser.ExitChild() );
    ASSERT_TRUE( deser.ExitChild() );

    // Access Food object for the second time.
    ASSERT_TRUE( deser.EnterChild( "Food" ) );
    EXPECT_EQ( deser.GetAttribute( "Tomatoe" ), "2" );
    ASSERT_TRUE( deser.ExitChild() );

    // Negative tests
    ASSERT_FALSE( deser.EnterChild( "root" ) );
    //

    ASSERT_TRUE( deser.ExitChild() );
}
