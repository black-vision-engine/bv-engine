#include "CFrame.h"


namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
CFrame::CFrame(BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
    : m_pAudioBuffer( nullptr )
    , m_nAudioSize( 0 )
{
    Init( Size, BytesPerLine,0 );
}

// ***********************
//
CFrame::CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd)
    : m_pAudioBuffer( nullptr )
    , m_nAudioSize( 0 )
{
    Init( Size, BytesPerLine,Odd );
    memcpy(m_pBuffer, buffer, Size);
}

// ***********************
//
CFrame::CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd, BLUE_UINT32 audioSize, const BLUE_UINT8* audioBuffer, BVTimeCode TimeCode, AVFrameDescriptor desc )
    : m_desc( desc )
    , m_nAudioSize( audioSize )
{
    Init( Size, BytesPerLine, Odd);
    memcpy(m_pBuffer, buffer, Size);

    m_TimeCode = TimeCode;

    InitAudioBuffer( audioSize, audioBuffer );
}

// ***********************
//
CFrame::~CFrame()
{
    ReleaseImageBuffer();
    ReleaseAudioBuffer();
}

// ***********************
//
void    CFrame::Init        (BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd)
{
    ReinitParams( Size, BytesPerLine, Odd );
    InitImageBuffer( Size, BytesPerLine, nullptr );
}

// ***********************
//
void    CFrame::ReinitParams        ( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd )
{
    m_pAudioBuffer = NULL;
    m_nCardBufferID = 0;
    m_nSize = Size;
    m_nBytesPerLine = BytesPerLine;
    m_pBuffer = NULL;
    m_lFieldCount = 0;
    m_FieldOdd = Odd;
}

// ***********************
//
void    CFrame::InitImageBuffer     ( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, const BLUE_UINT8* buffer )
{
    m_pBuffer = ( BLUE_UINT8* )VirtualAlloc( NULL, Size, MEM_COMMIT, PAGE_READWRITE );

    if( m_pBuffer )
    {
        VirtualLock( m_pBuffer, Size );

        if( buffer )
            memcpy( m_pBuffer, buffer, Size );
    }

    m_nBytesPerLine = BytesPerLine;
}

// ***********************
//
void    CFrame::InitAudioBuffer     ( BLUE_UINT32 audioSize, const BLUE_UINT8* audioBuffer )
{
    m_pAudioBuffer = ( BLUE_UINT8* )VirtualAlloc( NULL, audioSize, MEM_COMMIT, PAGE_READWRITE );

    if( m_pAudioBuffer )
    {
        VirtualLock( m_pAudioBuffer, audioSize );

        if( audioBuffer )
            memcpy( m_pAudioBuffer, audioBuffer, audioSize );
    }
}

// ***********************
//
void    CFrame::ReleaseImageBuffer  ()
{
    if( m_pBuffer )
    {
        VirtualUnlock( m_pBuffer, m_nSize );
        VirtualFree( m_pBuffer, 0, MEM_RELEASE );

        m_pBuffer = nullptr;
        m_nSize = 0;
    }
}

// ***********************
//
void    CFrame::ReleaseAudioBuffer  ()
{
    if( m_pAudioBuffer )
    {
        VirtualUnlock( m_pAudioBuffer, m_nAudioSize );
        VirtualFree( m_pAudioBuffer, 0, MEM_RELEASE );

        m_pAudioBuffer = nullptr;
        m_nAudioSize = 0;
    }
}

// ***********************
//
void    CFrame::ReinitVideoBuffer   ( BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, const BLUE_UINT8 * buffer )
{
    if( m_nSize != Size )
    {
        ReleaseImageBuffer();
        InitImageBuffer( Size, BytesPerLine, buffer );
    }
    else
    {
        memcpy( m_pBuffer, buffer, Size );
    }

    m_nBytesPerLine = BytesPerLine;
}

// ***********************
//
void    CFrame::ReinitAudioBuffer   ( BLUE_UINT32 audioSize, const BLUE_UINT8* audioBuffer )
{
    if( m_nSize != audioSize )
    {
        ReleaseAudioBuffer();
        InitAudioBuffer( audioSize, audioBuffer );
    }
    else
    {
        memcpy( m_pBuffer, audioBuffer, audioSize );
    }
}



} //bluefish
} //videocards
} //bv