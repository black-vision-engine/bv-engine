#include "AVEncoderImpl.h"


namespace bv { namespace avencoder
{

//**************************************
//
AVEncoder::Impl::Impl       ()
    : m_encoderThread( new AVEncoderThread() )
{}

//**************************************
//
bool            AVEncoder::Impl::OpenOutputStream       ( const std::string & /*outputFilePath*/, bool /*enableVideo*/, bool /*enableAudio*/ )
{
    assert( false );
    return false;
}

//**************************************
//
void            AVEncoder::Impl::CloseStream            ()
{

}

//**************************************
//
bool            AVEncoder::Impl::WriteFrame             ( const AVFramePtr & /*frame*/ )
{
    assert(false);
    return false;
}

//**************************************
//
AVEncoder::Impl::~Impl      () {}


}
}