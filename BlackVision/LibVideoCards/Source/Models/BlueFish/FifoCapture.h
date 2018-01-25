#pragma once

#include "win_sock.h"
#include "Bluefish/inc/BlueVelvet4.h"

#include "FifoBuffer.h"

#include "Expected.h"




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

	HANDLE			m_hThread;
	BLUE_UINT32		m_nThreadStopping;

	OVERLAPPED		m_Overlap;

};

} //bluefish
} //videocards
} //bv
