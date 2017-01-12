#include "stdafx.h"

#include "VideoOutput.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

    /*
    Basic video output algo description
    */
    //auto prevRt = m_offscreenDisplay->GetCurrentFrameRenderTarget();
    //auto ctx = GetContext( renderer, audioRenderer );

    //// FIXME: nrl - implement screenshot logic in the nrl model

    //if( m_displayVideoCardPreview )
    //{
    //    auto videoRt    = m_offscreenDisplay->GetVideoRenderTarget          ();
    //    auto curFrameRt = m_offscreenDisplay->GetCurrentFrameRenderTarget   ();
    //    auto prvFrameRt = m_offscreenDisplay->GetPreviousFrameRenderTarget  ();

    //    m_videoOutputRenderLogic->Render( videoRt, curFrameRt, prvFrameRt, ctx );

    //    prevRt = videoRt;
    //}

    //BlitToPreview( prevRt, ctx );

    //if( video card enabled )
    //{
    //    // FIXME: nrl - audio should be handled based on this implementation
    //    OnVideoFrameRendered( ctx );
    //}
// **************************
//
void    VideoOutput::HandleFrame( NRenderContext * ctx, const RenderResult * result )
{
    // FIXME: nrl - implement
 //   auto rt = m_offscreenDisplay->GetVideoRenderTarget();
	//{
	//	HPROFILER_SECTION("Shared Memory Copy", PROFILER_THREAD1);
	//	if (m_enableSharedMemory)
	//	{
	//		if(m_videoOutputRenderLogic->GetLastVideoFrame()!=nullptr)
	//			m_sharedMemoryVideoBuffer->DistributeFrame(m_videoOutputRenderLogic->GetLastVideoFrame());
	//	}
	//}

	//{
	//	HPROFILER_SECTION("Wait for Sync", PROFILER_THREAD1);

	//	m_videoOutputRenderLogic->VideoFrameRendered(rt, ctx);
	//}
    // TODO: implement

	//{
	//	HPROFILER_SECTION("ReadColorTexture", PROFILER_THREAD1);
	//	renderer(ctx)->ReadColorTexture(0, videoRenderTarget, m_videoFrame);
	//}

	//		videocards::AVFrameDescriptor desc;
	//		desc.width = m_videoFrame->GetWidth();
	//	desc.height = m_videoFrame->GetHeight();
	//	desc.depth = TextureUtils::Channels(m_videoFrame->GetFormat());

	//	desc.channels = audioRenderer(ctx)->GetChannels();
	//	desc.sampleRate = audioRenderer(ctx)->GetFrequency() / m_fps;

	//	auto audioSize = desc.sampleRate * desc.channels * audioRenderer(ctx)->GetChannelDepth();
	//	if (m_audioData->Size() != audioSize)
	//	{
	//		m_audioData = MemoryChunk::Create(audioSize);
	//	}

	//	audioRenderer(ctx)->GetBufferedData(m_audioData);

	//	desc.fieldModeEnabled = true;
	//	desc.timeCodePresent = true;
	//	desc.autoGenerateTimecode = true;

	//	auto frame = std::make_shared< videocards::AVFrame >(m_videoFrame->GetData(), m_audioData, desc);

	//	frame->m_TimeCode.h = 10;
	//	frame->m_TimeCode.m = 22;
	//	frame->m_TimeCode.s = 33;
	//	frame->m_TimeCode.frame = 12;

	//	{
	//		HPROFILER_SECTION("QueueFrame", PROFILER_THREAD1);
	//		videocards::VideoCardManager::Instance().QueueFrame(frame);
	//	}

    { ctx; result; }
}

} //nrl
} //bv
