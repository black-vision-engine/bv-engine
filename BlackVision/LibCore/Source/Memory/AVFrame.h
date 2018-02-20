#pragma once

#include "MemoryChunk.h"
#include "CoreDEF.h"


namespace bv {

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
    UInt32  sampleRate;         /// FIXME: Video cards interpret this field as number of samples. Check if other parts of code do the same and change this name.
    UInt32  channelDepth;
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
    UInt64                  m_frameTime;
    UInt64                  m_frameNum;
	
	static AVFrameConstPtr	Create( const MemoryChunkConstPtr & videoData, const MemoryChunkConstPtr & audioData, const AVFrameDescriptor & desc );
	static AVFramePtr	    Create();

	AVFrame		( MemoryChunkConstPtr videoData, MemoryChunkConstPtr audioData, AVFrameDescriptor desc );
	AVFrame		();
    ~AVFrame    ();
            
};

UInt32  ConvertAudioSampleTypeToSampleSize( AudioSampleType ast );

} //bv