#include "gtest/gtest.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/XML/XMLDeserializer.h"

using namespace bv;



// ***********************
// Computes line numbers and character numbers for given xml.
// Note that tabs are counted as 1 character.
TEST( Serialization_Serializers_XML, QueryLineNumberOfNode )
{
    XMLDeserializer deser;
    deser.LoadFile( "TestAssets/TestSerializers/TestLineNumber.xml" );

    ASSERT_TRUE( deser.EnterChild( "scene" ) );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 1 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 2 );

    ASSERT_TRUE( deser.EnterChild( "lights" ) );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 9 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 3 );

    ASSERT_TRUE( deser.EnterChild( "light" ) );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 10 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 4 );

    ASSERT_TRUE( deser.EnterChild( "params" ) );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 11 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 5 );

    deser.ExitChild();
    deser.ExitChild();
    deser.ExitChild();

    ASSERT_TRUE( deser.EnterChild( "gridlines" ) );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 112 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 3 );

    deser.ExitChild();

    ASSERT_TRUE( deser.EnterChild( "node" ) );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 123 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 3 );

    ASSERT_TRUE( deser.EnterChild( "plugins" ) );
    ASSERT_TRUE( deser.EnterChild( "plugin" ) );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 125 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 5 );

    ASSERT_TRUE( deser.NextChild() );
    ASSERT_TRUE( deser.NextChild() );

    EXPECT_EQ( deser.CurrentLineNumber().Line, 263 );
    EXPECT_EQ( deser.CurrentLineNumber().CharPosition, 5 );
}
