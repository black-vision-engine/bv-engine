#include "gtest/gtest.h"

#include "AVEncoder.h"

static bv::avencoder::AVEncoder enc;

TEST( OpenStream, OpenStream ) 
{
	ASSERT_TRUE( enc.OpenOutputStream( "output.mov" ) );
}

TEST( WriteFrames, WriteFrames )
{
	auto w = 352;
	auto h = 288;

	auto videoData = bv::MemoryChunk::Create( 6 * w * h );

	bv::AVFrameDescriptor desc;
	desc.channels = 0;
	desc.height = h;
	desc.width = w;
	desc.sampleRate = 0;

	auto frame = bv::AVFrame::Create( videoData, nullptr, desc );
	

	//for (auto i = 0; i < 25; i++) {
	//	/* prepare a dummy image */
	//	/* Y */
	//	for (auto y = 0; y < 288; y++) {
	//		for (auto x = 0; x < c->width; x++) {
	//			frame->m_videoData[0][y * frame->linesize[0] + x] = x + y + i * 3;
	//		}
	//	}
	//	/* Cb and Cr */
	//	for ( auto y = 0; y < h / 2; y++) {
	//		for ( auto  x = 0; w / 2; x++) {
	//			frame->m_videoData[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
	//			frame->m_videoData[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
	//		}
	//	}
	//	frame->m_frameTime = i;
	//	/* encode the image */
	//	ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
	//	if (ret < 0) {
	//		fprintf(stderr, "Error encoding frame\n");
	//		exit(1);
	//	}
	//	if (got_output) {
	//		printf("Write frame %3d (size=%5d)\n", i, pkt.size);
	//		fwrite(pkt.data, 1, pkt.size, f);
	//		av_packet_unref(&pkt);
	//	}
	//}
}

int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}



