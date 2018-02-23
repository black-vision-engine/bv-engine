#include "FifoCapture.h"

#include <process.h>

#include "Serialization/ConversionHelper.h"


namespace bv { namespace videocards { namespace bluefish {

extern struct blue_videomode_info gVideoModeInfo[];

CFifoCapture::CFifoCapture() :
	m_pSDK(NULL),
	m_iDevices(0),
	m_nIsAttached(0),
	m_nVideoMode(VID_FMT_INVALID),
	m_InvalidVideoModeFlag(VID_FMT_INVALID),
	m_pFifoBuffer(NULL),
	m_hThread(0),
	m_nThreadStopping(TRUE)
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

	varVal.ulVal = VIDEO_ENGINE_DUPLEX;	//do not set it to VIDEO_ENGINE_CAPTURE as this will automatically do a playthrough and this will conflict with our playback thread
	m_pSDK->SetCardProperty(VIDEO_INPUT_ENGINE, varVal);

    varVal.ulVal = ImageOrientation_VerticalFlip;
    m_pSDK->SetCardProperty( VIDEO_IMAGE_ORIENTATION, varVal );

	GoldenSize = BlueVelvetGolden(m_nVideoMode, m_nMemoryFormat, m_nUpdateFormat);
	BytesPerLine = BlueVelvetLineBytes(m_nVideoMode, m_nMemoryFormat);
	m_pFifoBuffer = pFifoBuffer;
	m_pFifoBuffer->Init(4, GoldenSize, BytesPerLine);

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
	    //vr.ulVal = Signal_FormatType_444_10BitSDI;
	    //vr.ulVal = Signal_FormatType_444_12BitSDI;
	    m_pSDK->SetCardProperty(VIDEO_DUAL_LINK_INPUT_SIGNAL_FORMAT_TYPE, varVal);

        return Result::Success();
    }

	return result;
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
    ULONG CurrentFieldCount = 0;
    ULONG LastFieldCount = 0;
    struct blue_videoframe_info_ex video_capture_frame;
    int	NotUsedCompostLater = 0;
    unsigned int capture_fifo_size = 0;
    BOOL bFirstFrame = TRUE;

    Reusable< CFramePtr > frames = CreateReusableChunks( pThis->GoldenSize, pThis->BytesPerLine, 3 );


    pThis->m_pSDK->video_capture_start();
    pThis->m_pSDK->wait_input_video_synch( pThis->m_nUpdateFormat, CurrentFieldCount );

    while( !pThis->m_nThreadStopping )
    {
        auto pFrame = frames.GetNext();

        GetVideo_CaptureFrameInfoEx( pThis->m_pSDK, &pThis->m_Overlap, video_capture_frame, NotUsedCompostLater, &capture_fifo_size );
        if( video_capture_frame.nVideoSignalType < VID_FMT_INVALID && video_capture_frame.BufferId != -1 )
        {
            pThis->m_pSDK->system_buffer_read_async( ( unsigned char* )pFrame->m_pBuffer,
                pFrame->m_nSize,
                NULL,
                BlueImage_DMABuffer( video_capture_frame.BufferId, BLUE_DATA_IMAGE ) );

            CurrentFieldCount = video_capture_frame.nFrameTimeStamp;
            //			if(!bFirstFrame && LastFieldCount + 2 < CurrentFieldCount)
            //				cout << "Dropped a frame, FC expected: " << (LastFieldCount + 2) << ", current FC: " << CurrentFieldCount << endl;
            LastFieldCount = CurrentFieldCount;

            pFrame->m_lFieldCount = video_capture_frame.nFrameTimeStamp;
            pFrame->m_nCardBufferID = video_capture_frame.BufferId;
            pThis->m_pFifoBuffer->PushFrame( pFrame );
            bFirstFrame = FALSE;
        }
        else
            pThis->m_pSDK->wait_input_video_synch( pThis->m_nUpdateFormat, CurrentFieldCount );
    }

    //cout << "Capture Thread Stopped..." << endl;

    pThis->m_pSDK->video_capture_stop();

    _endthreadex( 0 );
    return 0;
}

} //bluefish
} //videocards
} //bv