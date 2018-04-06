#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"

#include "Models/BlueFish/FifoBuffer.h"

#include "Expected.h"
#include "DataTypes/Reusable.h"



namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
class CFifoPlayback
{
public:

    CFifoPlayback();
    ~CFifoPlayback();

    ReturnResult                    Init                    ( BLUE_INT32 CardNumber, BLUE_UINT32 VideoChannel, BLUE_UINT32 UpdateFormat, BLUE_UINT32 MemoryFormat, BLUE_UINT32 VideoMode, CFifoBuffer* pFifoBuffer, bool flipped, bool EnableAudioEmbedding, bool EnableVbiVanc, BLUE_UINT32 sdiOutput );
    void                            RouteChannel            ( ULONG Source, ULONG Destination, ULONG LinkType );
    unsigned int static __stdcall   PlaybackThread          ( void * pArg );
    unsigned int static __stdcall   PlaybackThreadNotSynchronised   ( void * pArg );
    ReturnResult                    InitThread              ();
    //BLUE_INT32                      InitNotSyncedThread();
    void                            StartThread             ();
    void                            SuspendThread           ();
    void                            StopThread              ();

    SizeType                        GetNumFrameDrops        () { return m_debugNumberFrameDrops; }

public:

    CBlueVelvet4*   m_pSDK;
    BLUE_INT32      m_iDevices;
	BLUE_INT32		m_iCardType;
    BLUE_INT32      m_nIsAttached;
	bool			m_EnableEmbAudio;
	bool			m_EnableVbiVanc;
	
	BLUE_UINT32		m_SdiOutputConnector;
	BLUE_UINT32		m_nVideoChannel;
    BLUE_UINT32     m_nVideoMode;
    BLUE_UINT32     m_nUpdateFormat;
    BLUE_UINT32     m_nMemoryFormat;
    BLUE_UINT32     m_InvalidVideoModeFlag;
    ULONG           GoldenSize;
    ULONG           BytesPerLine;

    CFifoBuffer*            m_pFifoBuffer;
    Reusable< CFramePtr >   m_reusableFrames;

    HANDLE          m_hThread;
    BLUE_UINT32     m_nThreadStopping;

    bool			m_EnableTimecode;
    bool            m_AutoGeneratedTimecode;

    SizeType        m_debugNumberFrameDrops;
};

} //bluefish
} //videocards
} //bv