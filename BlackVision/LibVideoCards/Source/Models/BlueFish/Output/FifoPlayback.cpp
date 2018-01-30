#include "FifoPlayback.h"
#include <iostream>
#include "Bluefish/inc/BlueHancUtils.h"
#include <process.h>
#include "BVTimeCode.h"


#include "Serialization/ConversionHelper.h"

#include "UseLoggerVideoModule.h"




namespace bv {
namespace videocards {
namespace bluefish
{




#define MAX_HANC_BUFFER_SIZE (256*1024)
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
    m_nThreadStopping(TRUE),
    m_debugNumberFrameDrops( 0 )
{
    m_pSDK = BlueVelvetFactory4();
    //if(!m_pSDK)
    //  cout << "No Bluefish SDK" << endl;

    if(m_pSDK)
    {
        m_pSDK->device_enumerate(m_iDevices);
        //if(!m_iDevices)
        //  cout << "No Bluefish card" << endl;
    }
}

CFifoPlayback::~CFifoPlayback()
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
ReturnResult        CFifoPlayback::Init     ( BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, BLUE_UINT32 VideoMode, CFifoBuffer* pFifoBuffer, long referenceMode, int refH, int refV, bool flipped, bool EnableAudioEmbedding, bool EnableVbiVanc, BLUE_UINT32 sdiOutput )
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
        return ReturnResult::fromError( "Card " + Convert::T2String( CardNumber ) + " not available; maximum card number is: " + Convert::T2String( m_iDevices ) );
    }

    m_pSDK->device_attach(CardNumber, 0);
    m_nIsAttached = 1;
	m_nVideoChannel = VideoChannel;
	m_EnableEmbAudio = EnableAudioEmbedding;
	m_EnableVbiVanc = EnableVbiVanc;

    m_iCardType = m_pSDK->has_video_cardtype(CardNumber);

	m_SdiOutputConnector = sdiOutput;

    //Get number of supported outputs
    varVal.ulVal = 0;
    m_pSDK->QueryCardProperty(CARD_FEATURE_STREAM_INFO, varVal);

    unsigned int nOutputStreams = CARD_FEATURE_GET_SDI_OUTPUT_STREAM_COUNT(varVal.ulVal);
    if(!nOutputStreams)
    {
        m_pSDK->device_detach();
        m_nIsAttached = 0;

        return "Card does not support output channels";
    }

    varVal.vt = VT_UI4;
    m_pSDK->QueryCardProperty(INVALID_VIDEO_MODE_FLAG, varVal);
    m_InvalidVideoModeFlag = varVal.ulVal;
    
    varVal.ulVal = VideoChannel;
    m_pSDK->SetCardProperty(DEFAULT_VIDEO_OUTPUT_CHANNEL, varVal);

    
    if(VideoMode >= m_InvalidVideoModeFlag)
    {
        m_pSDK->device_detach();
        m_nIsAttached = 0;
        return "No valid video mode";
    }

