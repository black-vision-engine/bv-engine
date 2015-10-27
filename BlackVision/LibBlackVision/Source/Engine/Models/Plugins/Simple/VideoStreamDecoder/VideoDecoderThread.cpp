#include "VideoDecoderThread.h"

#include <Windows.h>

namespace bv
{

// *******************************
//
VideoDecoderThread::VideoDecoderThread     ( IVideoDecoder * decoder )
	: m_decoder( decoder )
{
}

// *******************************
//
VideoDecoderThread::~VideoDecoderThread    ()
{
}

// *******************************
//
void				VideoDecoderThread::Run			()
{
	//FIXME
	UInt32 currFrame = 0;
	while( currFrame >= 0 )
	{
		if( m_decoder->GetNextFrameData() )
		{
			//FIXME
			Sleep( 33 );
		}
	}
}

} //bv
