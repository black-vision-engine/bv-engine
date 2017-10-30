#include "gtest/gtest.h"

#include "Expected.h"

using namespace bv;

// ========================================================================= //
// Non-String types
// ========================================================================= //

// ***********************
//
Expected< int >         GetInt      () 
{
    return 42;
}

// ***********************
//
TEST( LibCore_Expected, CreateValInt )
{
    auto ret = GetInt();
    EXPECT_TRUE( ret.IsValid() );
    EXPECT_TRUE( ret == 42 );
}

// ***********************
//
Expected< int >         DontGetInt      ()
{
    return "hwdp";
//    return Expected< int >::fromError( "hwdp" ); // works also ;)
}

// ***********************
//
TEST( LibCore_Expected, CreateErrorFromString )
{
    auto ret = DontGetInt();
    EXPECT_FALSE( ret.IsValid() );
    EXPECT_TRUE( ret.GetErrorReason() == "hwdp" );
}



// ========================================================================= //
// String type
// ========================================================================= //


// ***********************
//
Expected< std::string > GetStringOrNot( bool valid ) {
    if( valid )
        return "hwdp";
    else
        return Expected< std::string >::fromError( "user didn't want string :(" );
}

// ***********************
//
TEST( LibCore_Expected, CreateErrorString )
{
    auto ret = GetStringOrNot( false );
    EXPECT_FALSE( ret.IsValid() );
    EXPECT_TRUE( ret.GetErrorReason() == "user didn't want string :(" );
}

TEST( LibCore_Expected, CreateValString ) {
    auto ret = GetStringOrNot( true );
    EXPECT_TRUE( ret.IsValid() );
    EXPECT_TRUE( ret == "hwdp" );
}
