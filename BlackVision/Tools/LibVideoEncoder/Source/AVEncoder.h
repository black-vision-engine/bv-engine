#pragma once 
#include "CoreDEF.h"

namespace bv 
{

namespace avencoder
{

class AVEncoder
{
    class Impl;
    std::unique_ptr< Impl > m_impl;

    AVEncoder( const AVEncoder & copy );
    const AVEncoder & operator=( const AVEncoder & copy );

public:
    AVEncoder           ();
    virtual ~AVEncoder  ();

    bool            OpenVideoStream     ( const std::string & outputFilePath );

};

} // videoencoder 

} // bv
