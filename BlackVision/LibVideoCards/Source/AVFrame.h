#pragma once

#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"
#include "BVTimeCode.h"


namespace bv { namespace videocards {

enum class AudioSampleType : int
{
    AV_SAMPLE_FMT_S32 = 0,
    AV_SAMPLE_FMT_S16,
    AV_SAMPLE_FMT_U8,

    AV_SAMPLE_FMT_TOTAL,
};

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

class AVFrame;
DEFINE_PTR_TYPE( AVFrame )
DEFINE_CONST_PTR_TYPE( AVFrame )

class AVFrame
{
public:

    MemoryChunkConstPtr     m_videoData;
    MemoryChunkConstPtr     m_audioData;
    AVFrameDescriptor       m_desc;
    BVTimeCode              m_TimeCode;
	
	static AVFrameConstPtr	Create( const MemoryChunkConstPtr & videoData, const MemoryChunkConstPtr & audioData, const AVFrameDescriptor & desc );
	static AVFramePtr	    Create();

	AVFrame		( MemoryChunkConstPtr videoData, MemoryChunkConstPtr audioData, AVFrameDescriptor desc );
	AVFrame		();
    ~AVFrame    ();
            
};



} //videocards
} //bv