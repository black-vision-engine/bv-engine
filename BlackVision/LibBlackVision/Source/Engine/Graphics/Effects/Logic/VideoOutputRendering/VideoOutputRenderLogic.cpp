#include "stdafx.h"

#include "VideoOutputRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectFactory.h"
#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "VideoCardManager.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
VideoOutputRenderLogic::VideoOutputRenderLogic          ( unsigned int height, bool interlaceOdd )
    : m_height( height )
    , m_videoOutputEffect( nullptr )
    , m_interlaceOdd( interlaceOdd )
    , m_videoFrame( nullptr )
    , m_vidTargets( 2 )
{
    m_videoOutputEffect = CreateFullscreenEffectInstance( FullscreenEffectType::FET_VIDEO_OUTPUT );
}

// *********************************
//
VideoOutputRenderLogic::~VideoOutputRenderLogic         ()
{
}

// *********************************
//
void                            VideoOutputRenderLogic::Render          ( RenderTarget * videoRenderTarget, RenderTarget * curFrameRenderTarget, RenderTarget * prevFrameRenderTarget, RenderLogicContext * ctx )
{
    auto render = renderer( ctx );

    if( !m_videoOutputEffect->GetRenderTarget( 0 ) )
    {
        //FIXME: fake initialize curFrameRenderTarget (as it will be used by the effect a moment after this)
        //FIXME: this suxx but there is an assert in the renderer and maybe it makes sense to leave it there
        render->Enable  ( prevFrameRenderTarget );
        render->Disable ( prevFrameRenderTarget );

        m_videoOutputEffect->SetRenderTarget( 0, curFrameRenderTarget );
        m_videoOutputEffect->SetRenderTarget( 1, prevFrameRenderTarget );

        m_videoOutputEffect->SetValue( "height", (float) m_height );
        m_videoOutputEffect->SetValue( "startEven", (int) !m_interlaceOdd );
    }

    m_vidTargets[ 0 ] = curFrameRenderTarget;
    m_vidTargets[ 1 ] = prevFrameRenderTarget;

    //FIXME: regquired - effect instance assumes that it is bound.... somehow deal with it in composite effects
    render->Enable    ( videoRenderTarget );

    m_videoOutputEffect->UpdateInputRenderTargets( m_vidTargets );
    m_videoOutputEffect->Render( videoRenderTarget, ctx );

    render->Disable   ( videoRenderTarget );
}

// *********************************
//

unsigned short sine_wave_const_48[] = { 0x0000, 0x01A8, 0x0349, 0x04DB, 0x0658, 0x07BA,
0x08FA, 0x0A12, 0x0AFE, 0x0BBA, 0x0C43, 0x0C96,
0x0CB1, 0x0C96, 0x0C43, 0x0BBA, 0x0AFE, 0x0A12,
0x08FA, 0x07BA, 0x0658, 0x04DB, 0x0349, 0x01A8,
0x0000, 0xFE58, 0xFCB7, 0xFB25, 0xF9A8, 0xF846,
0xF706, 0xF5EE, 0xF502, 0xF446, 0xF3BD, 0xF36A,
0xF34F, 0xF36A, 0xF3BD, 0xF446, 0xF502, 0xF5EE,
0xF706, 0xF846, 0xF9A8, 0xFB25, 0xFCB7, 0xFE58 };


void Fill48(USHORT* pAudio16, UINT32 Samples, UINT32 Channels)
{
	USHORT data = 0;
	USHORT end = (USHORT)(Samples / 48);
	USHORT Bytes = 0;


	for (USHORT i = 0; i<end; i++)
	{
		for (USHORT k = 0; k<48; k++)
		{
			data = sine_wave_const_48[k];
			// 16 channels
			/*if (Channels > 0) { pAudio16[i * 48 * 2 + k + 0] = data; }
			if (Channels > 1) { pAudio16[i * 48 * 2 + k + 1] = data; }*/
			if (Channels > 0) { *pAudio16 = data; pAudio16++; }
			if (Channels > 1) { *pAudio16 = data; pAudio16++; }
			if (Channels > 2) { *pAudio16 = data; pAudio16++; }
			if (Channels > 3) { *pAudio16 = data; pAudio16++; }
			if (Channels > 4) { *pAudio16 = data; pAudio16++; }
			if (Channels > 5) { *pAudio16 = data; pAudio16++; }
			if (Channels > 6) { *pAudio16 = data; pAudio16++; }
			if (Channels > 7) { *pAudio16 = data; pAudio16++; }
			if (Channels > 8) { *pAudio16 = data; pAudio16++; }
			if (Channels > 9) { *pAudio16 = data; pAudio16++; }
			if (Channels > 10) { *pAudio16 = data; pAudio16++; }
			if (Channels > 11) { *pAudio16 = data; pAudio16++; }
			if (Channels > 12) { *pAudio16 = data; pAudio16++; }
			if (Channels > 13) { *pAudio16 = data; pAudio16++; }
			if (Channels > 14) { *pAudio16 = data; pAudio16++; }
			if (Channels > 15) { *pAudio16 = data; pAudio16++; }
			Bytes++;
		}
	}
	int a = 0;
	{a;}
}


