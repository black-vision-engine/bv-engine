#include "gtest/gtest.h"

#include "AVEncoder.h"



static bv::avencoder::AVEncoder enc;



// ***********************
//
TEST( Tools_AVEncoder, OpenStream ) 
{
    bv::avencoder::VideoOptions vOps;
    vOps.bitRate = 40000;
    vOps.frameRate = 25;
    vOps.width = 1920;
    vOps.height = 1080;
    bv::avencoder::AudioOptions aOps = bv::avencoder::AudioOptions();

	ASSERT_TRUE( enc.OpenOutputStream( "output.mov", vOps, aOps ) );
}

// ***********************
//
TEST( Tools_AVEncoder, WriteFrames )
{
	auto w = 1920;
	auto h = 1080;

	auto videoData = bv::MemoryChunk::Create( 4 * w * h );

	bv::AVFrameDescriptor desc;
	desc.channels = 0;
	desc.height = h;
	desc.width = w;
	desc.numSamples = 0;

	auto frame = bv::AVFrame::Create( videoData, nullptr, desc );
	
    

	for (auto i = 0; i < 250; i++) {
        memset( videoData->GetWritable(), i, videoData->Size() );


        enc.WriteFrame( frame );
    }

    enc.CloseStream();
}


// ***********************
//
int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}



