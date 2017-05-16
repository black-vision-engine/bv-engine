#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"

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
