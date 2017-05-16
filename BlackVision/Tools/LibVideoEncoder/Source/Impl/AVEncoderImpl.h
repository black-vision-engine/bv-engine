#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"

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
    Impl( const Impl & copy );
    const AVEncoder & operator=( const Impl & copy );

public:
    Impl           ();
    virtual ~Impl  ();
};

} // videoencoder 

} // bv
