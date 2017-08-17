#pragma once 
#include "CoreDEF.h"
#include "Memory/AVFrame.h"


namespace bv 
{

namespace avencoder
{

struct VideoOptions
{
    Int32 width;
    Int32 height;
    Int64 bitRate;
    Int32 frameRate;
};

struct AudioOptions
{
    Int32 numChannels;
    Int64 bitRate;
    Int32 sampleRate;
    bv::AudioSampleType sampleType;
};


class AVEncoder
{
    class Impl;
    std::unique_ptr< Impl > m_impl;

    AVEncoder( const AVEncoder & copy );
    const AVEncoder & operator=( const AVEncoder & copy );

public:
    AVEncoder           ( UInt32 bufferSize = 128 );
    virtual ~AVEncoder  ();

    bool            OpenOutputStream    (   const std::string & outputFilePath,
                                            VideoOptions vOps,
                                            AudioOptions aOps,
                                            bool enableVideo = true,
                                            bool enableAudio = true  );
	bool			WriteFrame			( const AVFrameConstPtr & frame );

    void            CloseStream         ();

};

} // videoencoder 

} // bv
