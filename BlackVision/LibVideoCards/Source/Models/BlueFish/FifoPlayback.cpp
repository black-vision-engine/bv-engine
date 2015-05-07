#include "FifoPlayback.h"
namespace bv
{

namespace videocards{
using namespace std;
extern struct blue_videomode_info gVideoModeInfo[];

CFifoPlayback::CFifoPlayback() :
	m_pSDK(NULL),
	m_iDevices(0),
	m_nIsAttached(0),
	m_iCardType(CRD_INVALID),
	m_nVideoMode(VID_FMT_INVALID),
	m_InvalidVideoModeFlag(VID_FMT_INVALID),
	m_pFifoBuffer(NULL),
	m_hThread(0),
	m_nThreadStopping(TRUE)
{
	m_pSDK = BlueVelvetFactory4();
	if(!m_pSDK)
		cout << "No Bluefish SDK" << endl;

	if(m_pSDK)
	{
		m_pSDK->device_enumerate(m_iDevices);
		if(!m_iDevices)
			cout << "No Bluefish card" << endl;
	}
}

CFifoPlayback::~CFifoPlayback()
{
    //CloseHandle(OverlapCh.hEvent);
	if(m_pSDK)
	{
		if(m_nIsAttached)
			m_pSDK->device_detach();
		BlueVelvetDestroy(m_pSDK);
	}
	m_pSDK = NULL;

	//delete m_pFifoBuffer;
}

BLUE_INT32 CFifoPlayback::Init(BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, BLUE_UINT32 VideoMode, CFifoBuffer* pFifoBuffer, long referenceMode, int refH, int refV, bool flipped)
{
	VARIANT varVal;
	//BLUE_INT32 card_type = CRD_INVALID;

	if(m_nIsAttached)
	{
		m_pSDK->device_detach();
		m_nIsAttached = 0;
	}

	if(CardNumber <= 0 || CardNumber > m_iDevices)
	{
		cout << "Card " << CardNumber << " not available; maximum card number is: " << m_iDevices << endl;
		return -1;
	}

	m_pSDK->device_attach(CardNumber, 0);
	m_nIsAttached = 1;

	m_iCardType = m_pSDK->has_video_cardtype();

	//Get number of supported outputs
	varVal.ulVal = 0;
	m_pSDK->QueryCardProperty(CARD_FEATURE_STREAM_INFO, varVal);

	/*unsigned int nOutputStreams = CARD_FEATURE_GET_SDI_OUTPUT_STREAM_COUNT(varVal.ulVal);
	if(nOutputStreams < 2)
	{
		cout << "Card does not support two output channels" << endl;
		m_pSDK->device_detach();
		m_nIsAttached = 0;
		return -1;
	}*/

	varVal.vt = VT_UI4;
	m_pSDK->QueryCardProperty(INVALID_VIDEO_MODE_FLAG, varVal);
	m_InvalidVideoModeFlag = varVal.ulVal;
	
	varVal.ulVal = VideoChannel;
	m_pSDK->SetCardProperty(DEFAULT_VIDEO_OUTPUT_CHANNEL, varVal);

	
	if(VideoMode >= m_InvalidVideoModeFlag)
	{
		cout << "Not a valid video mode" << endl;
		m_pSDK->device_detach();
		m_nIsAttached = 0;
		return -1;
	}

	varVal.ulVal = VideoMode;
	m_pSDK->SetCardProperty(VIDEO_MODE, varVal);
	m_pSDK->QueryCardProperty(VIDEO_MODE, varVal);
	if(VideoMode != varVal.ulVal)
	{
		cout << "Can't set video mode: " << gVideoModeInfo[m_nVideoMode].strVideoModeFriendlyName.c_str() << endl;
		m_pSDK->device_detach();
		m_nIsAttached = 0;
		return -1;
	}
	m_nVideoMode = varVal.ulVal;

	varVal.ulVal = referenceMode;
    m_pSDK->SetCardProperty(VIDEO_GENLOCK_SIGNAL, varVal);

	if( referenceMode == BlueFreeRunning && refV >= 0 && refH >= 0 )
	{
		unsigned int HPhase=refH, VPhase=refV; 
		varVal.ulVal = HPhase;
		varVal.ulVal |= ((VPhase & 0xFFFF) << 16);
		m_pSDK->SetCardProperty(GENLOCK_TIMING, varVal);
		//NOTE: when using BlueSoftware mode the accepted value is a 20bit value coded as 2’s complement
	}

	m_nUpdateFormat = UpdateFormat;
	varVal.ulVal = m_nUpdateFormat;
	m_pSDK->SetCardProperty(VIDEO_UPDATE_TYPE, varVal);

	m_nMemoryFormat = MemoryFormat;
	varVal.ulVal = m_nMemoryFormat;
	m_pSDK->SetCardProperty(VIDEO_MEMORY_FORMAT, varVal);

	varVal.ulVal = VIDEO_ENGINE_PLAYBACK;
	m_pSDK->SetCardProperty(VIDEO_OUTPUT_ENGINE, varVal);

	varVal.ulVal = ENUM_BLACKGENERATOR_OFF;
	m_pSDK->SetCardProperty(VIDEO_BLACKGENERATOR, varVal);

    if(flipped)
    {
        varVal.ulVal = ImageOrientation_VerticalFlip;
    }
    else
    {
        varVal.ulVal = ImageOrientation_Normal;
    }
    m_pSDK->SetCardProperty(VIDEO_IMAGE_ORIENTATION, varVal);
    //RouteChannel(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_SDI_OUTPUT_A, BLUE_CONNECTOR_PROP_DUALLINK_LINK_1);
	//		RouteChannel(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_SDI_OUTPUT_B, BLUE_CONNECTOR_PROP_DUALLINK_LINK_2);
	GoldenSize = BlueVelvetGolden(m_nVideoMode, m_nMemoryFormat, m_nUpdateFormat);
	BytesPerLine = BlueVelvetLineBytes(m_nVideoMode, m_nMemoryFormat);
	m_pFifoBuffer = pFifoBuffer;
	m_pFifoBuffer->Init(4, GoldenSize, BytesPerLine);

	return 0;
}

void CFifoPlayback::RouteChannel(ULONG Source, ULONG Destination, ULONG LinkType)
{
	VARIANT varVal;
	varVal.vt = VT_UI4;

	varVal.ulVal = EPOCH_SET_ROUTING(Source, Destination, LinkType);
	m_pSDK->SetCardProperty(MR2_ROUTING, varVal);
}

BLUE_INT32 CFifoPlayback::InitThread()
{
    //OverlapCh.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  
	unsigned int ThreadId = 0;

	if(m_hThread)
	{
		cout << "Playback Thread already started" << endl;
		return 0;
	}

	cout << "Starting Playback Thread..." << endl;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, &PlaybackThread, this, CREATE_SUSPENDED, &ThreadId);
	if(!m_hThread)
	{
		cout << "Error starting Playback Thread" << endl;
		return -1;
	}

