#pragma once
#ifndef H_GUARD_BLUELOCK
#define H_GUARD_BLUELOCK

#include "win_sock.h"
namespace bv
{

namespace videocards{
class BlueLock
{
public:
	BlueLock();
	~BlueLock();

	void lock();
	void unlock();

private:
	HANDLE	m_hMutex;
};
}
}
#endif //H_GUARD_BLUELOCK
