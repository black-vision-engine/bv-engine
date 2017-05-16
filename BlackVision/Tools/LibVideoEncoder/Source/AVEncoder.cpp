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
bool            AVEncoder::OpenVideoStream     ( const std::string & outputFilePath )
{
    return m_impl->OpenVideoStream( outputFilePath );
}

} // videoencoder 
} // bv