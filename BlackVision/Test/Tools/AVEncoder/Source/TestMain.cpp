#include "gtest/gtest.h"

#include "AVEncoder.h"

static bv::avencoder::AVEncoder enc;

TEST( OpenStream, OpenStream ) 
{
	ASSERT_TRUE( enc.OpenOutputStream( "output.mov" ) );
}

int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}



