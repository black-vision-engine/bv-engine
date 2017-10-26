#include "gtest/gtest.h"

#include "Exception.h"
#include "ExceptionsHierarchy.h"


using namespace bv;



// ***********************
// Cast exception to the same type. Exception should be not nullptr.
TEST( LibCore_Exceptions, CastToSelf )
{
    SmallExceptionPtr exception = std::make_shared< SmallException >();
    SmallExceptionPtr casted = Cast< SmallException >( exception );
    
    EXPECT_EQ( exception, casted );
    EXPECT_NE( casted, nullptr );
}

// ***********************
// Cast exception to parent class in inheritance hierarchy.
TEST( LibCore_Exceptions, CastToParent )
{
    SmallExceptionPtr exception = std::make_shared< SmallException >();
    RuntimeExceptionPtr casted = Cast< RuntimeException >( exception );

    EXPECT_EQ( exception, casted );
    EXPECT_NE( casted, nullptr );
}

// ***********************
// Cast from base class to derived class.
TEST( LibCore_Exceptions, CastToDerived )
{
    SmallExceptionPtr exception = std::make_shared< SmallException >();
    RuntimeExceptionPtr base = std::static_pointer_cast< RuntimeException >( exception );

    SmallExceptionPtr casted = Cast< SmallException >( base );

    EXPECT_EQ( exception, casted );
    EXPECT_NE( casted, nullptr );
}

// ***********************
// Cast at first to base class and then try to cast to other branch in hierarchy.
// Conversion should fail with nullptr result.
TEST( LibCore_Exceptions, CastToNonRelated )
{
    SmallExceptionPtr exception = std::make_shared< SmallException >();
    RuntimeExceptionPtr base = std::static_pointer_cast< RuntimeException >( exception );

    BigExceptionPtr casted = Cast< BigException >( base );

    EXPECT_EQ( casted, nullptr );
}

// ***********************
// Cast exception to the same type. Exception should be not nullptr.
TEST( LibCore_Exceptions, CastTwoLevels )
{
    ChildishExceptionPtr exception = std::make_shared< ChildishException >();
    RuntimeExceptionPtr casted = Cast< RuntimeException >( exception );

    EXPECT_NE( casted, nullptr );
    EXPECT_EQ( exception, casted );
}


