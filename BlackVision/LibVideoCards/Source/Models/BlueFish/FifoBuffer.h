#pragma once

#include "win_sock.h"
#include <queue>
#include "BlueFish/inc/BlueVelvet4.h"
#include "Utils.h"
#include "../../ThreadSafeQueue.h"
#include "CFrame.h"
namespace bv
{

namespace videocards{


class CFifoBuffer
{
public:
	CFifoBuffer();
	~CFifoBuffer();

	void	Init(BLUE_UINT32 Count, BLUE_UINT32 Size, BLUE_UINT32 BytesPerLine);

	void	PushKillerFrame();
	ThreadSafeQueue<std::shared_ptr<CFrame>,2> m_threadsafebuffer;
	ULONG m_GoldenSize;
	ULONG m_BytesPerLine;

private:
    std::shared_ptr<CFrame> m_killerFrame;
};
}
}

