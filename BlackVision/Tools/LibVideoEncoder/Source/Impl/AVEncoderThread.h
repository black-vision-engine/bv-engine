#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"
#include "Threading/StoppableThread.h"

#pragma warning(push)
#pragma warning(disable : 4244) // warning C4244: 'return' : conversion from 'int' to 'uint8_t', possible loss of data

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#pragma warning(pop)

namespace bv 
{

namespace avencoder
{


class AVEncoderThread : public StoppableThread
{
    AVEncoderThread( const AVEncoderThread & copy );
    const AVEncoderThread & operator=( const AVEncoderThread & copy );

    virtual void				Process () override;

public:
    AVEncoderThread           ();
    virtual ~AVEncoderThread  ();
};

} // videoencoder 

} // bv
