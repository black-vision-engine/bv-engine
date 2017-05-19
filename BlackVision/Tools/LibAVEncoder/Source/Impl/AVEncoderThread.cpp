#include "AVEncoderThread.h"

#include <thread>

namespace bv { namespace avencoder
{

//**************************************
//
AVEncoderThread::AVEncoderThread    () {}

//**************************************
//
AVEncoderThread::~AVEncoderThread   () {}

//**************************************
//
void AVEncoderThread::Process       ()
{
	printf( "bla\n" );
	std::this_thread::sleep_for( std::chrono::microseconds( 500 ) );
}

} // avencoder
} // bv