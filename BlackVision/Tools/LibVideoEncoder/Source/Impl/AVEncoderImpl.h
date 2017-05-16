#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"
#include "AVEncoderThread.h"

#pragma warning(push)
#pragma warning(disable : 4244) // warning C4244: 'return' : conversion from 'int' to 'uint8_t', possible loss of data

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#pragma warning(pop)

namespace bv 
{

namespace avencoder
{

class AVEncoder;
class AVEncoder::Impl;


class AVEncoder::Impl
{
    std::unique_ptr< AVEncoderThread > m_encoderThread;

private:
    Impl( const Impl & copy );
    const AVEncoder & operator=( const Impl & copy );

public:
    Impl           ();
    virtual ~Impl  ();

    bool            OpenVideoStream     ( const std::string & outputFilePath );
};

} // videoencoder 

} // bv