void    VideoOutputRenderLogic::VideoFrameRendered      ( RenderTarget * videoRenderTarget, RenderLogicContext * ctx )
{
	renderer( ctx )->ReadColorTexture( 0, videoRenderTarget, m_videoFrame );
 
	videocards::BVVideoFramePtr frame = videocards::BVVideoFramePtr(new videocards::BVVideoFrame());
	frame->m_VideoData = m_videoFrame->GetData();
	
	int AudioChannels = 2;  // liczba kana³ów audio

	frame->m_TimeCode.h = 10;
	frame->m_TimeCode.m = 22;
	frame->m_TimeCode.s = 33;
	frame->m_TimeCode.frame = 12;

	frame->m_FrameInformation.m_depth = 4;
	frame->m_FrameInformation.m_width = 1920;
	frame->m_FrameInformation.m_height = 1080;

	frame->m_FrameInformation.m_AudioPresent = true;
	frame->m_FrameInformation.m_IsFieldMode = true;
	frame->m_FrameInformation.m_TimeCodePresent = true;
	frame->m_FrameInformation.m_AutoGenerateTimecode = true; // jesli ==true timecode sam sie bedzie generowa³ w fifoplayback na podstawie liczby wyswietlonych ramek
	frame->m_FrameInformation.m_VideoAspect = 1.777778f;
	frame->m_FrameInformation.m_AudioSamplesPerFrame = 1920; // poprawne dla HD 50i, SD 50i   dla dowolnego formatu mozna pobraæ z funkcji Bluefisha GetNumberOfAudioSamplesPerFrame
	frame->m_FrameInformation.m_AudioChannelsCount = AudioChannels;

	int AudioBufferSize = AudioChannels * 2002*4; // 2 channels (eg. stereo) 2002 jest maksymaln¹ wartoœci¹  // max 4 bajty g³êbi
	unsigned char *mem_dst = new unsigned char[AudioBufferSize];  // pewnie nie ma co tutaj tego za kazdym razem tworzyæ... tylko sk¹dœ pobraæ
	memset(mem_dst, 0, AudioBufferSize);
	Fill48((USHORT*)mem_dst, frame->m_FrameInformation.m_AudioSamplesPerFrame, AudioChannels);  // zape³nia bufor sygna³em testowym 1kHz

	frame->m_AudioData = MemoryChunkConstPtr(new MemoryChunk((char*)mem_dst, AudioBufferSize));
	
    videocards::VideoCardManager::Instance().QueueFrame( frame );
}

// *********************************
//
void    VideoOutputRenderLogic::SetChannelMapping       ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    if( m_videoOutputEffect )
    {
        auto val = ( ( aIdx & 0x3 ) << 6 ) | ( ( bIdx & 0x3 ) << 4 ) | ( ( gIdx & 0x3 ) << 2 ) | ( ( rIdx & 0x3 ) << 0 );

        m_videoOutputEffect->SetValue( "channelMask", val );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetOverwriteAlpha       ( bool overwriteAlpha )
{
    if( m_videoOutputEffect )
    {
        m_videoOutputEffect->SetValue( "overwriteAlpha", overwriteAlpha );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetAlpha                ( float alpha )
{
    if( m_videoOutputEffect )
    {
        m_videoOutputEffect->SetValue( "alpha", alpha );
    }
}

// *********************************
//
void    VideoOutputRenderLogic::SetHeight               ( int height )
{
    if( m_videoOutputEffect )
    {
        m_videoOutputEffect->SetValue( "height", height );
    }
}

// *********************************
//
Texture2DPtr    VideoOutputRenderLogic::GetLastVideoFrame  ()
{
    return m_videoFrame;
}

} //bv
