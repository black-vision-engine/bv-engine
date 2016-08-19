#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"


namespace bv { namespace videocards { namespace bluefish {

class CFrame
{
public:

	CFrame(BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine);
    //CFrame(unsigned char* buffer, unsigned int ID, unsigned int Size, unsigned int BytesPerLine);
	~CFrame();

public:

	BLUE_UINT32	m_nFrameID;
	BLUE_UINT32	m_nCardBufferID;
	BLUE_UINT32	m_nSize;
	BLUE_UINT32	m_nBytesPerLine;
	BLUE_UINT8*	m_pBuffer;
	ULONG		m_lFieldCount;

};

} //bluefish
} //videocards
} //bv
