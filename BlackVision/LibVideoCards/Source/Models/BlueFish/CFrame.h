#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"


namespace bv { namespace videocards { namespace bluefish {

class CFrame
{
public:

	CFrame(BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine);
    CFrame(const BLUE_UINT8* buffer, BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine);
	~CFrame();

public:

	BLUE_UINT32	m_nFrameID;
	BLUE_UINT32	m_nCardBufferID;
	BLUE_UINT32	m_nSize;
	BLUE_UINT32	m_nBytesPerLine;
	BLUE_UINT8*	m_pBuffer;
	ULONG		m_lFieldCount;

private:

    void        Init        (BLUE_UINT32 ID, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine);

};

} //bluefish
} //videocards
} //bv
