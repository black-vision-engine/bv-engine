#include "FifoCapture.h"

#include "BlueFish/inc/BlueHancUtils.h"
#include "FrameSchedule.h"

#include <process.h>

#include "Serialization/ConversionHelper.h"
#include "UseLoggerVideoModule.h"


namespace bv {
namespace videocards {
namespace bluefish
{

// FIXME: What does this number mean ??
#define MAX_HANC_BUFFER_SIZE (256*1024)


extern struct blue_videomode_info gVideoModeInfo[];

CFifoCapture::CFifoCapture() :
	m_pSDK(NULL),
	m_iDevices(0),
	m_nIsAttached(0),
	m_nVideoMode(VID_FMT_INVALID),
	m_InvalidVideoModeFlag(VID_FMT_INVALID),
	m_pFifoBuffer(NULL),
	m_hThread(0),
	m_nThreadStopping(TRUE),
    m_numAudioChannels( 2 ),
    m_audioFormat( AUDIO_CHANNEL_16BIT ),
    m_samplesFrequency( 48000 )
{
	m_pSDK = BlueVelvetFactory4();
	//if(!m_pSDK)
	//	cout << "No Bluefish SDK" << endl;

	if(m_pSDK)
	{
		m_pSDK->device_enumerate(m_iDevices);
		//if(!m_iDevices)
		//	cout << "No Bluefish card" << endl;
	}
}

CFifoCapture::~CFifoCapture()
{
	if(m_pSDK)
	{
		if(m_nIsAttached)
			m_pSDK->device_detach();
		BlueVelvetDestroy(m_pSDK);
	}
	m_pSDK = NULL;

}

// ***********************
//
ReturnResult            CFifoCapture::Init      ( BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, CFifoBuffer* pFifoBuffer )
{
	ULONG FieldCount = 0;
	VARIANT varVal;

	if(m_nIsAttached)
	{
		m_pSDK->device_detach();
		m_nIsAttached = 0;
	}

	if(CardNumber <= 0 || CardNumber > m_iDevices)
	{
		return ReturnResult::fromError( "Card " + Convert::T2String( CardNumber ) + " not available; maximum card number is: " + Convert::T2String( m_iDevices ) );
	}

	m_pSDK->device_attach(CardNumber, 0);
	m_nIsAttached = 1;

	m_iCardType = m_pSDK->has_video_cardtype();

	//Get number of supported inputs and outputs
	varVal.ulVal = 0;
	m_pSDK->QueryCardProperty(CARD_FEATURE_STREAM_INFO, varVal);
	
	unsigned int nInputStreams = CARD_FEATURE_GET_SDI_INPUT_STREAM_COUNT(varVal.ulVal);
	if(!nInputStreams)
	{
		m_pSDK->device_detach();
		m_nIsAttached = 0;

        return "Card does not support input channels";
	}

	varVal.vt = VT_UI4;
	m_pSDK->QueryCardProperty(INVALID_VIDEO_MODE_FLAG, varVal);
	m_InvalidVideoModeFlag = varVal.ulVal;
	
    m_videoChannel = VideoChannel;
	varVal.ulVal = VideoChannel;
	m_pSDK->SetCardProperty(DEFAULT_VIDEO_INPUT_CHANNEL, varVal);

	m_pSDK->wait_input_video_synch(UPD_FMT_FIELD, FieldCount); //synchronise with the card before querying VIDEO_INPUT_SIGNAL_VIDEO_MODE
	varVal.vt = VT_UI4;
	m_pSDK->QueryCardProperty(VIDEO_INPUT_SIGNAL_VIDEO_MODE, varVal);
	if(varVal.ulVal >= m_InvalidVideoModeFlag)
	{
		m_pSDK->device_detach();
		m_nIsAttached = 0;

        return "No valid input signal";
	}
	m_nVideoMode = varVal.ulVal;
	//cout << "Video Input mode: " << gVideoModeInfo[m_nVideoMode].strVideoModeFriendlyName.c_str() << endl;

	m_nUpdateFormat = UpdateFormat;
	varVal.ulVal = m_nUpdateFormat;
	m_pSDK->SetCardProperty(VIDEO_INPUT_UPDATE_TYPE, varVal);

	m_nMemoryFormat = MemoryFormat;
	varVal.ulVal = m_nMemoryFormat;
	m_pSDK->SetCardProperty(VIDEO_INPUT_MEMORY_FORMAT, varVal);

	varVal.ulVal = VIDEO_ENGINE_FRAMESTORE;	//do not set it to VIDEO_ENGINE_CAPTURE as this will automatically do a playthrough and this will conflict with our playback thread
	m_pSDK->SetCardProperty(VIDEO_INPUT_ENGINE, varVal);

    varVal.ulVal = ImageOrientation_VerticalFlip;
    m_pSDK->SetCardProperty( VIDEO_IMAGE_ORIENTATION, varVal );

	GoldenSize = BlueVelvetGolden(m_nVideoMode, m_nMemoryFormat, m_nUpdateFormat);
	BytesPerLine = BlueVelvetLineBytes(m_nVideoMode, m_nMemoryFormat);
	m_pFifoBuffer = pFifoBuffer;
	m_pFifoBuffer->Init(4, GoldenSize, BytesPerLine);

    InitAudio();

	::ZeroMemory(&m_Overlap, sizeof(m_Overlap));
	m_Overlap.hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );

