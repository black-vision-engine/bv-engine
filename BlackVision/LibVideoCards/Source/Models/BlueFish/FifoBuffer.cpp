
#include "FifoBuffer.h"
namespace bv
{

namespace videocards{
using namespace std;

CFifoBuffer::CFifoBuffer() 
{
}

CFifoBuffer::~CFifoBuffer()
{
}

void CFifoBuffer::Init(BLUE_UINT32 Count, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine)
{
	{Count;}
	m_GoldenSize = Size;
	m_BytesPerLine = BytesPerLine;
    m_killerFrame = std::make_shared<CFrame>(0,m_GoldenSize,m_BytesPerLine);
}

void CFifoBuffer::PushKillerFrame()
{
    
	cout << "Pushing Killer Frame in Playback " << endl;
    m_threadsafebuffer.push(m_killerFrame);
}
}
}
