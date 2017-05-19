#pragma once 
#include "CoreDEF.h"
#include "Memory/AVFrame.h"


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

    bool            OpenOutputStream    ( const std::string & outputFilePath );
	bool			WriteFrame			( const AVFrameConstPtr & frame );

    void            CloseStream         ();

};

} // videoencoder 

} // bv
