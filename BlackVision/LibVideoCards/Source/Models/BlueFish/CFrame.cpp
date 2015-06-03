#include "CFrame.h"
#include "win_sock.h"
namespace bv
{

namespace videocards{
CFrame::CFrame(unsigned int ID, unsigned int Size, unsigned int BytesPerLine) :
	m_nFrameID(ID),
	m_nCardBufferID(0),
	m_nSize(Size),
	m_nBytesPerLine(BytesPerLine),
	m_pBuffer(NULL),
	m_lFieldCount(0)
{
	m_pBuffer = (unsigned char*)VirtualAlloc(NULL, Size, MEM_COMMIT, PAGE_READWRITE);
	if(m_pBuffer)
    {
		VirtualLock(m_pBuffer, Size);
        if(ID == 0)
        {
            for( unsigned int i = 0; i < Size; i++)
            {
                m_pBuffer[i] = 0;
            }
        }
    }
}

CFrame::CFrame(unsigned char* buffer, unsigned int ID, unsigned int Size, unsigned int BytesPerLine) :
	m_nFrameID(ID),
	m_nCardBufferID(0),
	m_nSize(Size),
	m_nBytesPerLine(BytesPerLine),
	m_pBuffer(NULL),
	m_lFieldCount(0)
{
	m_pBuffer = (unsigned char*)VirtualAlloc(NULL, Size, MEM_COMMIT, PAGE_READWRITE);
	if(m_pBuffer)
		VirtualLock(m_pBuffer, Size);

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
}
}
