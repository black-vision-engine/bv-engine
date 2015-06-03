#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"
#include "FifoBuffer.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <process.h>
#include <chrono>
namespace bv
{

namespace videocards{
class CFifoPlayback
{
public:
	CFifoPlayback();
	~CFifoPlayback();

	BLUE_INT32						Init(BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, BLUE_UINT32 VideoMode, CFifoBuffer* pFifoBuffer, long referenceMode, int refH, int refV, bool flipped);
	void							RouteChannel(ULONG Source, ULONG Destination, ULONG LinkType);
	unsigned int static __stdcall	PlaybackThread(void * pArg);
	unsigned int static __stdcall	PlaybackThreadNotSynchronised(void * pArg);
	BLUE_INT32						InitThread();
	BLUE_INT32                      InitNotSyncedThread();
	void							StartPlaybackThread();
	void							SuspendPlaybackThread();
	void							ResumePlaybackThread();
	void							StopPlaybackThread();
	bool							UpdateReferenceOffset(int refH, int refV);
	bool							UpdateReferenceMode(long referenceMode);

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
};

 
}
}
