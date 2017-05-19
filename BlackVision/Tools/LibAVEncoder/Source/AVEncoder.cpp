#include "AVEncoder.h"
#include "Impl/AVEncoderImpl.h"

namespace bv { namespace avencoder {

//**************************************
//
AVEncoder::AVEncoder()
    : m_impl( new Impl() )
{}

//**************************************
//
AVEncoder::~AVEncoder() 
{
}

//**************************************
//
bool            AVEncoder::OpenOutputStream     ( const std::string & outputFilePath, VideoOptions vOps, AudioOptions aOps, bool enableVideo, bool enableAudio )
{
    return m_impl->OpenOutputStream( outputFilePath, vOps, aOps, enableAudio, enableVideo );
}

//**************************************
//
bool			AVEncoder::WriteFrame			( const AVFrameConstPtr & frame )
{
	return m_impl->WriteFrame( frame );
}

//**************************************
//
void            AVEncoder::CloseStream         ()
{
    m_impl->CloseStream();
}

} // videoencoder 
} // bv