#pragma once

#ifndef H_GUARD_FIFOBUFFER
#define H_GUARD_FIFOBUFFER

#include "win_sock.h"
#include <queue>
#include "BlueFish/inc/BlueVelvet4.h"
#include "Utils.h"
#include "BlueLock.h"
#include "../../ThreadSafeQueue.h"
#include "CFrame.h"
namespace bv
{

namespace videocards{
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
using namespace std;
class CFifoBuffer
{
public:
	CFifoBuffer();
	~CFifoBuffer();

	void	Init(BLUE_UINT32 Count, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine);

	//void	PutFreeBuffer(CFrame* pFrame);
	//void	PutLiveBuffer(CFrame* pFrame);
	//CFrame* GetFreeBuffer();
	//CFrame* GetLiveBuffer();
	ThreadSafeQueue<std::shared_ptr<CFrame>,16> m_threadsafebuffer;
	ULONG m_GoldenSize;
	ULONG m_BytesPerLine;

private:
	//std::queue<CFrame*>	m_qFreeBuffers;
	//std::queue<CFrame*>	m_qLiveBuffers;
	//std::queue<CFrame*>	m_qBuffers;
	//HANDLE			m_hFreeMutexEmpty;
	//HANDLE			m_hFreeMutexLive;
	//HANDLE			m_hLiveMutexEmpty;
	//HANDLE			m_hLiveMutexLive;
	//BlueLock		m_FreeLock;
	//BlueLock		m_LiveLock;
};
}
}

#endif	//H_GUARD_FIFOBUFFER
