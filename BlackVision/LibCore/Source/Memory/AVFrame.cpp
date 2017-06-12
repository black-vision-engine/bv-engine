#include "stdafxCore.h"

#include "Memory/AVFrame.h"


namespace bv {

// ****************************
//
AVFrameConstPtr	AVFrame::Create( const MemoryChunkConstPtr & videoData, const MemoryChunkConstPtr & audioData, const AVFrameDescriptor & desc )
{
	return std::make_shared< const AVFrame >( videoData, audioData, desc );
}

// ****************************
//
AVFramePtr AVFrame::Create()
{
	return std::make_shared< AVFrame >();
}

// ****************************
//
AVFrame::AVFrame(MemoryChunkConstPtr videoData, MemoryChunkConstPtr audioData, AVFrameDescriptor desc)
	: m_videoData(videoData)
	, m_audioData(audioData)
	, m_desc(desc)
{
}

AVFrame::AVFrame()
{
}

// ****************************
//
AVFrame::~AVFrame     ()
{
}

// ****************************
//
UInt32  ConvertAudioSampleTypeToSampleSize( AudioSampleType ast )
{
    switch( ast )
    {
    case AudioSampleType::AV_SAMPLE_FMT_S16:
        return 2;
    case AudioSampleType::AV_SAMPLE_FMT_S32:
        return 4;
    case AudioSampleType::AV_SAMPLE_FMT_U8:
        return 1;
    default:
        assert( false );
        return 0;
    }
}

} //bv