#include "AVFrame.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { namespace videocards {

// ****************************
//
AVFrameConstPtr	AVFrame::Create( const MemoryChunkConstPtr & videoData, const MemoryChunkConstPtr & audioData, const AVFrameDescriptor & desc )
{
	return std::make_shared< const AVFrame >( videoData, audioData, desc );
}

// ****************************
//
AVFrameConstPtr AVFrame::Create()
{
	return std::make_shared< const AVFrame >();
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

} //videocards
} //bv