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
bool            AVEncoder::Impl::OpenVideoStream     ( const std::string & /*outputFilePath*/ )
{
    assert( false );
    return false;
}

//**************************************
//
AVEncoder::Impl::~Impl      () {}


}
}