	return Result::Success();
}

// ***********************
//
ReturnResult            CFifoCapture::InitDualLink  ( BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, CFifoBuffer* pFifoBuffer )
{
    auto result = Init( CardNumber, VideoChannel, UpdateFormat, MemoryFormat, pFifoBuffer );
    if( result.IsValid() )
    {
	    VARIANT varVal;

        //enable dual link input (sets up the routing automatically to SDI A -> Video input channel A link A, SDI B-> Video input channel A link B
	    varVal.ulVal = 1; //turn dual link on
	    m_pSDK->SetCardProperty(VIDEO_DUAL_LINK_INPUT, varVal); //sets routing automatically

	    varVal.vt = VT_UI4;
	    varVal.ulVal = Signal_FormatType_4224;	//select format type
	    //varVal.ulVal = Signal_FormatType_444_10BitSDI;
	    //varVal.ulVal = Signal_FormatType_444_12BitSDI;
	    m_pSDK->SetCardProperty(VIDEO_DUAL_LINK_INPUT_SIGNAL_FORMAT_TYPE, varVal);

        return Result::Success();
    }

	return result;
}

// ***********************
//
void                    CFifoCapture::InitAudio         ()
{
    VARIANT varVal;
    varVal.vt = VT_UI4;

    varVal.ulVal = BLUE_AUDIO_EMBEDDED;
    m_pSDK->SetCardProperty( AUDIO_INPUT_PROP, varVal );
}

// ***********************
//
void                    CFifoCapture::RouteChannel      ( ULONG Source, ULONG Destination, ULONG LinkType )
{
	VARIANT varVal;
	varVal.vt = VT_UI4;

	varVal.ulVal = EPOCH_SET_ROUTING(Source, Destination, LinkType);
	m_pSDK->SetCardProperty(MR2_ROUTING, varVal);
}

// ***********************
//
ReturnResult            CFifoCapture::InitThread        ()
{
	unsigned int ThreadId = 0;

	if(m_hThread)
	{
		//cout << "Capture Thread already started" << endl;
		return Result::Success();
	}

	//cout << "Starting Capture Thread..." << endl;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, &CaptureThread, this, CREATE_SUSPENDED, &ThreadId);
	if(!m_hThread)
	{
		//cout << "Error starting Capture Thread" << endl;
        return "Error starting Capture Thread.";
	}

	m_nThreadStopping = FALSE;
	SetThreadPriority(m_hThread, THREAD_PRIORITY_TIME_CRITICAL);
	//cout << "...done." << endl;
    return Result::Success();
}

// ***********************
//
void                    CFifoCapture::StartThread()
{
	ResumeThread(m_hThread);
}

// ***********************
//
void                    CFifoCapture::SuspendThread()
{
	::SuspendThread(m_hThread);
}

// ***********************
//
void                    CFifoCapture::StopThread()
{
	DWORD dw = 0;

	if(m_hThread)
	{
        //cout << "Stopping Capture Thread..." << endl;

        m_nThreadStopping = TRUE;

        std::shared_ptr< CFrame > frame;
        m_pFifoBuffer->TryPopFrame( frame );

		dw = WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
        m_hThread = NULL;
	}
	else
	{
		m_hThread = NULL;
		//cout << "...done. Now You need to initialize from scratch" << endl;
	}
	return;
}

// ***********************
//
unsigned int __stdcall CFifoCapture::CaptureThread(void * pArg)
{
    CFifoCapture* pThis = ( CFifoCapture* )pArg;
    BOOL bFirstFrame = TRUE;

    // Init audio
    // In field mode we get each second frame doubled audio size.
    UInt32 audioBufferSize = 2 * GetSampleSize( pThis->m_audioFormat ) * pThis->m_numAudioChannels * pThis->m_samplesFrequency / 50;

    unsigned char* pHancBuffer = ( unsigned char* )VirtualAlloc( NULL, MAX_HANC_BUFFER_SIZE, MEM_COMMIT, PAGE_READWRITE );
    VirtualLock( pHancBuffer, MAX_HANC_BUFFER_SIZE );

    hanc_decode_struct hancInfo;
    hancInfo.audio_ch_required_mask = GetAudioChannelsMask( pThis->m_numAudioChannels );
    hancInfo.type_of_sample_required = pThis->m_audioFormat;
    hancInfo.max_expected_audio_sample_count = 2002;        // FIXME: What is this?

    unsigned int nAudioChannelInfo = 0;

    // Prepare chunks for input. Chunks will be reused between frames.
    Reusable< CFramePtr > frames = CreateReusableVideoChunks( pThis->GoldenSize, pThis->BytesPerLine, audioBufferSize, 3 );


    const int numBuffers = 5;
    FrameSchedule scheduler( numBuffers );

    scheduler.SyncToNextFrameInterrupt( pThis );
    scheduler.SyncToOddFrame( pThis );

    scheduler.ScheduleNextFrame( pThis );
    scheduler.NextFrame();

    scheduler.SyncToNextFrameInterrupt( pThis );
    scheduler.ScheduleNextFrame( pThis );
    scheduler.NextFrame();


    while( !pThis->m_nThreadStopping )
    {
        auto pFrame = frames.GetNext();

        scheduler.SyncToNextFrameInterrupt( pThis );
        scheduler.ScheduleNextFrame( pThis );


        VARIANT varVal;
        pThis->m_pSDK->QueryCardProperty( VIDEO_INPUT_SIGNAL_VIDEO_MODE, varVal );	//check if the video signal was valid for the last frame (until wait_input_video_synch() returned)


        if( varVal.ulVal < pThis->m_InvalidVideoModeFlag )
        {   
            if( scheduler.IsAudioFrame() )
            {
                pThis->m_pSDK->system_buffer_read_async( ( unsigned char* )pFrame->m_pBuffer, pFrame->m_nSize, NULL, BlueImage_HANC_DMABuffer( scheduler.DoneID, BLUE_DATA_IMAGE ) );
                pThis->m_pSDK->system_buffer_read_async( pHancBuffer, MAX_HANC_BUFFER_SIZE, NULL, BlueImage_HANC_DMABuffer( scheduler.DoneID, BLUE_DATA_HANC ) );

                hancInfo.audio_pcm_data_ptr = pFrame->m_pAudioBuffer;
                hancInfo.raw_custom_anc_pkt_data_ptr = nullptr;
                hancInfo.audio_input_source = AUDIO_INPUT_SOURCE_EMB;

                hanc_decoder_ex( pThis->m_iCardType, ( UINT32* )pHancBuffer, &hancInfo );

                nAudioChannelInfo = QueryChannelInfo( pThis->m_pSDK );
                pFrame->m_desc.numSamples = hancInfo.no_audio_samples / pThis->m_numAudioChannels;
                pFrame->m_desc.channels = pThis->m_numAudioChannels;
                pFrame->m_desc.channelDepth = GetSampleSize( pThis->m_audioFormat );
            }
            else
            {
                pThis->m_pSDK->system_buffer_read_async( ( unsigned char* )pFrame->m_pBuffer, pFrame->m_nSize, NULL, BlueImage_DMABuffer( scheduler.DoneID, BLUE_DATA_IMAGE ) );

                pFrame->m_desc.numSamples = 0;
                pFrame->m_desc.channels = 0;
                pFrame->m_desc.channelDepth = 0;
            }


            pFrame->m_lFieldCount = scheduler.CurrentFieldCount;
            pFrame->m_nCardBufferID = scheduler.DoneID;
            pFrame->m_FieldOdd = scheduler.DoneID.Odd;
            pThis->m_pFifoBuffer->PushFrame( pFrame );
            bFirstFrame = FALSE;

            scheduler.NextFrame();
        }
        else
            pThis->m_pSDK->wait_input_video_synch( pThis->m_nUpdateFormat, scheduler.CurrentFieldCount );
    }

    //cout << "Capture Thread Stopped..." << endl;

    pThis->m_pSDK->video_capture_stop();

    VirtualUnlock( pHancBuffer, MAX_HANC_BUFFER_SIZE );
    VirtualFree( pHancBuffer, 0, MEM_RELEASE );

    _endthreadex( 0 );
    return 0;
}

// ***********************
//
UInt32                  CFifoCapture::GetAudioChannelsMask      ( UInt32 numChannels )
{
    UInt32 audioChannelsMask = 0;

    if( numChannels > 0 )	audioChannelsMask |= MONO_CHANNEL_1;
    if( numChannels > 1 )	audioChannelsMask |= MONO_CHANNEL_2;
    if( numChannels > 2 )	audioChannelsMask |= MONO_CHANNEL_3;
    if( numChannels > 3 )	audioChannelsMask |= MONO_CHANNEL_4;
    if( numChannels > 4 )	audioChannelsMask |= MONO_CHANNEL_5;
    if( numChannels > 5 )	audioChannelsMask |= MONO_CHANNEL_6;
    if( numChannels > 6 )	audioChannelsMask |= MONO_CHANNEL_7;
    if( numChannels > 7 )	audioChannelsMask |= MONO_CHANNEL_8;
    if( numChannels > 8 )	audioChannelsMask |= MONO_CHANNEL_11;
    if( numChannels > 9 )	audioChannelsMask |= MONO_CHANNEL_12;
    if( numChannels > 10 )	audioChannelsMask |= MONO_CHANNEL_13;
    if( numChannels > 11 )	audioChannelsMask |= MONO_CHANNEL_14;
    if( numChannels > 12 )	audioChannelsMask |= MONO_CHANNEL_15;
    if( numChannels > 13 )	audioChannelsMask |= MONO_CHANNEL_16;
    if( numChannels > 14 )	audioChannelsMask |= MONO_CHANNEL_17;
    if( numChannels > 15 )	audioChannelsMask |= MONO_CHANNEL_18;

    return audioChannelsMask;
}

// ***********************
//
UInt32                  CFifoCapture::GetSampleSize             ( UInt32 audioFormat )
{
    switch( audioFormat )
    {
    case AUDIO_CHANNEL_16BIT:
        return 2;
    case AUDIO_CHANNEL_24BIT:
        return 3;
    }

    return 0;
}

// ***********************
//
unsigned int            CFifoCapture::QueryChannelInfo          ( CBlueVelvet4 * sdk )
{
    VARIANT varVal;

    sdk->QueryCardProperty( EMBEDDED_AUDIO_INPUT_INFO, varVal );
    return varVal.ulVal;
}

// ***********************
//
unsigned int            CFifoCapture::QueryNumSamples           ( unsigned int channelInfo )
{
    return channelInfo & 0xFFFF;
}



} //bluefish
} //videocards
} //bv