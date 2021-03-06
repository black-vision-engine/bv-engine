#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"
#include "Memory/AVFrame.h"
#include "BVTimeCode.h"

namespace bv { namespace videocards { namespace bluefish {

class CFrame
{
public:

    CFrame( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine );
    CFrame( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, BLUE_UINT32 audioSize );
    CFrame( const BLUE_UINT8* buffer, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int odd );
    CFrame( const BLUE_UINT8* videoBuffer, BLUE_UINT32 videoSize, BLUE_UINT32 BytesPerLine, int odd, BLUE_UINT32 audioSize, const BLUE_UINT8* audioBuffer, BVTimeCode TimeCode, AVFrameDescriptor desc );
    ~CFrame();

public:

    int m_FieldOdd;

    AVFrameDescriptor   m_desc;
    BVTimeCode          m_TimeCode;

    BLUE_UINT32 m_nCardBufferID;
    BLUE_UINT32 m_nSize;
    BLUE_UINT32 m_nBytesPerLine;
    BLUE_UINT8* m_pBuffer;
    BLUE_UINT8* m_pAudioBuffer;
    BLUE_UINT32 m_nAudioSize;
    ULONG       m_lFieldCount;
    
private:

    void        Init                    ( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd );
    void        ReinitParams            ( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd );

    void        InitImageBuffer         ( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, const BLUE_UINT8* buffer );
    void        InitAudioBuffer         ( BLUE_UINT32 audioSize, const BLUE_UINT8* audioBuffer );
    void        ReleaseImageBuffer      ();
    void        ReleaseAudioBuffer      ();

public:

    void        ReinitVideoBuffer       ( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, const BLUE_UINT8* buffer );
    void        ReinitAudioBuffer       ( BLUE_UINT32 audioSize, const BLUE_UINT8* audioBuffer );
};

DEFINE_PTR_TYPE( CFrame );

} //bluefish
} //videocards
} //bv
