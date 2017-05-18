#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"
#include "Threading/StoppableThread.h"

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
