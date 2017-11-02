#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"


using namespace bv;


// ========================================================================= //
// Test fixture for templated integer types
// ========================================================================= //



// ***********************
//
template< class Type >
class LibCore_String2T_Integer : public testing::Test
{
protected:
    LibCore_String2T_Integer() {};
};

typedef testing::Types< UInt64, Int64, UInt32, Int32, UInt16, Int16, UInt8, Int8 > TypesList;

TYPED_TEST_CASE( LibCore_String2T_Integer, TypesList );


// ========================================================================= //
// Tests for different integer types
// ========================================================================= //


// ***********************
// Convert int from valid input data.
TYPED_TEST( LibCore_String2T_Integer, ValidInput )
{
    Expected< TypeParam > intExp = SerializationHelper::String2T< TypeParam >( "123" );
    TypeParam intDef = SerializationHelper::String2T< TypeParam >( "123", 0 );

    EXPECT_TRUE( intExp.IsValid() );
    EXPECT_EQ( intExp.GetVal(), 123 );
    EXPECT_EQ( intDef, 123 );
}


