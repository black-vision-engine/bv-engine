#pragma once

#include "win_sock.h"
#include "Bluefish/inc/BlueVelvet4.h"

#include "Models/BlueFish/FifoBuffer.h"

#include "Expected.h"
#include "DataTypes/Reusable.h"




namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
class CFifoCapture
{
public:
	CFifoCapture();
	~CFifoCapture();

    ReturnResult                    Init            ( BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, CFifoBuffer* pFifoBuffer );
    ReturnResult                    InitDualLink    ( BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, CFifoBuffer* pFifoBuffer );

    void                            InitAudio       ();

    void							RouteChannel(ULONG Source, ULONG Destination, ULONG LinkType);
	
    unsigned int static __stdcall	CaptureThread(void * pArg);

    ReturnResult                    InitThread      ();
	void							StartThread     ();
	void							SuspendThread   ();
	void							StopThread      ();

public:

	CBlueVelvet4*	m_pSDK;
	BLUE_INT32		m_iDevices;
	BLUE_INT32		m_nIsAttached;
	BLUE_INT32		m_iCardType;
	BLUE_UINT32		m_nVideoMode;
	
	
	BLUE_UINT32		m_nUpdateFormat;
	BLUE_UINT32		m_nMemoryFormat;
	BLUE_UINT32		m_InvalidVideoModeFlag;
	ULONG			GoldenSize;
	ULONG			BytesPerLine;
	CFifoBuffer*	m_pFifoBuffer;

    UInt8           m_numAudioChannels;
    UInt32          m_audioFormat;
    UInt32          m_samplesFrequency;

	HANDLE			m_hThread;
	BLUE_UINT32		m_nThreadStopping;

	OVERLAPPED		m_Overlap;


private:

    static UInt32           GetAudioChannelsMask            ( UInt32 numChannels );
    static UInt32           GetSampleSize                   ( UInt32 audioFormat );

    static unsigned int     QueryChannelInfo                ( CBlueVelvet4 * sdk );
    static unsigned int     QueryNumSamples                 ( unsigned int channelInfo );
};

} //bluefish
} //videocards
} //bv
