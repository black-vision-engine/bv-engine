#include "CFrame.h"


namespace bv { namespace videocards { namespace bluefish {


CFrame::CFrame(BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
    : m_pAudioBuffer( nullptr )
    , m_nAudioSize( 0 )
{
    Init( Size, BytesPerLine,0 );
}

CFrame::CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd)
    : m_pAudioBuffer( nullptr )
    , m_nAudioSize( 0 )
{
    Init( Size, BytesPerLine,Odd );
    memcpy(m_pBuffer, buffer, Size);
}

CFrame::CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd, BLUE_UINT32 audioSize, const BLUE_UINT8* audioBuffer, BVTimeCode TimeCode, AVFrameDescriptor desc )
    : m_desc( desc )
    , m_nAudioSize( audioSize )
{
    Init( Size, BytesPerLine, Odd);
    memcpy(m_pBuffer, buffer, Size);

    m_TimeCode = TimeCode;

    m_pAudioBuffer = (BLUE_UINT8*)VirtualAlloc(NULL, audioSize, MEM_COMMIT, PAGE_READWRITE);

    if(m_pAudioBuffer)
    {
        VirtualLock(m_pAudioBuffer, audioSize);
        memcpy(m_pAudioBuffer, audioBuffer, audioSize);
    }
}

CFrame::~CFrame()
{
    if (m_pBuffer)
    {
        VirtualUnlock(m_pBuffer, m_nSize);
        VirtualFree(m_pBuffer, 0, MEM_RELEASE);
    }

    if (m_pAudioBuffer)
    {
        VirtualUnlock(m_pAudioBuffer, m_nAudioSize);
        VirtualFree(m_pAudioBuffer, 0, MEM_RELEASE);
    }
}

void    CFrame::Init        (BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd)
{
    m_pAudioBuffer = NULL;
    m_nCardBufferID = 0;
    m_nSize = Size;
    m_nBytesPerLine = BytesPerLine;
    m_pBuffer = NULL;
    m_lFieldCount = 0;
    m_FieldOdd = Odd;

    m_pBuffer = (BLUE_UINT8*)VirtualAlloc(NULL, Size, MEM_COMMIT, PAGE_READWRITE);
    if(m_pBuffer)
    {
        VirtualLock(m_pBuffer, Size);
    }

}

} //bluefish
} //videocards
} //bv