    varVal.ulVal = VideoMode;
    m_pSDK->SetCardProperty(VIDEO_MODE, varVal);
    m_pSDK->QueryCardProperty(VIDEO_MODE, varVal);
    if(VideoMode != varVal.ulVal)
    {
        //cout << "Can't set video mode: " << gVideoModeInfo[m_nVideoMode].strVideoModeFriendlyName.c_str() << endl;
        m_pSDK->device_detach();
        m_nIsAttached = 0;
        return "Can't set video mode."; // +gVideoModeInfo[ m_nVideoMode ].strVideoModeFriendlyName.c_str()
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
    
    VARIANT vProp;
    vProp.vt = VT_UI4;
    m_pSDK->QueryCardProperty(EPOCH_AVAIL_VIDEO_SCALER_COUNT,vProp);
    //cout << "Available Scaler Count: " << vProp.ulVal << endl;
    if(vProp.ulVal > 0)
    {
        UINT32 nAvailableScalers = vProp.ulVal;
        UINT32 scalerID = 0;
        for(scalerID=0; scalerID < nAvailableScalers; scalerID++)
        {
            vProp.ulVal = scalerID;
            m_pSDK->QueryCardProperty(EPOCH_ENUM_AVAIL_VIDEO_SCALERS_ID, vProp);
            //cout << "Available Scaler ID: " << vProp.ulVal << endl;
        }
    }


    GoldenSize = BlueVelvetGolden(m_nVideoMode, m_nMemoryFormat, m_nUpdateFormat);
    BytesPerLine = BlueVelvetLineBytes(m_nVideoMode, m_nMemoryFormat);
    m_pFifoBuffer = pFifoBuffer;
    m_pFifoBuffer->Init(4, GoldenSize, BytesPerLine);

    return Result::Success();
}

// ***********************
//
void                CFifoPlayback::RouteChannel(ULONG Source, ULONG Destination, ULONG LinkType)
{
    VARIANT varVal;
    varVal.vt = VT_UI4;

    varVal.ulVal = EPOCH_SET_ROUTING(Source, Destination, LinkType);
    m_pSDK->SetCardProperty(MR2_ROUTING, varVal);
}

// ***********************
//
ReturnResult        CFifoPlayback::InitThread()
{ 
    unsigned int ThreadId = 0;

    if(m_hThread)
    {
        //cout << "Playback Thread already started" << endl;
        return Result::Success();
    }
    
    //cout << "Starting Playback Thread..." << endl;
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, &PlaybackThread, this, CREATE_SUSPENDED, &ThreadId);

    if(!m_hThread)
    {
        return "Error starting Playback Thread.";
    }

