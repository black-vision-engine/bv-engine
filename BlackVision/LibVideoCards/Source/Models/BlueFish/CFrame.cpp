#include "CFrame.h"


namespace bv { namespace videocards { namespace bluefish {


CFrame::CFrame(BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
{
    Init( ID, Size, BytesPerLine,0 );
}

CFrame::CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd)
{
    Init( ID, Size, BytesPerLine,Odd );
    memcpy(m_pBuffer, buffer, Size);
}

CFrame::CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd, unsigned int AudioSize, const unsigned int* AudioData, BVTimeCode TimeCode, BVFrameDescriptor FrameDescriptor)
{
	Init(ID, Size, BytesPerLine, Odd);
	memcpy(m_pBuffer, buffer, Size);

	m_AudioSize = AudioSize;
	m_FrameInformation = FrameDescriptor;
	m_TimeCode = TimeCode;
	if (AudioSize != 0)
	{
		
		m_AudioData = new unsigned int[16 * 2002];
		memset(m_AudioData, 0, 2002 * 16 *4);
		memcpy(m_AudioData, AudioData, AudioSize);
		
		if (m_AudioData)
		{
			//VirtualLock(m_AudioData, AudioSize);
		}
	}	
}

CFrame::~CFrame()
{
	if (m_pBuffer)
	{
		VirtualUnlock(m_pBuffer, m_nSize);
		VirtualFree(m_pBuffer, 0, MEM_RELEASE);
	}

	if (m_AudioData)
	{
		delete m_AudioData;
		//VirtualUnlock(m_AudioData, 16*2002);
		//VirtualFree(m_AudioData, 0, MEM_RELEASE);
	}
}

void    CFrame::Init        (BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine, int Odd)
{
	m_AudioData = NULL;
    m_nFrameID = ID;
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