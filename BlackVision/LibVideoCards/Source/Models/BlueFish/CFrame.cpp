#include "CFrame.h"


namespace bv { namespace videocards { namespace bluefish {


CFrame::CFrame(BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
{
    Init( ID, Size, BytesPerLine );
}

CFrame::CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
{
    Init( ID, Size, BytesPerLine );
    memcpy(m_pBuffer, buffer, Size);
}

CFrame::~CFrame()
{
	if(m_pBuffer)
	{
		VirtualUnlock(m_pBuffer, m_nSize);
		VirtualFree(m_pBuffer, 0, MEM_RELEASE);
	}
}

void    CFrame::Init        (BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
{
    m_nFrameID = ID;
    m_nCardBufferID = 0;
	m_nSize = Size;
	m_nBytesPerLine = BytesPerLine;
	m_pBuffer = NULL;
	m_lFieldCount = 0;

	m_pBuffer = (BLUE_UINT8*)VirtualAlloc(NULL, Size, MEM_COMMIT, PAGE_READWRITE);
	if(m_pBuffer)
    {
		VirtualLock(m_pBuffer, Size);
    }
}

} //bluefish
} //videocards
} //bv