#pragma once
#include "../../VideoCardBase.h"
#include "win_sock.h"
#include <string>
#include <queue>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <process.h>

#include "BlueFish/inc/BlueVelvet4.h"
#include "BlueFish/inc/BlueGpuDirect.h"
#include "BlueFish/inc/BlueHancUtils.h"

#include "Utils.h"
#include "BlueLock.h"
#include "FifoBuffer.h"
#include "FifoCapture.h"
#include "FifoPlayback.h"

namespace bv
{

namespace videocards{
using namespace std;
struct MainThreadArgs
{
	CFifoBuffer*	pInputFifo;
	CFifoBuffer*	pOutputFifo;
	string			strChannel;
	BOOL			bDoRun;
};

class Channel
{
private:
	MainThreadArgs*	m_PlaythroughThreadArgs;
	CFifoBuffer		m_CaptureFifoBuffer;
	CFifoBuffer		m_PlaybackFifoBuffer;
	CFifoCapture*	m_CaptureChannel;
	CFifoPlayback*	m_PlaybackChannel;
	std::string		m_Name;
	std::string		m_Type;
	std::string		m_InputType;
	unsigned int	m_PlaythroughThreadID;
	
public:
	unsigned short	m_Renderer;
	unsigned short	m_Resolution;
	unsigned short	m_Refresh;
	bool			m_Interlaced;
	bool			m_Flipped;
	bool			m_Playback;
	bool			m_Capture;
	bool			m_playthrough;
	ULONG			m_VideoMode;
	HANDLE			m_PlaythroughThreadHandle;
	std::shared_ptr<CFrame>			pFrameOut;
    long            m_referenceMode;
    int             m_refH;
    int             m_refV;

public:

	Channel( std::string name, std::string type, unsigned short renderer, unsigned short resolution, unsigned short	refresh, bool interlaced, bool flipped, bool playback, bool capture, bool playthrough, std::string inputType, long referenceMode, int refH, int refV);
	~Channel();

	std::string		GetName				();
	std::string		GetType 			();
	std::string		GetInputType		();
	CFifoBuffer*	GetCaptureBuffer	();
	CFifoBuffer*	GetPlaybackBuffer	();
	CFifoCapture*	GetCaptureChannel	();
	CFifoPlayback*	GetPlaybackChannel	();
	ULONG			ParseVideoMode		( unsigned short resolution, unsigned short	refresh, bool interlaced );
	void			InitThreads			();
	void			StartThreads		();
	void			StopThreads			();
	void			SuspendThreads		();
	void			ResumeThreads		();
	void			StartDuplexThread	();
    void            GenerateBlack       ();
	unsigned int static __stdcall PlaythroughThread	( void * pArg );

};
}
}