    m_nThreadStopping = FALSE;
    SetThreadPriority(m_hThread, THREAD_PRIORITY_TIME_CRITICAL);
    //cout << "...done." << endl;
    return Result::Success();
}

//BLUE_INT32 CFifoPlayback::InitNotSyncedThread()
//{ 
//  unsigned int ThreadId = 0;
//
//  if(m_hThread)
//  {
//      //cout << "Playback Thread already started" << endl;
//      return 0;
//  }
//  
//  //cout << "Starting Not Synced Playback Thread..." << endl;
//  m_hThread = (HANDLE)_beginthreadex(NULL, 0, &PlaybackThreadNotSynchronised, this, CREATE_SUSPENDED, &ThreadId);
//    
//  if(!m_hThread)
//  {
//      //cout << "Error starting Playback Thread" << endl;
//      return -1;
//  }
//
//  m_nThreadStopping = FALSE;
//  SetThreadPriority(m_hThread, THREAD_PRIORITY_TIME_CRITICAL);
//  cout << "...done." << endl;
//  return 0;
//}


void CFifoPlayback::StartThread()
{
    ResumeThread(m_hThread);
}

void CFifoPlayback::SuspendThread()
{
    ::SuspendThread(m_hThread);
}

void CFifoPlayback::StopThread()
{
    DWORD dw = 0;

    if(m_hThread)
    {
        m_pFifoBuffer->PushEmptyFrame();
        m_pFifoBuffer->PushEmptyFrame();

        m_nThreadStopping = TRUE;
        dw = WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
    }
    else
    {
        m_hThread = NULL;
    }
    return;
}

int GetCardProperty(CBlueVelvet4* pSdk, BLUE_UINT32 prop, BLUE_UINT32& value)
{
	VARIANT varVal;
	int errorCode;
	varVal.vt = VT_UI4;
	varVal.ulVal = value;
	errorCode = pSdk->QueryCardProperty(prop, varVal);
	value = varVal.ulVal;
	return errorCode;
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
	unsigned int nFramesTobuffer = 2;
	unsigned int nFramesPlayed = 0;
	//BOOL bPlaybackStarted = FALSE;

	// hanc / audio / vanc

	BLUE_UINT32* pHancBuffer = NULL;
	
	BLUE_UINT32* pVbiVancBuffer = NULL;
	BLUE_UINT32 nAudioSequenceCount = 0; 
	
	
	BLUE_UINT32	nBytesPerAudioChannel = 2;
	BLUE_UINT32 nSampleType = AUDIO_CHANNEL_16BIT;

	BLUE_UINT32 embAudioProp = 0;
	BLUE_UINT32 aesAudioRouting = 0;
	BLUE_UINT32 sdiOutputRouting = 0;
	embAudioProp = blue_enable_hanc_timestamp_pkt;

	

	if (pThis->m_EnableEmbAudio)
	{
		embAudioProp |= (blue_emb_audio_enable | blue_emb_audio_group1_enable );
	}

	if (pThis->m_nVideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_D)
	{
		aesAudioRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHD, EPOCH_DEST_AES_ANALOG_AUDIO_OUTPUT, BLUE_CONNECTOR_PROP_SINGLE_LINK);
		sdiOutputRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHD, pThis->m_SdiOutputConnector, BLUE_CONNECTOR_PROP_SINGLE_LINK);
	}
	else if (pThis->m_nVideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_C)
	{
		aesAudioRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHC, EPOCH_DEST_AES_ANALOG_AUDIO_OUTPUT, BLUE_CONNECTOR_PROP_SINGLE_LINK);
		sdiOutputRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHC, pThis->m_SdiOutputConnector, BLUE_CONNECTOR_PROP_SINGLE_LINK);
	}
	else if (pThis->m_nVideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_B)
	{
		aesAudioRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHB, EPOCH_DEST_AES_ANALOG_AUDIO_OUTPUT, BLUE_CONNECTOR_PROP_SINGLE_LINK);
		sdiOutputRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHB, pThis->m_SdiOutputConnector, BLUE_CONNECTOR_PROP_SINGLE_LINK);
	}
	else //if(nVideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_A)
	{
		aesAudioRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_AES_ANALOG_AUDIO_OUTPUT, BLUE_CONNECTOR_PROP_SINGLE_LINK);
		sdiOutputRouting = EPOCH_SET_ROUTING(EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, pThis->m_SdiOutputConnector, BLUE_CONNECTOR_PROP_SINGLE_LINK);
	}


	std::shared_ptr<CFrame> pFrame = NULL;

	OVERLAPPED OverlapChA;
	OverlapChA.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//	DWORD BytesReturnedChA = 0;

	//make sure FIFO is not running
	pThis->m_pSDK->video_playback_stop(0, 0);

	VARIANT varVal;
	varVal.vt = VT_UI4;
	varVal.ulVal = ENUM_BLACKGENERATOR_OFF;
	pThis->m_pSDK->SetCardProperty(VIDEO_BLACKGENERATOR, varVal);

	LastFieldCount = CurrentFieldCount;
	ULONG LastUnderrunChA = 0;
	ULONG UnderrunChA = 0;
	
	

	int nCardType = pThis->m_pSDK->has_video_cardtype();

	BLUE_UINT32 UpdateType = UPD_FMT_FIELD;
	BLUE_UINT32 VideoMode = UPD_FMT_FIELD;
	BLUE_UINT32 MemoryFormat = UPD_FMT_FIELD;
	GetCardProperty(pThis->m_pSDK, VIDEO_MEMORY_FORMAT, MemoryFormat);
	GetCardProperty(pThis->m_pSDK, VIDEO_MODE, VideoMode);
	GetCardProperty(pThis->m_pSDK, VIDEO_UPDATE_TYPE, UpdateType);




	



	//hanc
	
	ULONG VideoGolden = BlueVelvetGolden(VideoMode, MemoryFormat, UpdateType);
	ULONG PixelsPerLine = BlueVelvetLinePixels(VideoMode);
	ULONG VBILines = BlueVelvetVANCLineCount(nCardType, pThis->m_nVideoMode, (UpdateType == UPD_FMT_FRAME) ? BLUE_DATA_FRAME : BLUE_DATA_FIELD1);
	ULONG VBIBytesPerLine = 0;
	ULONG VBIGolden = 0;

	if (VideoMode == VID_FMT_PAL || VideoMode == VID_FMT_NTSC)
	{
		VBIBytesPerLine = BlueVelvetVANCLineBytes(nCardType, VideoMode, MemoryFormat);
		VBIGolden = BlueVelvetVANCGoldenValue(nCardType, VideoMode, MemoryFormat, (UpdateType == UPD_FMT_FRAME) ? BLUE_DATA_FRAME : BLUE_DATA_FIELD1);
	}
	else
	{
		VBIBytesPerLine = BlueVelvetVANCLineBytes(nCardType, VideoMode, MEM_FMT_V210);
		VBIGolden = BlueVelvetVANCGoldenValue(nCardType, VideoMode, MEM_FMT_V210, (UpdateType == UPD_FMT_FRAME) ? BLUE_DATA_FRAME : BLUE_DATA_FIELD1);
	}

	pHancBuffer = (BLUE_UINT32 *)VirtualAlloc(NULL, MAX_HANC_BUFFER_SIZE, MEM_COMMIT, PAGE_READWRITE);
	pVbiVancBuffer = (BLUE_UINT32 *)VirtualAlloc(NULL, VBIGolden, MEM_COMMIT, PAGE_READWRITE);

	if (VideoMode == VID_FMT_PAL || VideoMode == VID_FMT_NTSC)
		memset(pVbiVancBuffer, 0, VBIGolden);
	else
		blue_init_vanc_buffer(nCardType, VideoMode, PixelsPerLine, VBILines, pVbiVancBuffer);


	hanc_stream_info_struct hanc_stream_info;
	::ZeroMemory(&hanc_stream_info, sizeof(hanc_stream_info));
	hanc_stream_info.AudioDBNArray[0] = -1;
	hanc_stream_info.AudioDBNArray[1] = -1;
	hanc_stream_info.AudioDBNArray[2] = -1;
	hanc_stream_info.AudioDBNArray[3] = -1;
	hanc_stream_info.hanc_data_ptr = pHancBuffer;
	hanc_stream_info.video_mode = pThis->m_nVideoMode;


	{nAudioSequenceCount;}
	{nBytesPerAudioChannel;}
	{nSampleType;}
	{PixelsPerLine;}
	{nBytesPerAudioChannel;}
	{VBIBytesPerLine;}
	{VBILines;}
	{VideoGolden;}

	// audio

	
	{pHancBuffer;}

	//timecode

	BLUE_UINT32 nProcessHANC = 0;
	BVBluefishTimeCode rp188_vitc;
	BVBluefishTimeCode rp188_ltc;
	BVBluefishTimeCode ext_ltc;
	BVBluefishTimeCode sd_vitc;

	{rp188_vitc;}
	{rp188_ltc;}
	{ext_ltc;}
	{sd_vitc;}
	
	pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount);
	if (!(CurrentFieldCount & 0x1))
		pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount);	//we need to schedule the playback of field 0 at field 1 interrupt


	BLUE_UINT32 nEmbAudioFlag = 0;
	nEmbAudioFlag = (blue_emb_audio_enable | blue_emb_audio_group1_enable); // liczba kana³ów... do ogarniêcia...
	

    while( !pThis->m_nThreadStopping )
    {
		
        if( BLUE_OK( pThis->m_pSDK->video_playback_allocate( ( void** )&NotUsedAddress, BufferId, UnderrunChA ) ) )
        {
			
			pFrame = pThis->m_pFifoBuffer->PopFrame();
			int odd = (int)(pFrame->m_FieldOdd);
			if (!pFrame)
			{
				//std::cout << "Couldn't get buffer from Live queue (playback)" << std::endl;
				pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount);
				continue;
			}
			
			if( pThis->m_pFifoBuffer->IsEmptyFrame( pFrame ) )
			{
				continue;
			}

			if ((UpdateType == UPD_FMT_FRAME) || ((UpdateType == UPD_FMT_FIELD) && !odd))
				nProcessHANC = 1;
			else
				nProcessHANC = 0;

			
			if (nProcessHANC)
			{
				rp188_vitc = pFrame->m_TimeCode.GetTimeCode(false);
				rp188_ltc = pFrame->m_TimeCode.GetTimeCode(true);
				ext_ltc = pFrame->m_TimeCode.GetTimeCode(true);
				sd_vitc = pFrame->m_TimeCode.GetTimeCode(true);

                if ( pThis->m_AutoGeneratedTimecode )
				{
					pFrame->m_TimeCode.GenerateTimeCode(nFramesPlayed, 50, false, false, false, rp188_vitc, false);
					pFrame->m_TimeCode.GenerateTimeCode(nFramesPlayed, 50, false, false, true, rp188_ltc, false);
					pFrame->m_TimeCode.GenerateTimeCode(nFramesPlayed, 50, false, false, true, ext_ltc, false);
					pFrame->m_TimeCode.GenerateTimeCode(nFramesPlayed, 50, false, false, true, sd_vitc, false);
				}
				hanc_stream_info.time_code = rp188_vitc.timecode_u64;			//RP188 VITC time code
				hanc_stream_info.rp188_ltc_time_code = rp188_ltc.timecode_u64;	//RP188 LTC time code
				hanc_stream_info.ltc_time_code = ext_ltc.timecode_u64;			//external LTC time code
				hanc_stream_info.sd_vitc_time_code = sd_vitc.timecode_u64;		//this field is only valid for SD video signal 

				if ( !pThis->m_EnableTimecode )
				{
					hanc_stream_info.time_code = 0LL;			//RP188 VITC time code
					hanc_stream_info.rp188_ltc_time_code = 0LL;	//RP188 LTC time code
					hanc_stream_info.ltc_time_code = 0LL;			//external LTC time code
					hanc_stream_info.sd_vitc_time_code = 0LL;		//this field is only valid for SD video signal 
				}

				encode_hanc_frame_ex(nCardType,
					&hanc_stream_info,
					pFrame->m_pAudioBuffer,
					pFrame->m_desc.channels,
					pFrame->m_desc.sampleRate,
					nSampleType, //FIXME: get bit depth from frame
					nEmbAudioFlag);

				if (pThis->m_EnableVbiVanc)
					pThis->m_pSDK->system_buffer_write_async((unsigned char*)pHancBuffer, MAX_HANC_BUFFER_SIZE, NULL, BlueImage_VBI_HANC_DMABuffer(BufferId, BLUE_DATA_HANC));
				else
					pThis->m_pSDK->system_buffer_write_async((unsigned char*)pHancBuffer, MAX_HANC_BUFFER_SIZE, NULL, BlueImage_HANC_DMABuffer(BufferId, BLUE_DATA_HANC));

				pThis->m_pSDK->render_buffer_update(BlueBuffer_Image_HANC(BufferId));
			}

			if (pThis->m_EnableVbiVanc)
			{
				pThis->m_pSDK->system_buffer_write_async((PBYTE)pVbiVancBuffer, VBIGolden, NULL, nProcessHANC ? BlueImage_VBI_HANC_DMABuffer(BufferId, BLUE_DATA_VBI) : BlueImage_VBI_DMABuffer(BufferId, BLUE_DATA_VBI));
				pThis->m_pSDK->system_buffer_write_async(pFrame->m_pBuffer, pFrame->m_nSize, NULL, nProcessHANC ? BlueImage_VBI_HANC_DMABuffer(BufferId, BLUE_DATA_IMAGE) : BlueImage_VBI_DMABuffer(BufferId, BLUE_DATA_IMAGE));
				
			/*	GetOverlappedResult(pThis->m_pSDK->m_hDevice, &OverlapChA, &BytesReturnedChA, TRUE);
				ResetEvent(OverlapChA.hEvent);*/

				pThis->m_pSDK->video_playback_present(UniqueId, nProcessHANC ? BlueBuffer_Image_VBI_HANC(BufferId) : BlueBuffer_Image_VBI(BufferId), 1, 0, odd);
			}
			else
			{
				pThis->m_pSDK->system_buffer_write_async(pFrame->m_pBuffer, pFrame->m_nSize, NULL, nProcessHANC ? BlueImage_HANC_DMABuffer(BufferId, BLUE_DATA_IMAGE) : BlueImage_DMABuffer(BufferId, BLUE_DATA_IMAGE));
				
				/*GetOverlappedResult(pThis->m_pSDK->m_hDevice, &OverlapChA, &BytesReturnedChA, TRUE);
				ResetEvent(OverlapChA.hEvent*);*/


				pThis->m_pSDK->video_playback_present(UniqueId, nProcessHANC ? BlueBuffer_Image_HANC(BufferId) : BlueBuffer_Image(BufferId), 1, 0, odd);
			}

			_EDMADataType dt = BLUE_DATA_FIELD1;
			if (odd == 1)
				dt = BLUE_DATA_FIELD2;

		
			
			
			//track UnderrunChA and UnderrunChB to see if frames were dropped
			if (UnderrunChA != LastUnderrunChA)
			{
                pThis->m_debugNumberFrameDrops++;

				//std::cout << "Dropped a frame: ChA underruns: " << UnderrunChA << std::endl;
                LOG_MESSAGE( SeverityLevel::info ) << "DROP FRAME: BlueFish channel A";
				LastUnderrunChA = UnderrunChA;
			}

			nFramesPlayed++;

            //if( bPlaybackStarted && Underrun != LastUnderrun )
                //cout << "Frame dropped (playback). Current underruns: " << Underrun << endl;
            LastUnderrun = Underrun;
            
            //pThis->m_pSDK->wait_output_video_synch( UPD_FMT_FIELD, CurrentFieldCount );
        }
		else {
			pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount);
			//std::cout << CurrentFieldCount << " waiting..." << std::endl;
		}

		if (nFramesTobuffer > 0)
		{
			nFramesTobuffer--;
			if (nFramesTobuffer == 0)
			{
				pThis->m_pSDK->video_playback_start(0, 0);

			}
		}
    }

	//turn on black generator (unless we want to keep displaying the last rendered frame)
	varVal.ulVal = ENUM_BLACKGENERATOR_ON;
	pThis->m_pSDK->SetCardProperty(VIDEO_BLACKGENERATOR, varVal);
	varVal.ulVal = 0;
	pThis->m_pSDK->SetCardProperty(FORCE_SD_VBI_OUTPUT_BUFFER_TO_V210, varVal);	//when changing this property the video output mode and video output engine need to be set again manually!

    bool blackout = false;
    pFrame = std::make_shared<CFrame>(pThis->GoldenSize, pThis->BytesPerLine );
    while( !blackout )
    {
        if( BLUE_OK( pThis->m_pSDK->video_playback_allocate( ( void** )&NotUsedAddress, BufferId, Underrun ) ) )
        {
            pThis->m_pSDK->system_buffer_write_async( pFrame->m_pBuffer,
                                                      pFrame->m_nSize,
                                                      NULL,
                                                      BlueImage_DMABuffer( BufferId, BLUE_DATA_IMAGE ), 0 );

            //cout << "Playback Black..." << endl;
            pThis->m_pSDK->video_playback_present( UniqueId, BlueBuffer_Image( BufferId ), 1, 0, 0 );
            blackout = true;
        }
        else
        {
            pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FIELD, CurrentFieldCount );
        }
    }
    //cout << "Playback Thread Stopped..." << endl;

    pThis->m_pSDK->video_playback_stop( 100, 1 );
    _endthreadex( 0 );
    return 0;
}

