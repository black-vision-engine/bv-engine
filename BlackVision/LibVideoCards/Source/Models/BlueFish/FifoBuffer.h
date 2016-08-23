#pragma once

#include <queue>

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"
#include "Utils.h"
//#include "../../ThreadSafeQueue.h"
#include "CFrame.h"
#include "BlueLock.h"


namespace bv { namespace videocards { namespace bluefish {

class CFifoBuffer
{
public:

	CFifoBuffer();
	~CFifoBuffer();

	void	Init(BLUE_UINT32 Count, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine);
	void	PutFreeBuffer(CFrame* pFrame);
	void	PutLiveBuffer(CFrame* pFrame);
	CFrame* GetFreeBuffer();
	CFrame* GetLiveBuffer();
    bool    IsLiveBufferEmpty();

private:

	std::queue<CFrame*>	m_qFreeBuffers;
	std::queue<CFrame*>	m_qLiveBuffers;
	std::queue<CFrame*>	m_qBuffers;
	HANDLE			m_hFreeMutexEmpty;
	HANDLE			m_hFreeMutexLive;
	HANDLE			m_hLiveMutexEmpty;
	HANDLE			m_hLiveMutexLive;
	BlueLock		m_FreeLock;
	BlueLock		m_LiveLock;

//	void	PushKillerFrame();
//	ThreadSafeQueue<std::shared_ptr<CFrame>,1> m_threadsafebuffer;
//	ULONG m_GoldenSize;
//	ULONG m_BytesPerLine;
//
//private:
//
//    std::shared_ptr<CFrame> m_killerFrame;

};

} //bluefish
} //videocards
} //bv