	m_nThreadStopping = FALSE;
	SetThreadPriority(m_hThread, THREAD_PRIORITY_TIME_CRITICAL);
	cout << "...done." << endl;
	return 0;
}

void CFifoPlayback::StartPlaybackThread()
{
	ResumeThread(m_hThread);
}

void CFifoPlayback::ResumePlaybackThread()
{
	ResumeThread(m_hThread);
}

void CFifoPlayback::SuspendPlaybackThread()
{
	SuspendThread(m_hThread);
}

void CFifoPlayback::StopPlaybackThread()
{
	DWORD dw = 0;

	if(m_hThread)
	{
		cout << "Stopping Playback Thread..." << endl;
		m_nThreadStopping = TRUE;
		dw = WaitForSingleObject(m_hThread, (DWORD)-1);
		CloseHandle(m_hThread);
	}
	else
	{
		m_hThread = NULL;
		cout << "...done." << endl;
	}
	return;
}

unsigned int __stdcall CFifoPlayback::PlaybackThread(void * pArg)
{
	CFifoPlayback* pThis = (CFifoPlayback*)pArg;
	ULONG BufferId = 0;
	ULONG CurrentFieldCount = 0;
	ULONG LastFieldCount = 0;
	//ULONG LastBufferTimeStamp = 0;
	unsigned long* NotUsedAddress = NULL;
	unsigned long Underrun = 0;
	unsigned long LastUnderrun = 0;
	unsigned long UniqueId = 0;
	unsigned int nFramesTobuffer = 5;
	unsigned int nFramesPlayed = 0;
	BOOL bPlaybackStarted = FALSE;
	std::shared_ptr<CFrame> pFrame = NULL;

	//make sure FIFO is not running
	pThis->m_pSDK->video_playback_stop(0, 0);

	LastFieldCount = CurrentFieldCount;

	while(!pThis->m_nThreadStopping)
	{
        //DWORD BytesReturnedCh = 0;
			pFrame = pThis->m_pFifoBuffer->m_threadsafebuffer.pop();
		if(!pFrame)
		{
			cout << "Couldn't get buffer from Live queue (playback)" << endl;
			pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount);
			continue;
		}
		if(BLUE_OK(pThis->m_pSDK->video_playback_allocate((void**)&NotUsedAddress, BufferId, Underrun)))
		{
			pThis->m_pSDK->system_buffer_write_async(pFrame->m_pBuffer,
																	pFrame->m_nSize,
																	NULL, 
																	BlueImage_DMABuffer(BufferId, BLUE_DATA_IMAGE),0);
            //GetOverlappedResult(pThis->m_pSDK->m_hDevice, &pThis->OverlapCh, &BytesReturnedCh, TRUE);
            //ResetEvent(pThis->OverlapCh.hEvent);

			pThis->m_pSDK->video_playback_present(UniqueId, BlueBuffer_Image(BufferId), 1, 0, 0);
            nFramesPlayed++;

			if(bPlaybackStarted && Underrun != LastUnderrun)
				cout << "Frame dropped (playback). Current underruns: " << Underrun << endl;
			LastUnderrun = Underrun;
            if(nFramesTobuffer > 0)
            { 
                nFramesTobuffer--;
                if(nFramesTobuffer == 0)
                {
				    pThis->m_pSDK->video_playback_start(0, 0);
                }
            }
		    pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount);
		}
		else
			pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount);
	}

    VARIANT varVal;
    varVal.ulVal = ENUM_BLACKGENERATOR_ON;
	pThis->m_pSDK->SetCardProperty(VIDEO_BLACKGENERATOR, varVal);

	pThis->m_pSDK->video_playback_stop(0, 0);
    
	//CloseHandle(pThis->OverlapCh.hEvent);
	_endthreadex(0);
	return 0;
}

//**************************************
//
void CFifoPlayback::UpdateReferenceOffset(int refH, int refV)
{
    if(m_pSDK)
    {
        VARIANT varVal;       
        varVal.vt = VT_UI4;
        varVal.ulVal = refH;
        varVal.ulVal |= ((refV & 0xFFFF) << 16);
        m_pSDK->SetCardProperty(GENLOCK_TIMING, varVal);
    }
    else
        cout << "BlueFish SDK not INITIALISED" << endl;
}

void CFifoPlayback::UpdateReferenceMode(long referenceMode)
{
    if(m_pSDK)
    {
        VARIANT varVal;       
        varVal.vt = VT_UI4;
        varVal.ulVal = referenceMode;
        m_pSDK->SetCardProperty(VIDEO_GENLOCK_SIGNAL, varVal);
    }
    else
        cout << "BlueFish SDK not INITIALISED" << endl;
}
}
}