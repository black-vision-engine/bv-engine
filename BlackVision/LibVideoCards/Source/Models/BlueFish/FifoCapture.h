#pragma once

#include "win_sock.h"
#include "Bluefish/inc/BlueVelvet4.h"
#include "FifoBuffer.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <process.h>
namespace bv
{

namespace videocards{
class CFifoCapture
{
public:
	CFifoCapture();
	~CFifoCapture();

	BLUE_INT32                      InitDualLink(BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, CFifoBuffer* pFifoBuffer);
	BLUE_INT32						Init(BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, CFifoBuffer* pFifoBuffer);
	void							RouteChannel(ULONG Source, ULONG Destination, ULONG LinkType);
	unsigned int static __stdcall	CaptureThread(void * pArg);
	//unsigned int static __stdcall	InputMidgardThread( void * pArg );
	BLUE_INT32						InitThread();
	void							StartCaptureThread();
	void							ResumeCaptureThread();
	void							SuspendCaptureThread();
	void							StopCaptureThread();

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
	CFifoBuffer*	m_InputMidgardFifoBuffer;

	HANDLE			m_hThread;
	BLUE_UINT32		m_nThreadStopping;
	HANDLE			m_hMidgardInputThread;
	BLUE_UINT32		m_nMidgardInputThreadStopping;

	OVERLAPPED		m_Overlap;

    bool            m_playthrough;
};
}
}
