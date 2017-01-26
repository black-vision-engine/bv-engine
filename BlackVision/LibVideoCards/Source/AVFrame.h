#pragma once

#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"
#include "BVTimeCode.h"


namespace bv { namespace videocards {


struct AVFrameDescriptor
{
    UInt32  width;
    UInt32  height;
    UInt32  depth;
    UInt32  channels;
    UInt32  sampleRate;
    bool    timeCodePresent;
    bool    autoGenerateTimecode;
    bool    fieldModeEnabled;
    UInt32  odd;
};

class AVFrame
{
public:

    MemoryChunkConstPtr     m_videoData;
    MemoryChunkConstPtr     m_audioData;
    AVFrameDescriptor       m_desc;
    BVTimeCode              m_TimeCode;

	AVFrame(MemoryChunkConstPtr videoData, MemoryChunkConstPtr audioData, AVFrameDescriptor desc);
	AVFrame();
    ~AVFrame    ();
            
};

DEFINE_PTR_TYPE( AVFrame )

} //videocards
} //bv