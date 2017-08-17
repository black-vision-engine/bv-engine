#include "AVEncoder.h"
#include "Impl/AVEncoderImpl.h"

namespace bv { namespace avencoder {

//**************************************
//
AVEncoder::AVEncoder( UInt32 bufferSize )
    : m_impl( new Impl( bufferSize ) )
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
    return m_impl->OpenOutputStream( outputFilePath, vOps, aOps, enableVideo, enableAudio );
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