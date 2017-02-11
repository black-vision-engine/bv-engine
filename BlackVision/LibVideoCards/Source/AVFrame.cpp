#include "AVFrame.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { namespace videocards {

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