//
//unsigned int __stdcall CFifoPlayback::PlaybackThreadNotSynchronised(void * pArg)
//{
//  CFifoPlayback* pThis = (CFifoPlayback*)pArg;
//  ULONG BufferId = 0;
//  ULONG CurrentFieldCount = 0;
//  ULONG LastFieldCount = 0;
//  unsigned long* NotUsedAddress = NULL;
//  unsigned long Underrun = 0;
//  unsigned long LastUnderrun = 0;
//  unsigned long UniqueId = 0;
//  unsigned int nFramesTobuffer = 1;
//  unsigned int nFramesPlayed = 0;
//  BOOL bPlaybackStarted = FALSE;
//  std::shared_ptr<CFrame> pFrame = NULL;
//
//  //make sure FIFO is not running
//  pThis->m_pSDK->video_playback_stop(0, 0);
//
//  LastFieldCount = CurrentFieldCount;
//
//  while(!pThis->m_nThreadStopping)
//  {
//          pFrame = pThis->m_pFifoBuffer->m_threadsafebuffer.pop();
//      if(!pFrame)
//      {
//          cout << "Couldn't get buffer from Live queue (playback)" << endl;
//          continue;
//      }
//      if(BLUE_OK(pThis->m_pSDK->video_playback_allocate((void**)&NotUsedAddress, BufferId, Underrun)))
//      {
//          pThis->m_pSDK->system_buffer_write_async(pFrame->m_pBuffer,
//                                                                  pFrame->m_nSize,
//                                                                  NULL, 
//                                                                  BlueImage_DMABuffer(BufferId, BLUE_DATA_IMAGE),0);
//
//          pThis->m_pSDK->video_playback_present(UniqueId, BlueBuffer_Image(BufferId), 1, 0, 0);
//            nFramesPlayed++;
//
//          if(bPlaybackStarted && Underrun != LastUnderrun)
//              cout << "Frame dropped (playback). Current underruns: " << Underrun << endl;
//          LastUnderrun = Underrun;
//            if(nFramesTobuffer > 0)
//            { 
//                nFramesTobuffer--;
//                if(nFramesTobuffer == 0)
//                {
//                  pThis->m_pSDK->video_playback_start(0, 0);
//                }
//            }
//      }
//        else
//        {
//            pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FRAME, CurrentFieldCount);
//        }
//  }
//    
//  bool blackout = false;
//  pFrame = std::make_shared<CFrame>(0,pThis->GoldenSize,pThis->BytesPerLine);
//    while(!blackout)
//    {
//      if(BLUE_OK(pThis->m_pSDK->video_playback_allocate((void**)&NotUsedAddress, BufferId, Underrun)))
//      {
//          pThis->m_pSDK->system_buffer_write_async(pFrame->m_pBuffer,
//                                                                  pFrame->m_nSize,
//                                                                  NULL, 
//                                                                  BlueImage_DMABuffer(BufferId, BLUE_DATA_IMAGE),0);
//        
//          cout << "Playback Black..." << endl;
//          pThis->m_pSDK->video_playback_present(UniqueId, BlueBuffer_Image(BufferId), 1, 0, 0);
//            blackout = true;
//      }
//        else
//        {
//            pThis->m_pSDK->wait_output_video_synch(UPD_FMT_FRAME, CurrentFieldCount);
//        }
//    }
//  cout << "Playback Thread Stopped..." << endl;
//
//    pThis->m_pSDK->video_playback_stop(100, 1);
//  _endthreadex(0);
//  return 0;
//}

//**************************************
//
bool CFifoPlayback::UpdateReferenceOffset(int refH, int refV)
{
    if(m_pSDK)
    {
        VARIANT varVal;       
        varVal.vt = VT_UI4;
        varVal.ulVal = refH;
        varVal.ulVal |= ((refV & 0xFFFF) << 16);
        m_pSDK->SetCardProperty(GENLOCK_TIMING, varVal);
        return true;
    }
    else
    {
        //cout << "BlueFish SDK not INITIALISED (UpdateReferenceOffset in CFifoPlayback)" << endl;
        return false;
    }
}

//**************************************
//
bool CFifoPlayback::UpdateReferenceMode(long referenceMode)
{
    if(m_pSDK)
    {
        VARIANT varVal;       
        varVal.vt = VT_UI4;
        varVal.ulVal = referenceMode;
        m_pSDK->SetCardProperty(VIDEO_GENLOCK_SIGNAL, varVal);
        return true;
    }
    else
    {
        //cout << "BlueFish SDK not INITIALISED (UpdateReferenceMode in CFifoPlayback)" << endl;
        return false;
    }
}

} //bluefish
} //videocards
} //bv
