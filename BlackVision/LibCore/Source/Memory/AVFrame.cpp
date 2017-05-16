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

} //bv