#pragma once
namespace bv
{

namespace videocards{
class CFrame
{
public:
	CFrame(unsigned int ID, unsigned int Size, unsigned int BytesPerLine);
	CFrame(unsigned char* buffer, unsigned int ID, unsigned int Size, unsigned int BytesPerLine);
	~CFrame();

public:
	unsigned int	m_nFrameID;
	unsigned int	m_nCardBufferID;
	unsigned int	m_nSize;
	unsigned int	m_nBytesPerLine;
	unsigned char*	m_pBuffer;
	unsigned long	m_lFieldCount;

};
}
}