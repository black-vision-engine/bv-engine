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
bool            AVEncoder::OpenOutputStream     ( const std::string & outputFilePath )
{
    return m_impl->OpenOutputStream( outputFilePath );
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